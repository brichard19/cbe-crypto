#include <stdlib.h>
#include <string.h>

#include "libcell.h"
#include "cbecrypto_error.h"
#include "cbecrypto_commands.h"
#include "specmd.h"
#include "sha1.h"
#include "ctx.h"

#define SHA1_BUF_SIZE 1024

static void destroy_sha1_ctx( struct cbecrypto_sha1_ctx **ctx )
{
    if( ctx == NULL || *ctx == NULL ) {
        return;
    }
    
    free( (*ctx)->buf );
    free( *ctx );
    *ctx = NULL;
}

static int create_sha1_ctx( struct cbecrypto_sha1_ctx **ctx )
{
    int rc = CBECRYPTO_SUCCESS;
    struct cbecrypto_sha1_ctx *tmpCtx = NULL;
    void *bufPtr = NULL;
    
    tmpCtx = (struct cbecrypto_sha1_ctx *)malloc( sizeof(struct cbecrypto_sha1_ctx) );
    if( tmpCtx == NULL ) {
        return CBECRYPTO_ERR_FAIL_ALLOC;
    }

    memset( tmpCtx, 0, sizeof(struct cbecrypto_sha1_ctx) );

    // Need to create a 128 byte aligned buffer to the SPE can read it
    if( posix_memalign( &bufPtr, 128, SHA1_BUF_SIZE ) ) {
        rc = CBECRYPTO_ERR_FAIL_ALLOC;
        goto end;
    }
    
    tmpCtx->buf = (unsigned char *)bufPtr;

    *ctx = tmpCtx;

end:
    if( rc != CBECRYPTO_SUCCESS ) {
        if( tmpCtx ) {
            destroy_sha1_ctx( &tmpCtx );
        }
    }
    
    return rc;
}

int cbecrypto_sha1_create( struct cbecrypto_ctx **ctx )
{
    struct cbecrypto_ctx *tmpCtx;
    int rc = CBECRYPTO_SUCCESS;
    
    if( ctx == NULL ) {
        return CBECRYPTO_ERR_NULL_CTX_PTR;
    }
    
    // Create CTX
    rc = cbecrypto_create_ctx( &tmpCtx );
    if( rc != CBECRYPTO_SUCCESS ) {
        goto end;
    }
    
    // Creat SHA1 CTX
    rc = create_sha1_ctx( &tmpCtx->sha1 );
    if( rc != CBECRYPTO_SUCCESS ) {
        goto end;
    }
    
    // Send the SHA1_INIT command to the SPE
    if( cbecrypto_cmd_sha1_init( tmpCtx->spe ) ) {
        rc = CBECRYPTO_ERR_IO;
        goto end;
    }
    
    *ctx = tmpCtx;

end:
    if( rc != CBECRYPTO_SUCCESS ) {
        destroy_sha1_ctx( &tmpCtx->sha1 );
        cbecrypto_destroy_ctx( &tmpCtx );
    }
    
    return rc;
}

int cbecrypto_sha1_update( struct cbecrypto_ctx *ctx, const unsigned char *data, size_t len )
{
    printf( "cbecrypto_sha1_update()\n" );
    struct cbecrypto_sha1_ctx *sha1 = NULL;
    int rc = CBECRYPTO_SUCCESS;

    if( ctx == NULL ) {
        return CBECRYPTO_ERR_NULL_CTX_PTR;
    }
    sha1 = ctx->sha1;
    
    while( len > 0 ) {
        size_t bytesToCopy = SHA1_BUF_SIZE - sha1->bufLen < len ? SHA1_BUF_SIZE - sha1->bufLen : len;
        
        memcpy( sha1->buf + sha1->bufLen, data, bytesToCopy );
        len -= bytesToCopy;
        sha1->bufLen += bytesToCopy;
        
        if( sha1->bufLen > 64 ) {
            size_t bytesToHash = sha1->bufLen & ~0x07;
            
            if( cbecrypto_cmd_sha1_update( ctx->spe, sha1->buf, bytesToHash ) ) {
                rc = CBECRYPTO_ERR_IO;
                goto end;
            }
            
            memcpy( sha1->buf, sha1->buf + bytesToHash, sha1->bufLen - bytesToHash );
            sha1->bufLen -= bytesToHash;
            sha1->msgLen += 512;
        }
    }

end:
    return rc;
}

int cbecrypto_sha1_reset( struct cbecrypto_ctx *ctx )
{
    if( cbecrypto_cmd_sha1_init( ctx->spe ) ) {
        return CBECRYPTO_ERR_IO;
    }
 
    // Clear the internal buffer
    ctx->sha1->bufLen = 0;

    return CBECRYPTO_SUCCESS;
}

int cbecrypto_sha1_get( struct cbecrypto_ctx *ctx, unsigned char *hash )
{
    int rc = CBECRYPTO_SUCCESS;
    unsigned char buf[ 32 ] __attribute__((aligned(128)));
    struct cbecrypto_sha1_ctx *sha1 = ctx->sha1;
    
    // Process remaining bytes
    sha1->msgLen += sha1->bufLen * 8;
    
    if( sha1->bufLen < 56 ) {
        memset( sha1->buf + sha1->bufLen, 0, 64 - sha1->bufLen );
        sha1->buf[ sha1->bufLen ] = 0x80;
        ((uint64_t *)sha1->buf)[ 7 ] = sha1->msgLen;
        if( cbecrypto_cmd_sha1_update( ctx->spe, sha1->buf, 64 ) ) {
            rc = CBECRYPTO_ERR_IO;
            goto end;
        }
    } else {
        memset( sha1->buf + sha1->bufLen, 0, 128 - sha1->bufLen );
        sha1->buf[ sha1->bufLen ] = 0x80;
        ((uint64_t *)sha1->buf)[ 15 ] = sha1->msgLen;
        if( cbecrypto_cmd_sha1_update( ctx->spe, sha1->buf, 128 ) ) {
            rc = CBECRYPTO_ERR_IO;
            goto end;
        }
    }

    cbecrypto_cmd_sha1_get_state( ctx->spe, buf );
    cbecrypto_sha1_reset( ctx );
    
    memcpy( hash, buf, 20 );
end:

    return rc;
}


int cbecrypto_sha1_destroy( struct cbecrypto_ctx **ctx )
{
    struct cbecrypto_ctx *ctxPtr = *ctx;
    
    destroy_sha1_ctx( &ctxPtr->sha1 );
    cbecrypto_destroy_ctx( ctx );
    
    return CBECRYPTO_SUCCESS;
}
