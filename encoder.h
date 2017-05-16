#ifndef ENCODER_H
#define ENCODER_H

#include "protocol.h"

void encode(message_t *message, unsigned char *encoded);

void encode_byte(unsigned char *byte, unsigned char *encoded);

void set_bit(unsigned char *encoded, int position);

#endif