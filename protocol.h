#ifndef PROTOCOL_H
#define PROTOCOL_H

#define RECEIVER_ROLE 0
#define TRANSMITTER_ROLE 1

const char NUM_PARAMETERS = 2;

extern char role;

typedef struct message {
    unsigned char target;
	unsigned char command;
	unsigned char parameters[2];
} message_t;

#endif
