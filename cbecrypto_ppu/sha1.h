#ifndef CBECRYPTO_SHA1_H_
#define CBECRYPTO_SHA1_H_

#include <stddef.h>
#include <stdint.h>

struct cbecrypto_sha1_ctx {
    unsigned char *buf;
    size_t bufLen;
    uint64_t msgLen;
};

#endif /*CBECRYPTO_SHA1_H_*/
