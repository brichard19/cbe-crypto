#include <stdarg.h>

#include "libcell.h"
#include "cbecrypto_error.h"
#include "cbecrypto_commands.h"
#include "specmd.h"
#include "ctx.h"


static int cbecrypto_send_spe_command( struct cbecrypto_spe_ctx *ctx, int cmd, struct cbecrypto_cmd_arg *args, int argCount, uint32_t *result )
{
    int i = 0;
    printf( "Sending cmd %d\n", cmd );
    int rc = cell_send_int32( ctx->spe, cmd );
    
    if( rc ) {
        goto end;
    }
    
    for( i = 0; i < argCount; i++ ) {
        switch( args[ i ].type ) {
        case TYPE_UINT32:
            rc = cell_send_int32( ctx->spe, args[ i ].ulValue );
            break;
        case TYPE_UINT64:
            rc = cell_send_int64( ctx->spe, args[ i ].ullValue );
            break;
        default:
            rc = EINVAL;
        }
        
        if( rc ) {
            goto end;
        }
    }
    
    if( result != NULL ) {
        printf( "Waiting for result\n" );
        rc = cell_recv_int32( ctx->spe, result );
    }
    
end:
    return rc;
}

int cbecrypto_cmd_exit( struct cbecrypto_spe_ctx *spe )
{
    return cbecrypto_send_spe_command( spe, CMD_EXIT, NULL, 0, NULL );
}

int cbecrypto_cmd_sha1_init( struct cbecrypto_spe_ctx *spe )
{
    return cbecrypto_send_spe_command( spe, CMD_SHA1_INIT, NULL, 0, NULL );
}

int cbecrypto_cmd_sha1_update( struct cbecrypto_spe_ctx *spe, unsigned char *buf, size_t len )
{
    int rc = 0;
    uint32_t response = 0;
    
    struct cbecrypto_cmd_arg args[] = {
            { .type = TYPE_UINT64, .ullValue = (uint64_t)buf },
            { .type = TYPE_UINT32, .ulValue = (uint32_t)len }
    };
    
    rc = cbecrypto_send_spe_command( spe, CMD_SHA1_UPDATE, args, 2, &response );
    
    return rc;    
}

int cbecrypto_cmd_sha1_get_state( struct cbecrypto_spe_ctx *spe, unsigned char *state )
{
    uint32_t response = 0;
    int rc = 0;
    
    struct cbecrypto_cmd_arg args[] = {
            { .type = TYPE_UINT64, .ullValue = (uint64_t)state }
    };
    
    rc = cbecrypto_send_spe_command( spe, CMD_SHA1_GET_STATE, args, 1, &response );
    
    return rc;
}
