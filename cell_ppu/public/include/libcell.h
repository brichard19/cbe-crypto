#ifndef LIB_CELL_H
#define LIB_CELL_H

#include <libspe2.h>
#include <stdint.h>

struct cell_spe_ctx;
typedef struct cell_spe_ctx *cell_spe_ctx;

int cell_create_spe_ctx( cell_spe_ctx *ctx, spe_program_handle_t *handle );
void cell_destroy_spe_ctx( cell_spe_ctx *ctx );
int cell_wait_spe_ctx( cell_spe_ctx ctx );

int cell_send_int32( cell_spe_ctx ctx, uint32_t msg );
int cell_send_int64( cell_spe_ctx ctx, uint64_t msg );

int cell_recv_int32( cell_spe_ctx ctx, uint32_t *msg );
int cell_recv_int64( cell_spe_ctx ctx, uint64_t *msg );

#endif /*LIB_CELL_H*/
