#ifndef SPU_MACROS_H_
#define SPU_MACROS_H_

#include <spu_intrinsics.h>

/**
 * Unpacks 4 32-bit elements from a vector until the first element
 * of four vectors
 */
#define int32_unpack( d1, d2, d3, d4, s ) {(d1) = (s); (d2) = spu_slqwbyte( (s), 4 ); (d3) = spu_slqwbyte( (s), 8 ); (d4) = spu_slqwbyte( (s), 12 );}

#endif /*SPU_MACROS_H_*/
