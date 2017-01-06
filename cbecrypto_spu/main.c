#include <stdio.h>
#include <spu_mfcio.h>
#include "libcell.h"
#include "cbecrypto_commands.h"
#include "cbecrypto_spu.h"


// Declared extern in cbecrypto_spu.h
//unsigned char *_dataBuffer;
//union cbecrypto_spu_ctx *_ctxBuffer;

unsigned char _dataBuffer[ 65536 ] __attribute__((aligned(128)));
struct cbecrypto_spu_ctx _ctxBuffer;

int main()
{
    uint32_t command;
    
    do {
        command = spe_recv_int32();
        
        switch( command ) {
        
        case CMD_EXIT:
            printf( "EXIT\n" );
            break;
        
        // SHA-1 commands
        case CMD_SHA1_INIT:
            sha1_init();
            break;
        case CMD_SHA1_UPDATE:
            sha1_update();
            break;
        case CMD_SHA1_GET_STATE:
            sha1_get_state();
            break;
        default:
            break;
        }
        
    }while( command != CMD_EXIT );
    
    return 0;
}
