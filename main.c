#include <msp430fr2355.h>

#define functionset 0x3C
#define displayonoff 0x0E
#define cleardsp 0x01
#define entry 0x05
#define cursorshift 0x18


void lcd_init(void);  //funcion para inicializar el LCD
void lcd_write(char); //funcion para escribir un caracter

#define E1 P6OUT|=BIT1  //Enable en 0
#define E0 P6OUT&=~BIT1 //Enable en 1
#define RS1 P6OUT|=BIT0 //RS en 0
#define RS0 P6OUT&=~BIT0 //RS en 1


void main(void)
{

    FRCTL0 = FRCTLPW | NWAITS_2 ; // Configure two FRAM waitstate as required by the device datasheet for MCLK
                                  // operation at 24MHz(beyond 8MHz) before configuring the clock system.
    PM5CTL0 &= ~LOCKLPM5; // Disable the GPIO power-on default high-impedance mode to activate
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

	//CLK a 16MHZ
	__bis_SR_register(SCG0);                     // disable FLL
	CSCTL3 |= SELREF__XT1CLK;                    // Set XT1 as FLL reference source
	CSCTL0 = 0;                                  // clear DCO and MOD registers
	CSCTL1 |= DCORSEL_5;                         // Set DCO = 16MHz
	CSCTL2 = FLLD_0 + 487;                       // DCOCLKDIV = 16MHz


	//Reset de direcciones
	P1OUT &= ~(0xFF);
	P6OUT &= ~(0xFF);
	P1DIR = 0;
	P6DIR = 0;
	//Definimos Pines de Salida
	P1DIR = 0xFF; //Todos los pines 1 como salida
	P6DIR = 0x03; //Bit 0 y 1 del P6 los ponemos como salida
	P1SEL0=0; //Indica vinculacion a otro periferico;
	P6SEL0=0;



	//*****Configuracion de PINES*********
	//*****Comunicacion a 8 BITS**********
	// RS - > P6.0  H=Data L=Instruction
	// E  - > P6.1
	// D0 - > P1.0
	// D1 - > P1.1
	// D2 - > P1.2
	// D3 - > P1.3
	// D4 - > P1.4
	// D5 - > P1.5
	// D6 - > P1.6
	// D7 - > P1.7

	lcd_init();

	__delay_cycles(30);

	for(;;){
	lcd_write('o');
	__delay_cycles(10000000);
	lcd_write('i');
	__delay_cycles(10000000);
	lcd_write('l');
	__delay_cycles(10000000);
	lcd_write('a');
	__delay_cycles(10000000);
	lcd_write('s');
	__delay_cycles(10000000);
	lcd_write(' ');
	__delay_cycles(10000000);
	lcd_write('a');
	__delay_cycles(10000000);
	lcd_write('y');
	__delay_cycles(10000000);
	lcd_write(' ');
	__delay_cycles(10000000);
	lcd_write(' ');
	__delay_cycles(10000000);


	}


	
}

void lcd_init(void){

    __delay_cycles(240000); //15ms por datasheet

    RS0; //RS en 0 para enviar comandos

    //Function Set 0
    __delay_cycles(1); //CLK a 16MHz es decir 62.5ns por ciclo
    E1;
    P1OUT = 0x30;
    __delay_cycles(2);
    E0;
    __delay_cycles(1);
    P1OUT = 0;
    __delay_cycles(65600);  //4.1ms

    //Function Set 1
    E1;
    P1OUT = 0x30;
    __delay_cycles(2);
    E0;
    __delay_cycles(1);
    P1OUT = 0;
    __delay_cycles(1600);  //100us

    //Function Set 2
    E1;
    P1OUT = 0x30;
    __delay_cycles(2);
    E0;
    __delay_cycles(1);
    P1OUT = 0;

    //Function Set
    __delay_cycles(1); //CLK a 16MHz es decir 62.5ns por ciclo
    E1;
    P1OUT = functionset;
    __delay_cycles(2);
    E0;
    __delay_cycles(1);
    P1OUT = 0;
    __delay_cycles(624);  //39us

    //Display ONOFF
    __delay_cycles(1); //CLK a 16MHz es decir 62.5ns por ciclo
    E1;
    P1OUT = displayonoff;
    __delay_cycles(2);
    E0;
    __delay_cycles(1);
    P1OUT = 0;
    __delay_cycles(624);  //39us

    //Display CLEAR
    __delay_cycles(1); //CLK a 16MHz es decir 62.5ns por ciclo
    E1;
    P1OUT = cleardsp;
    __delay_cycles(2);
    E0;
    __delay_cycles(1);
    P1OUT = 0;
    __delay_cycles(25320);  //1.53us

    //Entry Mode Set
    __delay_cycles(1); //CLK a 16MHz es decir 62.5ns por ciclo
    E1;
    P1OUT = entry;
    __delay_cycles(2);
    E0;
    __delay_cycles(1);
    P1OUT = 0;
    __delay_cycles(624);  //39us

    //Cursor Shift
    __delay_cycles(1); //CLK a 16MHz es decir 62.5ns por ciclo
    E1;
    P1OUT = cursorshift;
    __delay_cycles(2);
    E0;
    __delay_cycles(1);
    P1OUT = 0;
    __delay_cycles(624);  //39us


}

void lcd_write(char a){
    //Write Char
    RS1;
    __delay_cycles(1); //CLK a 16MHz es decir 62.5ns por ciclo
    E1;
    P1OUT = a;
    __delay_cycles(2);
    E0;
    __delay_cycles(624);  //39us
    P1OUT=0;

}
