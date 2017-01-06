#include <stdlib.h>
#include <string.h>
#include <libspe2.h>
#include <stdint.h>
#include <errno.h>
#include "cell_internal.h"


typedef struct {
	spe_context_ptr_t ctx;
}spe_thread_parameters;


/**
 * Since the SPE program blocks until it is finished running, each SPE process
 * requires its own thread
 */
static void *speHoldingThread( void *p )
{
	unsigned int entry = SPE_DEFAULT_ENTRY;
	spe_stop_info_t stopinfo;
	spe_thread_parameters *params = (spe_thread_parameters *)p;
	int r = 0;
	
	r = spe_context_run( params->ctx, &entry, 0, NULL, NULL, &stopinfo );
	
	printf( "spe_context_run() returned %d; errno: %d: %s\n", r, errno, strerror( errno ) );
	
	return NULL;
}


int cell_create_spe_ctx( struct cell_spe_ctx **ctx, spe_program_handle_t *handle )
{
	spe_thread_parameters *params = NULL;
	struct cell_spe_ctx *tmpCtx = NULL;
	int err = 0;
	
	// Allocate room for context
	tmpCtx = (struct cell_spe_ctx *)malloc( sizeof(*tmpCtx) );
	if( tmpCtx == NULL ) {
		err = ENOMEM;
		goto end;
	}
	
	memset( tmpCtx, 0, sizeof(*tmpCtx) );
	
	// allocate room for thread parameters
	params = (spe_thread_parameters *)malloc( sizeof(spe_thread_parameters) );
	if( params == NULL ) {
	    err = ENOMEM;
	    goto end;
	}
	
	// create SPE context
	tmpCtx->spe = spe_context_create( 0, NULL );
	if( tmpCtx->spe == NULL ) {
		err = errno;
		goto end;
	}
	
	// load SPE program
	if( spe_program_load( tmpCtx->spe, handle ) ) {
		err = errno;
		goto end;
	}
	
	params->ctx = tmpCtx->spe;
	
	// create thread
	err = pthread_create( &tmpCtx->thread, NULL, speHoldingThread, params );
	if( err ) {
		goto end;
	}
	
	*ctx = tmpCtx;
	
end:
	if( err ) {
		if( tmpCtx->spe != NULL ) {
		    spe_context_destroy( tmpCtx->spe );
		}
		free( params );
		free( tmpCtx );
	}
	
	return err;
}

int cell_wait_spe_ctx( struct cell_spe_ctx *ctx )
{
    pthread_join( ctx->thread, NULL );
    
    return 0;
}

void cell_destroy_spe_ctx( struct cell_spe_ctx **ctx )
{
    if( (*ctx)->spe != NULL ) {
        spe_context_destroy( (*ctx)->spe );
    }
    
    free( *ctx );
    *ctx = NULL;
}


int cell_send_int32( struct cell_spe_ctx *ctx, uint32_t msg )
{
    if( spe_in_mbox_write( ctx->spe, &msg, 1, SPE_MBOX_ANY_NONBLOCKING ) ) {
        return errno;
    }
    
    return 0;
}

int cell_recv_int32( struct cell_spe_ctx *ctx, uint32_t *msg )
{    
    if( spe_out_intr_mbox_read( ctx->spe, msg, 1, SPE_MBOX_ALL_BLOCKING ) < 0 ) {
        return errno;
    }
 
    return 0;
}

int cell_recv_int64( struct cell_spe_ctx *ctx, uint64_t *msg )
{
    if( spe_out_intr_mbox_read( ctx->spe, (uint32_t *)msg, 2, SPE_MBOX_ALL_BLOCKING ) < 0 ) {
        return errno;
    }
    
    return 0;
}

int cell_send_int64( struct cell_spe_ctx *ctx, uint64_t msg )
{
    if( spe_in_mbox_write( ctx->spe, (uint32_t *)&msg, 2, SPE_MBOX_ANY_NONBLOCKING ) ) {
        return errno;
    }
    
    return 0;
}
