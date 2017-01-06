#ifndef SPECMD_H_
#define SPECMD_H_

#include <stdint.h>

#include "ctx.h"

#define CMD_MAX_ARGS 4
enum param_type {
    TYPE_UINT32 = 0,
    TYPE_UINT64
};

struct cbecrypto_cmd_arg {
    int type;
    uint32_t ulValue;
    uint64_t ullValue;
};
int cbecrypto_cmd_exit( struct cbecrypto_spe_ctx *spe );

int cbecrypto_cmd_sha1_init( struct cbecrypto_spe_ctx *spe );
int cbecrypto_cmd_sha1_update( struct cbecrypto_spe_ctx *spe, unsigned char *buf, size_t len );
int cbecrypto_cmd_sha1_get_state( struct cbecrypto_spe_ctx *spe, unsigned char *state );

#endif /*COMMANDS_H_*/
