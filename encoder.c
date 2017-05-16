#include "encoder.h"

/**
 * Encodes a message using Manchester encoding.
 * @param message The message to encode
 * @param encoded The char array to encode it into
 */
void encode(message_t *message, unsigned char *encoded) {
    encode_byte(&message->target, encoded);
    encode_byte(&message->command, encoded+2);
    
    
    for (int i=0; i<NUM_PARAMETERS; i++) {
        encode_byte(&message->parameters[i], encoded+4+i*2);
    }
}

/**
 * Encodes a byte using Manchester encoding.
 * LO-HI for 1, and HI-LO for 0.
 * @param byte The byte to encode
 * @param encoded The cleared char array of size >= 2 to encode the byte into
 */
void encode_byte(unsigned char *byte, unsigned char *encoded) {    
    for (int j=0; j<2; j++) {
        for (int i=0; i<4; i++) {
            encoded[j] |= (1 << i*2+((*byte >> i+4*j) & 1));
            /*
             if ((*byte >> i+4*j) & 1) { // Check bit at current bit in byte
                encoded[j] |= (1 << i*2+1); // LO-HI: 0
            }
            else {
                encoded[j] |= (1 << i*2); // HI-LO: 1
            }
             * Condensed above.
            */
        }
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