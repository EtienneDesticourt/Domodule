#ifndef RF_TRANSMITTER_H
#define RF_TRANSMITTER_H

#include "protocol.h"

typedef struct transmitter_context {
    unsigned char sent_flag;
    unsigned char *data;
    unsigned char data_size;
    unsigned char current_byte;
    unsigned char current_bit;
} transmitter_ctx;

extern transmitter_ctx TCTX;

void init_transmitter_context();

void setup_transmitter_interrupt();

void handle_transmitter_interrupts();

void increase_bit_counter();

void send_data(char *data, char data_size);

void send_wake_signal();

void send_sync_signal();

void send_message(message_t *message);

#endif