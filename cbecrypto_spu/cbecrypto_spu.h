#ifndef CBECRYPTO_SPU_H_
#define CBECRYPTO_SPU_H_

#include <stdint.h>
#include "sha1.h"
#include "aes.h"

struct cbecrypto_spu_ctx {
    union {
        struct cbecrypto_sha1_ctx sha1Ctx;
        struct cbecrypto_aes_ctx aesCtx;
    };
};


extern struct cbecrypto_spu_ctx _ctxBuffer;

#define DATA_BUFFER_SIZE 65536
extern unsigned char _dataBuffer[];


#endif /*CBECRYPTO_SPU_H_*/
