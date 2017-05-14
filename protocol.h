#ifndef PROTOCOL_H
#define PROTOCOL_H

const int MESSAGE_DATA_SIZE = 2;

typedef struct message {
    unsigned char target;
	unsigned char command;
	unsigned char data[2];
} message_t;

#endif
