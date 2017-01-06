#ifndef CBECRYPTO_H_
#define CBECRYPTO_H_

#include <stddef.h>
#include "cbecrypto_error.h"

int cbecrypto_init( int spus );
int cbecrypto_cleanup();

struct cbecrypto_ctx;
typedef struct cbecrypto_ctx *cbecrypto_ctx;

/**
 * Creates a SHA-1 context
 */
int cbecrypto_sha1_create( cbecrypto_ctx *ctx );

/**
 * Destroys a SHA-1 context
 */
int cbecrypto_sha1_destroy( cbecrypto_ctx *ctx );

/**
 * Updates a SHA-1 context with the given data
 */
int cbecrypto_sha1_update( cbecrypto_ctx ctx, const unsigned char *data, size_t len );

/**
 * Completes the SHA-1 digest operation and writes the digest to the given
 * buffer. The SHA-1 context is also reset to its initial state.
 */
int cbecrypto_sha1_get( cbecrypto_ctx ctx, unsigned char *hash );

/**
 * Resets the SHA-1 context to its initial state
 */
int cbecrypto_sha1_reset( cbecrypto_ctx ctx );

#endif /*CBECRYPTO_H_*/
