#include <xc.h>
#include "protocol.h"
#include "encoder.h"
#include "decoder.h"
#include "rf_receiver.h"
#include "rf_transmitter.h"


void interrupt isr() {
    if (role == RECEIVER_ROLE) {
        handle_receiver_interrupts();        
    }
    else {
        handle_transmitter_interrupts();        
    }
}

void main(void) { 
    init_transmitter_context();
    setup_transmitter_interrupt();
    
    
    message_t data;
    data.target = 0x0C;
    data.command = 0x1D;
    data.parameters[0] = 0xD1;
    data.parameters[1] = 0x01;
    
    send_message(&data);
    
    while (1);
    
    
    /*
     
    init_decoder_context();
    setup_receiver_interrupts();
    message_t data;
    data.target = 0x0C;
    data.command = 0x1D;
    data.parameters[0] = 0xD1;
    data.parameters[1] = 0x01;
    
    unsigned char encoded[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    
    encode(&data, encoded);
    
    int last = 0;
    DCTX.width = 0;
    init_decoder_context();
    int current;
    int byte_offset, bit_offset;
    for (int j=0; j<4; j++) {
        for (int k=0; k<2; k++) {
            for (int i=0; i<8; i++) {
                DCTX.width += 1;
                byte_offset = j*2+k;
                bit_offset = i;
                current = ((encoded[byte_offset] >> bit_offset) & 1);
                if (last == 0 && current == 1) {
                    decode(POSITIVE_EDGE);
                    DCTX.width = 0;
                }
                else if (last == 1 && current == 0) {
                    decode(NEGATIVE_EDGE);
                    DCTX.width = 0;
                }
                last = current;
            }            
        }        
    }
    
    int temp = 3;
    for (int i = 0; i < 1; i++) {
        temp += 1;
    }
    
    */
}
