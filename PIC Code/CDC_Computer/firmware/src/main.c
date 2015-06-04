
#include<xc.h> // processor SFR definitions
#include<sys/attribs.h> // __ISR macro



int main ( void )
{
    /* Initialize all MPLAB Harmony modules, including application(s). */
    SYS_Initialize ( NULL );
    start_up();                 // startup

    ANSELBbits.ANSB13 = 0;
    TRISBbits.TRISB13 = 1;      // set up USER pin as input

    TRISBbits.TRISB7 = 0;       // set up LED1 pin as a digital output
    ANSELBbits.ANSB15 = 0;
    TRISBbits.TRISB15 = 0; 
    LATBbits.LATB5 = 0;
            
    TRISBbits.TRISB8 = 0;       // set up pin 8 as digital ouput for wheel 2
    TRISBbits.TRISB4 = 0;       // set up pin 4 as digital ouput for wheel 1
      
    // set up RB4 as OC1 using Timer2 at 1kHz
    //ANSELBbits.ANSB15 = 0;
    RPB4Rbits.RPB4R = 0b0101;  
    
    OC1CONbits.OCTSEL = 0;       // enable Timer2 for OC1    
    T2CONbits.TCKPS = 0b011;     // Timer2 prescaler N=8:1
    PR2 = 4999;                  // period = (PR3+1) * N * 25 ns = 1000 us, 1kHz
    TMR2 = 0;                    // initial TMR2 count is 0
    OC1CONbits.OCM = 0b110;      // PWM mode without fault pin; other OC1CON bits are defaults
    OC1RS = 0;                // duty cycle = OC1RS/(PR3+1) = 50%
    OC1R = 2500;                 // initialize before turning OC1 on; afterward it is read-only
    T2CONbits.ON = 1;            // turn on Timer2
    OC1CONbits.ON = 1;           // turn on OC1
    //set up RB5 as phase control pin for RB4, wheel 1
    TRISBbits.TRISB5 = 0;  
    LATBbits.LATB5 = 1;
    
    // set up RB8 as OC2 using Timer 3 at 1kHz
    RPB8Rbits.RPB8R = 0b0101; 
    OC2CONbits.OCTSEL = 1;       // enable Timer3 for OC2    
    T3CONbits.TCKPS = 0b011;     // Timer3 prescaler N=8:1
    PR3 = 4999;                  // period = (PR3+1) * N * 25 ns = 1000 us, 1kHz
    TMR3 = 0;                    // initial TMR2 count is 0
    OC2CONbits.OCM = 0b110;      // PWM mode without fault pin; other OC1CON bits are defaults
    OC2RS = 0;                // duty cycle = OC1RS/(PR3+1) = 50%
    OC2R = 2500;                 // initialize before turning OC1 on; afterward it is read-only
    T3CONbits.ON = 1;            // turn on Timer2
    OC2CONbits.ON = 1;           // turn on OC1
    
    //set up RB9 as phase control pin for RB8, wheel 2
    TRISBbits.TRISB9 = 0;  
    LATBbits.LATB9 = 1;

    while(PORTBbits.RB13==1)
    {
    while ( 1 )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );
//        _CP0_SET_COUNT(0); // set core timer to 0, remember it counts at half the CPU clock
//        while (_CP0_GET_COUNT() < 8000000) {
//            ;
//        }
        _CP0_SET_COUNT(0); // set core timer to 0, remember it counts at half the CPU clock
        while (_CP0_GET_COUNT() < 1000000) {
                    ;
        }

    }

    /* Execution should not come here during normal operation */
    
    return ( EXIT_FAILURE );
}
}


void start_up (void)
{
__builtin_disable_interrupts();

// set the CP0 CONFIG register to indicate that
// kseg0 is cacheable (0x3) or uncacheable (0x2)
// see Chapter 2 "CPU for Devices with M4K Core"
// of the PIC32 reference manual
__builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);

// no cache on this chip!

// 0 data RAM access wait states
BMXCONbits.BMXWSDRM = 0x0;

// enable multi vector interrupts
INTCONbits.MVEC = 0x1;

// disable JTAG to be able to use TDI, TDO, TCK, TMS as digital
DDPCONbits.JTAGEN = 0;

__builtin_enable_interrupts();
  

}
/*******************************************************************************
 End of File
*/

