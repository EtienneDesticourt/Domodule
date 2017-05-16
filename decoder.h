#ifndef DECODER_H
#define DECODER_H

#include "protocol.h"

#define STATE_IDLE 0
#define STATE_WAKE 1
#define STATE_SYNC 2
#define STATE_FILL 3
#define STATE_MESS_1T2T_LO 4
#define STATE_MESS_1T_HI   5
#define STATE_MESS_1T2T_HI 6
#define STATE_MESS_1T_LO   7

#define NEGATIVE_EDGE 0
#define POSITIVE_EDGE 1


typedef struct decoder_context {
    unsigned char state;
    unsigned char num_wake_pulses;
    message_t message;
    unsigned char new_message_flag;
    unsigned char received_byte;
    unsigned char num_received_bytes;
    unsigned char num_received_bits;
    unsigned char last_pulse_time;
} decoder_ctx;

extern decoder_ctx DCTX;

void decode(char low_pulse);

void record_pulse_time();

void init_decoder_context();

void reset_FSM();

char calc_pulse_width();

#endif
