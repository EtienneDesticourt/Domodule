#ifndef PROTOCOL_H
#define PROTOCOL_H

const int NUM_PARAMETERS = 2;

typedef struct message {
    unsigned char target;
	unsigned char command;
	unsigned char parameters[2];
} message_t;

#endif
