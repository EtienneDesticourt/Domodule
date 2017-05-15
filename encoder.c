#include "encoder.h"

/**
 * Encodes a message using Manchester encoding.
 * @param message The message to encode
 * @param encoded The char array to encode it into
 */
void encode(message_t *message, unsigned char *encoded) {
    encode_byte(message->target, encoded);
    encode_byte(message->command, encoded+2);
    
    
    for (int i=0; i<NUM_PARAMETERS; i++) {
        encode_byte(message->parameters[i], encoded+4+i*2);
    }
}

/**
 * Encodes a byte using Manchester encoding.
 * LO-HI for 1, and HI-LO for 0.
 * @param byte The byte to encode
 * @param encoded The cleared char array of size >= 2 to encode the byte into
 */
void encode_byte(unsigned char byte, unsigned char *encoded) {
    unsigned char encoded_byte;
    
    for (int j=0; j<2; j++) {
        encoded_byte = 0x00;
        for (int i=0; i<4; i++) {
            if ((byte >> i+4*j) & 1) {
                set_bit(&encoded_byte, i*2+1); //LO-HI
            }
            else {
                set_bit(&encoded_byte, i*2); //HI-LO
            }
        }
        encoded[j] = encoded_byte;
    }
}

/**
 * Sets the bit at given position in the given array.
 * @param encoded
 * @param position
 */
void set_bit(unsigned char *encoded, int position) {
    *encoded |= (1 << position);
    /*
    int byte_offset = position / 8;
    int bit_offset = position - byte_offset * 8;
    *(encoded+byte_offset) |= (1 << bit_offset);
    */
}

