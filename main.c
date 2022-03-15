#include <msp430.h> 
#include <msp430fr2355.h>


//Coeficientes IIR Fs=820 Fc=120
float b[3]={1,2,1};
float a[3]={1, -1.6822, 0.7262};;

int count=0;

float outx;
float outy;

int ej[3]={0};
int convo[21];
unsigned int i,o,y,k,j,g;

float salida[3]={0};
int bandera;
long SalidaActual;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer


            PM5CTL0 &= ~LOCKLPM5;
            FRCTL0 = FRCTLPW | NWAITS_2 ; // Configure two FRAM waitstate as required by the device datasheet for MCLK
                                              // operation at 24MHz(beyond 8MHz) before configuring the clock system.

            P5SEL1=0x04;  //ADC en 5.2
            P5SEL0=0x04;

            P1SEL0=0;
            P1SEL1=0;

            P6SEL0=0;
            P6SEL1=0;

            P1DIR=0xFF;
            P6DIR=0xFF;

                    __bis_SR_register(SCG0);                     // disable FLL
                    CSCTL3 |= SELREF__XT1CLK;                    // Set XT1 as FLL reference source
                    CSCTL0 = 0;                                  // clear DCO and MOD registers
                    CSCTL1 |= DCORSEL_7;                         // Set DCO = 24MHz
                    CSCTL2 = FLLD_0 + 731;                       // DCOCLKDIV = 24MHz*/



                    // Configure ADC
                    ADCCTL0 |= ADCON;                                         // ADCON
                    ADCCTL1=0x0200;//ADCCTL1 |= ADCSHS_0 | ADCCONSEQ_2;               // repeat single channel; ADCSC BIT for start sample
                    ADCCTL2 &= ~ADCRES;                                       // clear ADCRES in ADCCTL
                    ADCCTL2 |= ADCRES_2;                                      // 12-bit conversion results
                    ADCMCTL0 |= ADCINCH_10 | ADCSREF_0;                       // A10 ADC input select; Vref=1.5V

                    ADCCTL0 |= ADCENC;                                        // ADC Enable


                    TB0CCTL0 |= CCIE;                             // TBCCR0 interrupt enabled
                    TB0CCR0 = 1000;                               //Cuenta para interrupcion, mientras menor sea el valor mas rÃ¡pido harÃ¡ el muestreo, cuenta maxima 2^16-1 (Lo mas lento)
                    TB0CTL = TBSSEL_SMCLK | MC_UP;             // SMCLK, UP mode

                    //***Con CLK a 24MHz tenemos lo siguiente:
                    //65536=366.21Hz
                    //50000 = 480Hz
                    //60000 = 400Hz
                    //20000 = 1.2kHz
                    //10000 = 2.4kHz
                    //1000  = 24kHz
                    //100   = 240kHz

                    //***Con CLK a 1MHz tenemos lo siguiente:
                    //65536 = 15.25Hz
                    //50000 = 20Hz
                    //60000 = 16.66Hz
                    //20000 = 50Hz
                    //10000 = 100Hz
                    //1000  = 1kHz
                    //3900  = 256.41 Hz (Frecuencia de muestreo de nuestros coeficientes)
                    //100   = 10kHz


                    __bis_SR_register(LPM0_bits | GIE);


                    //P1OUT &= ~BIT0; //Apagar
                    //bandera=0;



                    for(;;){

                    }



}

//Interrupcion Timer
#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR (void)
{

    ADCCTL0 |= ADCSC_1; //Iniciar Conversion
    while(ADCIFG & ADCIFG0 == 0); //Espera a que termine la conversion


    //Guardar lectura en el bit mas significativo
                ej[0]=ej[1];
                ej[1]=ej[2];
                ej[2]=ADCMEM0>>4;

                SalidaActual=((b[0]*ej[2]+b[1]*ej[1]+b[2]*ej[0])-(a[1]*salida[2]+ a[2]*salida[1]));

                salida[1]=salida[2];
                salida[2]=SalidaActual;

                P1OUT=SalidaActual*0.01;

    //Salir de la interrupcion
    _BIC_SR_IRQ(LPM0_bits);


}
