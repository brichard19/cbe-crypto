#ifndef LIB_CELL_INTERNAL_H_
#define LIB_CELL_INTERNAL_H_

#include <libspe2.h>
#include <pthread.h>

/**
 * Structure to hold an SPE context and thread. We need the thread
 * because spe_context_run() blocks until the SPE process is finished
 */
struct cell_spe_ctx {
    spe_context_ptr_t spe;
    pthread_t thread;
};

#endif /*LIB_CELL_INTERNAL_H_*/
