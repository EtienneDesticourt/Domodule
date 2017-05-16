#include <pic18f1330.h>
#include "decoder.h"



void setup_receiver_interrupts() {
    // Enables global interrupts
    INTCONbits.GIE = 1;
    // Enables INT2 and INT3 interrupts on rising and falling edge
    INTCON3bits.INT2IF = 0; // Clear flags
    INTCON3bits.INT3IF = 0;
    INTCON3bits.INT2IE = 1; // Enable
    INTCON3bits.INT3IE = 1;
    INTCON2bits.INTEDG2 = 1; // On rising edge
    INTCON2bits.INTEDG3 = 0; // On falling edge
    // Setup internal clock
    OSCCONbits.IRCF = 0b000; // Set internal clock to 31kHz
    
    // Configures Timer 0
    T0CONbits.TMR0ON = 1; // Turns on timer
    T0CONbits.T016BIT = 1; // Configured as 8 bit weirdly
    T0CONbits.T0CS = 0; // Use internal clock
    // Sets 1:2 prescaler on FOSC/4 : FOSC/8 ~= 4kHz.
    T0CONbits.T0PS = 0b000;
    /*
    // Enables Timer 0 interrupt to calculate pulse width
    INTCONbits.T0IF = 0; 
    INTCONbits.T0IE = 1;
     * */
}

void handle_receiver_interrupts() {
    if (INTCON3bits.INT2IF == 1) {
        decode(POSITIVE_EDGE);
        INTCON3bits.INT2IF = 0;        
    }
    
    if (INTCON3bits.INT3IF == 1) {
        decode(NEGATIVE_EDGE);
        INTCON3bits.INT3IF = 0;
    }
    /*
    if (INTCONbits.T0IF == 1) {
        INTCONbits.T0IF = 0;
    }
     * */
}
