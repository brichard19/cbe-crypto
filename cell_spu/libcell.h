#ifndef LIBCELL_H_
#define LIBCELL_H_

uint32_t spe_recv_int32();
uint64_t spe_recv_int64();
void spe_send_int32( uint32_t msg );
void spe_send_int64( uint64_t msg );
uint32_t spe_read( uint64_t ea, void *lsa, uint32_t n );
uint32_t spe_write( uint64_t ea, void *lsa, uint32_t n );

#endif /*LIBCELL_H_*/
