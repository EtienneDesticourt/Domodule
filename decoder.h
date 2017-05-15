#ifndef DECODER_H
#define DECODER_H

#include "protocol.h"

#define STATE_IDLE 1
#define STATE_WAKE 2
#define STATE_SYNC 3
#define STATE_FILL 4
#define STATE_MESS_1T2T_LO 5
#define STATE_MESS_1T_HI   6
#define STATE_MESS_1T2T_HI 7
#define STATE_MESS_1T_LO   8

#define NEGATIVE_EDGE 0
#define POSITIVE_EDGE 1

extern int width;
extern message_t message;

void decode(char low_pulse);

void record_pulse_time();

void reset_FSM();

char calc_pulse_width();

#endif
