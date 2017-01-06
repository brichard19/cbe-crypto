#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include "cbecrypto_error.h"
#include "ctx.h"
#include "specmd.h"

// Declared in cbecrypto_internal.h
struct cbecrypto_global_ctx _globalCtx;

extern spe_program_handle_t libcbecrypto_spu;

/**
 * Gets the next available SPE
 */
static int get_available_spe( struct cbecrypto_spe_ctx **spe )
{
    int rc = CBECRYPTO_ERR_NO_SPES;
    int i = 0;
    struct cbecrypto_spe_ctx *ctxPtr = NULL;
    
    printf( "get_available_spe()\n" );
    pthread_mutex_lock( &_globalCtx.mutex );
    
    // Find available SPE
    printf( "There are %d spes\n", _globalCtx.numSpes );
    for( i = 0; i < _globalCtx.numSpes; i++ ) {
        printf( "SPE %d status: %d\n", i, _globalCtx.spe[ i ].status );
        if( _globalCtx.spe[ i ].status == SPE_STATUS_AVAILABLE ) {
            ctxPtr = &(_globalCtx.spe[ i ]);
            ctxPtr->status = SPE_STATUS_IN_USE;
            rc = CBECRYPTO_SUCCESS;
            break;
        }
    }
    
    pthread_mutex_unlock( &_globalCtx.mutex );
    
    *spe = ctxPtr;
    
    return rc;
}

static void release_spe( struct cbecrypto_spe_ctx **spe )
{
    printf( "release_spe()\n" );
    if( spe == NULL || *spe == NULL ) {
        return;
    }
    
    pthread_mutex_lock( &_globalCtx.mutex );
    
    (*spe)->status = SPE_STATUS_AVAILABLE;
    
    pthread_mutex_unlock( &_globalCtx.mutex );
    
    *spe = NULL;
}

int cbecrypto_init( int maxSpes )
{
    int i = 0;

    if( maxSpes > CBECRYPTO_MAX_SPES ) {
        return -1;
    }

    memset( &_globalCtx, 0, sizeof(_globalCtx ) );
    _globalCtx.numSpes = maxSpes;

    printf( "Creating SPEs\n" );
    for( i = 0; i < maxSpes; i++ ) {
        int err = cell_create_spe_ctx( &_globalCtx.spe[ i ].spe, &libcbecrypto_spu );
        if( err != 0 ) {
            printf( "Error creating SPE context: %d\n", err );
        }
        _globalCtx.spe[ i ].status = SPE_STATUS_AVAILABLE;
    }

    return CBECRYPTO_SUCCESS;
}

int cbecrypto_cleanup()
{
    int i = 0;
    
    for( i = 0; i < _globalCtx.numSpes; i++ ) {
        cbecrypto_cmd_exit( &_globalCtx.spe[ i ] );
        
    }
    
    return CBECRYPTO_SUCCESS;
}

int cbecrypto_create_ctx( struct cbecrypto_ctx **ctx )
{
    int rc = CBECRYPTO_SUCCESS;
    
    struct cbecrypto_ctx *tmpCtx = NULL;
    printf( "cbecrypto_create_ctx()\n" );
    tmpCtx = (struct cbecrypto_ctx *)malloc( sizeof(struct cbecrypto_ctx) );
    
    if( tmpCtx == NULL ) {
        return CBECRYPTO_ERR_FAIL_ALLOC;
    }
    memset( tmpCtx, 0, sizeof(struct cbecrypto_ctx) );
    
    rc = get_available_spe( &tmpCtx->spe );
    if( rc != CBECRYPTO_SUCCESS ) {
        goto end;
    }

    *ctx = tmpCtx;

end:
    if( rc != CBECRYPTO_SUCCESS ) {
        printf( "cbecrypto_create_ctx(): %d\n", rc );
        cbecrypto_destroy_ctx( &tmpCtx );
    }

    return rc;
}

int cbecrypto_destroy_ctx( struct cbecrypto_ctx **ctx )
{
    printf( "cbecrypto_destroy_ctx()\n" );
    if( ctx == NULL || *ctx == NULL ) {
        return CBECRYPTO_SUCCESS;
    }

    if( (*ctx)->spe != NULL ) {
        release_spe( &(*ctx)->spe );
        (*ctx)->spe = NULL;
    }
    
    free( *ctx );
    *ctx = NULL;
    
    return CBECRYPTO_SUCCESS;
}
