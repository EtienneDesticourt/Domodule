#include "decoder.h"
#include "encoder.h"
#include "protocol.h"

char state = STATE_IDLE;
char last_pulse;
char num_wake_pulses = 0x00;
message_t message;
char received_byte = 0x00;
char num_received_bytes = 0x00;
char num_received_bits = 0x00;

void decode(char low_pulse) {
    int pulse_width = calc_pulse_width();
    switch (state) {
        case STATE_IDLE:
            if (low_pulse == 1) {
                state = STATE_WAKE;
            }
            break;
        case STATE_WAKE:
            // Wake up signal: 5-40 zeros
            if (low_pulse == 1 && pulse_width == 1 && num_wake_pulses < 40) {
                num_wake_pulses += 1;
                break;
            }
            // First sync: 3T high
            else if (low_pulse == 1 && pulse_width == 3 && num_wake_pulses > 4) {
                state = STATE_SYNC;
                break;
            }
            reset_FSM();
            break;
        case STATE_SYNC:
            if (low_pulse == 0 && pulse_width == 3) {
                state = STATE_FILL;
            }
            else { // No correct second sync signal (3T low) following the first
                reset_FSM();
            }
            break;
        case STATE_FILL:
            if (low_pulse == 1 && pulse_width == 1) { // Filler 0 to set back to low
                state = STATE_MESS_1T2T_LO;
            }
            else {
                reset_FSM(); // Unknown state: reset
            }
            break;
        case STATE_MESS_1T2T_LO:
            if (low_pulse == 1) {
                if (pulse_width == 1) {
                    state = STATE_MESS_1T_HI;
                    break;
                }
                else if (pulse_width == 2) {
                    set_bit(&received_byte, num_received_bits); // Write 1
                    num_received_bits++;
                    state = STATE_MESS_1T2T_HI;
                    break;
                }
            }
            reset_FSM();
            break;
        case STATE_MESS_1T_HI:
            if (low_pulse == 0 && pulse_width == 1) {
                num_received_bits++; // 'Write' 0 (byte starts cleared)
                state = STATE_MESS_1T2T_LO;
            }
            else {
                reset_FSM();
            }
            break;
        case STATE_MESS_1T2T_HI:
            if (low_pulse == 1) {
                if (pulse_width == 1) {
                    state = STATE_MESS_1T_LO;
                    break;
                }
                else if (pulse_width == 2) {
                    num_received_bits++; // 'Write' 0 (byte starts cleared)                    
                    state = STATE_MESS_1T2T_LO;
                    break;                    
                }
            }
            reset_FSM();
            break;
        case STATE_MESS_1T_LO:
            if (low_pulse == 0 && pulse_width == 1) {
                set_bit(&received_byte, num_received_bits); // Write 1
                num_received_bits++;
                state = STATE_MESS_1T2T_HI;
            }
            else {
                reset_FSM();
            }
            break;
        default:
            break;
    }
    
    // Write last byte received to message
    if (num_received_bits == 8) {
        num_received_bits = 0;
        num_received_bytes++;
        if (num_received_bytes == 1) {
            message.target = received_byte;
        }
        else if (num_received_bytes == 2) {
            message.command = received_byte;
        }
        else if (num_received_bytes == 3) {
            message.parameters[0] = received_byte;
        }
        else if (num_received_bytes == 4) {
            message.parameters[1] = received_byte;
        }
        else {
            reset_FSM();
        }
        received_byte = 0x00;
    }
    record_pulse_time();
}

void record_pulse_time() {
    
}

void reset_FSM() {
    state = STATE_IDLE;
    num_wake_pulses = 0;
    num_received_bytes = 0;
    num_received_bits = 0;
    received_byte = 0x00;
}

char calc_pulse_width() {
    return 0;
}