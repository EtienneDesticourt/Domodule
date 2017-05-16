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
}
