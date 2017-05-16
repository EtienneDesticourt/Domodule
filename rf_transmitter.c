#include <pic18f1330.h>
#include "rf_transmitter.h"
#include "encoder.h"

transmitter_ctx TCTX;

const unsigned char WAKE_SIGNAL[2] = {0b01010101, 0b01010101};//,
                                      //0b10101010, 0b10101010,
                                      //0b10101010, 0b10101010,
                                      //0b10101010, 0b10101010}; // 32 * 0

// 3T HI, 3T LO, HI-LO (filler 0)
const unsigned char SYNC_SIGNAL[1] = {0b01000111};

unsigned char encoded_message[8] = {0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00};

void init_transmitter_context() {
    TCTX.sent_flag = 1;
}

void setup_transmitter_interrupt() {
    // Enables global interrupts
    INTCONbits.GIE = 1;
    // Setup internal clock
    OSCCONbits.IRCF = 0b000; // Set internal clock to 31kHz
    // Configure Timer 0
    T0CONbits.TMR0ON = 1; // Turns on timer
    T0CONbits.T016BIT = 1; // Configured as 8 bit weirdly
    T0CONbits.T0CS = 0; // Use internal clock
    T0CONbits.T0PS = 0b010;
    // Enables the Timer 0 interrupt
    INTCONbits.T0IF = 0; 
    INTCONbits.T0IE = 1;
    // Setup output pin
    PORTAbits.RA0 = 0;
    TRISAbits.RA0 = 0; // Set as output
    
}

void handle_transmitter_interrupts() {
    if (INTCONbits.T0IF == 1) {
        if (TCTX.sent_flag == 1) {
            return;
        }
        LATAbits.LATA0 = ((TCTX.data[TCTX.current_byte] >> TCTX.current_bit) & 1);
        //PORTAbits.RA0 = value;
        increase_bit_counter();
        INTCONbits.T0IF = 0;
    }
}

void increase_bit_counter() {
    TCTX.current_bit++;
    if (TCTX.current_bit > 7) {
        TCTX.current_bit = 0;
        TCTX.current_byte++;
        if (TCTX.current_byte >= TCTX.data_size) {
            TCTX.current_byte = 0;
            TCTX.sent_flag = 1;
            INTCONbits.T0IE = 0; // Turn off timer interrupt (without it stack overflow)
        }
    }
}

void send_data(unsigned char data[], char data_size) {
    TCTX.data = data;
    TCTX.data_size = data_size;
    TCTX.sent_flag = 0;
    INTCONbits.T0IE = 1;
}

void send_wake_signal() {
    send_data((unsigned char*) WAKE_SIGNAL, 2);
}

void send_sync_signal() {
    send_data((unsigned char*) SYNC_SIGNAL, 1);
}

void send_message(message_t *message) {
    send_wake_signal();
    while(TCTX.sent_flag == 0);
    send_sync_signal();
    while(TCTX.sent_flag == 0);
    encode(message, encoded_message);
    send_data(encoded_message, 8);
    while(TCTX.sent_flag == 0);
}