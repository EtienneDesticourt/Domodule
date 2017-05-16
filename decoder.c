#include <pic18f1330.h>
#include "decoder.h"
#include "encoder.h"
#include "protocol.h"

decoder_ctx DCTX;

void decode(char edge) {
    if (DCTX.new_message_flag == 1) {
        return;        
    }
    char pulse_width = calc_pulse_width();
    record_pulse_time();
    switch (DCTX.state) {
        case STATE_IDLE:
            if (edge == NEGATIVE_EDGE) {
                DCTX.state = STATE_WAKE;
            }
            break;
        case STATE_WAKE:
            // Wake up signal: 5-40 zeros
            if (edge == NEGATIVE_EDGE && pulse_width == 1 && DCTX.num_wake_pulses < 40) {
                DCTX.num_wake_pulses += 1;
                break;
            }
            // First sync: 3T high
            else if (edge == NEGATIVE_EDGE && pulse_width == 3 && DCTX.num_wake_pulses > 4) {
                DCTX.state = STATE_SYNC;
                break;
            }
            reset_FSM();
            break;
        case STATE_SYNC:
            if (edge == POSITIVE_EDGE && pulse_width == 3) {
                DCTX.state = STATE_FILL;
            }
            else { // No correct second sync signal (3T low) following the first
                reset_FSM();
            }
            break;
        case STATE_FILL:
            if (edge == POSITIVE_EDGE && pulse_width == 1) { // Filler 0 to set back to low
                DCTX.state = STATE_MESS_1T2T_LO;
            }
            else {
                reset_FSM(); // Unknown state: reset
            }
            break;
        case STATE_MESS_1T2T_LO:
            if (edge == POSITIVE_EDGE) {
                if (pulse_width == 1) {
                    DCTX.state = STATE_MESS_1T_HI;
                    break;
                }
                else if (pulse_width == 2) {
                    set_bit(&(DCTX.received_byte), DCTX.num_received_bits); // Write 1
                    DCTX.num_received_bits++;
                    DCTX.state = STATE_MESS_1T2T_HI;
                    break;
                }
            }
            reset_FSM();
            break;
        case STATE_MESS_1T_HI:
            if (edge == NEGATIVE_EDGE && pulse_width == 1) {
                DCTX.num_received_bits++; // 'Write' 0 (byte starts cleared)
                DCTX.state = STATE_MESS_1T2T_LO;
            }
            else {
                reset_FSM();
            }
            break;
        case STATE_MESS_1T2T_HI:
            if (edge == NEGATIVE_EDGE) {
                if (pulse_width == 1) {
                    DCTX.state = STATE_MESS_1T_LO;
                    break;
                }
                else if (pulse_width == 2) {
                    DCTX.num_received_bits++; // 'Write' 0 (byte starts cleared)                    
                    DCTX.state = STATE_MESS_1T2T_LO;
                    break;                    
                }
            }
            reset_FSM();
            break;
        case STATE_MESS_1T_LO:
            if (edge == POSITIVE_EDGE && pulse_width == 1) {
                set_bit(&(DCTX.received_byte), DCTX.num_received_bits); // Write 1
                DCTX.num_received_bits++;
                DCTX.state = STATE_MESS_1T2T_HI;
            }
            else {
                reset_FSM();
            }
            break;
        default:
            break;
    }
    
    // Write last byte received to message
    if (DCTX.num_received_bits == 8) {
        DCTX.num_received_bits = 0;
        DCTX.num_received_bytes++;
        if (DCTX.num_received_bytes == 1) {
            DCTX.message.target = DCTX.received_byte;
        }
        else if (DCTX.num_received_bytes == 2) {
            DCTX.message.command = DCTX.received_byte;
        }
        else if (DCTX.num_received_bytes == 3) {
            DCTX.message.parameters[0] = DCTX.received_byte;
        }
        else if (DCTX.num_received_bytes == 4) {
            DCTX.message.parameters[1] = DCTX.received_byte;
        }
        else {
            DCTX.new_message_flag = 1;
            reset_FSM();
        }
        DCTX.received_byte = 0x00;
    }
}

void record_pulse_time() {
    DCTX.last_pulse_time = TMR0L;   
}

void init_decoder_context() {
    reset_FSM();
    DCTX.new_message_flag = 0x00;
}

void reset_FSM() {
    DCTX.state = STATE_IDLE;
    DCTX.num_wake_pulses = 0;
    DCTX.num_received_bytes = 0;
    DCTX.num_received_bits = 0;
    DCTX.received_byte = 0x00;
}

unsigned char calc_pulse_width() {
    // Overflow will solve itself (3-254=7)
    unsigned char elapsed = ((unsigned char)TMR0L - DCTX.last_pulse_time);
    // timer runs at 4kHz so 4 counts for one pulse + margin
    if (elapsed > 0 && elapsed <= 6) {
        return 1;
    }    
    if (elapsed > 6 && elapsed <= 10) {
        return 2;
    }    
    if (elapsed > 10 && elapsed <= 14) {
        return 3;
    }    
    else {
        return -1;
    }
}