#ifndef CTX_H_
#define CTX_H_

#include <pthread.h>
#include "libcell.h"

#include "sha1.h"

#define SPE_STATUS_AVAILABLE 0
#define SPE_STATUS_IN_USE 1

#define CBECRYPTO_MAX_SPES 6

struct cbecrypto_spe_ctx {
    cell_spe_ctx spe;
    int status;
};

struct cbecrypto_global_ctx {
    pthread_mutex_t mutex;
    struct cbecrypto_spe_ctx spe[ CBECRYPTO_MAX_SPES ];
    int numSpes;
};


extern struct cbecrypto_global_ctx _globalCtx;

struct cbecrypto_ctx {
    int state;
    struct cbecrypto_spe_ctx *spe;
    
    union {
        struct cbecrypto_sha1_ctx *sha1;
    };
};

int cbecrypto_create_ctx( struct cbecrypto_ctx **ctx );
int cbecrypto_destroy_ctx( struct cbecrypto_ctx **ctx );

#endif /*CTX_H_*/
