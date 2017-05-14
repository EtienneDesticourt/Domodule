#include <xc.h>
#include <stdio.h>
#include <string.h>
#include "encoder.h"


void main(void) {
    printf("hello");
    message_t message;
    message.target = 0x0C;
    message.command = 0x1D;
    message.data[0] = 0xD1;
    message.data[1] = 0x01;
    
    unsigned char encoded[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    
    encode(&message, encoded);
    for (int i=0; i<8; i++) {
        printf("0x%02x", encoded[i]);
    }
}
