#include <xc.h>
#include "encoder.h"


void main(void) {
    message_t message;
    message.target = 0x0C;
    message.command = 0x1D;
    message.parameters[0] = 0xD1;
    message.parameters[1] = 0x01;
    
    unsigned char encoded[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    
    encode(&message, encoded);
}
