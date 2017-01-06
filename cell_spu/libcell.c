#include <spu_mfcio.h>
#include "libcell.h"

uint32_t spe_recv_int32()
{
    return spu_read_in_mbox();
}

uint64_t spe_recv_int64()
{
    uint32_t m1 = spu_read_in_mbox();
    uint32_t m2 = spu_read_in_mbox();
    
    return ( (uint64_t)m1 << 32 ) | m2;
}

void spe_send_int32( uint32_t msg )
{
    spu_write_out_intr_mbox( msg );
}

void spe_send_int64( uint64_t msg )
{
    spu_write_out_intr_mbox( (uint32_t)( msg >> 32 ) );
    spu_write_out_intr_mbox( (uint32_t)msg );
}

uint32_t spe_read( uint64_t ea, void *lsa, uint32_t n )
{
    unsigned int offset = 0;
    uint32_t tag = 0;
    uint32_t tag_mask = 0x01;
    
    while( n > 0 ) {
        uint32_t size = n >= 16384 ? 16384 : n;

        mfc_get( lsa, ea + offset, size, tag, 0, 0 );
        mfc_write_tag_mask( tag_mask );
        mfc_read_tag_status_all();

        offset += size;
        n -= size;
    }
 
    return n;
}

uint32_t spe_write( uint64_t ea, void *lsa, uint32_t n )
{

    unsigned int offset =0;
    uint32_t tag = 0;
    uint32_t tag_mask = 0x01;
    
    
    while( n > 0 ) {
        uint32_t size = n >= 16384 ? 16384 : n;
        
        mfc_put( lsa, ea + offset, size, tag, 0, 0 );
        mfc_write_tag_mask( tag_mask );
        mfc_read_tag_status_all();
        
        offset += size;
        n -= size;
    }

    return n;
}
