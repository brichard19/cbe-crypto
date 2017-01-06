#include <stdint.h>

#include "cbecrypto_commands.h"
#include "spu_macros.h"
#include "cbecrypto_spu.h"
#include "sha1.h"
#include "libcell.h"

static struct cbecrypto_sha1_ctx *_ctx = &_ctxBuffer.sha1Ctx;

static const vector unsigned int SHA1_IV0 = { 0x67452301 };
static const vector unsigned int SHA1_IV1 = { 0xEFCDAB89 };
static const vector unsigned int SHA1_IV2 = { 0x98BADCFE };
static const vector unsigned int SHA1_IV3 = { 0x10325476 };
static const vector unsigned int SHA1_IV4 = { 0xC3D2E1F0 };

static const vector unsigned int K1 = { 0x5a827999 };
static const vector unsigned int K2 = { 0x6ed9eba1 };
static const vector unsigned int K3 = { 0x8f1bbcdc };
static const vector unsigned int K4 = { 0xca62c1d6 };

#define rotl( x, n ) spu_rl( (x), (n) )

#define A(x,y,z) spu_sel( (z), (y), (x) )
#define B(x,y,z)( (x) ^ (y) ^ (z) )
#define C(x,y,z)( ((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define D(x,y,z)( (x) ^ (y) ^ (z) )

#define F0(a,b,c,d,e,x){ (e) += A( (b), (c), (d) ) + rotl((a),5) + (x) + K1; (b) = rotl((b),30); }
#define F1(a,b,c,d,e,x){ (e) += B( (b), (c), (d) ) + rotl((a),5) + (x) + K2; (b) = rotl((b),30); }
#define F2(a,b,c,d,e,x){ (e) += C( (b), (c), (d) ) + rotl((a),5) + (x) + K3; (b) = rotl((b),30); }
#define F3(a,b,c,d,e,x){ (e) += D( (b), (c), (d) ) + rotl((a),5) + (x) + K4; (b) = rotl((b),30); }

#define W(t,a,b,c,d) (t) = rotl( ( (a) ^ (b) ^ (c) ^ (d) ), 1 )


static void sha1_compress( vector unsigned int *state, vector unsigned int *m )
{
    vector unsigned int a, b, c, d, e;
    vector unsigned int w[ 16 ];
    
    a = state[ 0 ];
    b = state[ 1 ];
    c = state[ 2 ];
    d = state[ 3 ];
    e = state[ 4 ];

    // Unpack
    int32_unpack( w[ 0 ], w[ 1 ], w[ 2 ], w[ 3 ], m[ 0 ] );
    int32_unpack( w[ 4 ], w[ 5 ], w[ 6 ], w[ 7 ], m[ 1 ] );
    int32_unpack( w[ 8 ], w[ 9 ], w[ 10 ], w[ 11 ], m[ 2 ] );
    int32_unpack( w[ 12 ], w[ 13 ], w[ 14 ], w[ 15 ], m[ 3 ] );
    
    F0( a, b, c, d, e, w[0] );
    F0( e, a, b, c, d, w[1] );
    F0( d, e, a, b, c, w[2] );
    F0( c, d, e, a, b, w[3] );
    F0( b, c, d, e, a, w[4] );
    F0( a, b, c, d, e, w[5] );
    F0( e, a, b, c, d, w[6] );
    F0( d, e, a, b, c, w[7] );
    F0( c, d, e, a, b, w[8] );
    F0( b, c, d, e, a, w[9] );
    F0( a, b, c, d, e, w[10] );
    F0( e, a, b, c, d, w[11] );
    F0( d, e, a, b, c, w[12] );
    F0( c, d, e, a, b, w[13] );
    F0( b, c, d, e, a, w[14] );
    F0( a, b, c, d, e, w[15] );

    W( w[ 0 ], w[ 13 ], w[ 8 ], w[ 2 ], w[ 0 ] );
    W( w[ 1 ], w[ 14 ], w[ 9 ], w[ 3 ], w[ 1 ] );
    W( w[ 2 ], w[ 15 ], w[ 10 ], w[ 4 ], w[ 2 ] );
    W( w[ 3 ], w[ 0 ], w[ 11 ], w[ 5 ], w[ 3 ] );
    W( w[ 4 ], w[ 1 ], w[ 12 ], w[ 6 ], w[ 4 ] );
    W( w[ 5 ], w[ 2 ], w[ 13 ], w[ 7 ], w[ 5 ] );
    W( w[ 6 ], w[ 3 ], w[ 14 ], w[ 8 ], w[ 6 ] );
    W( w[ 7 ], w[ 4 ], w[ 15 ], w[ 9 ], w[ 7 ] );
    W( w[ 8 ], w[ 5 ], w[ 0 ], w[ 10 ], w[ 8 ] );
    W( w[ 9 ], w[ 6 ], w[ 1 ], w[ 11 ], w[ 9 ] );
    W( w[ 10 ], w[ 7 ], w[ 2 ], w[ 12 ], w[ 10 ] );
    W( w[ 11 ], w[ 8 ], w[ 3 ], w[ 13 ], w[ 11 ] );
    W( w[ 12 ], w[ 9 ], w[ 4 ], w[ 14 ], w[ 12 ] );
    W( w[ 13 ], w[ 10 ], w[ 5 ], w[ 15 ], w[ 13 ] );
    W( w[ 14 ], w[ 11 ], w[ 6 ], w[ 0 ], w[ 14 ] );
    W( w[ 15 ], w[ 12 ], w[ 7 ], w[ 1 ], w[ 15 ] );

    F0( e, a, b, c, d, w[0] );
    F0( d, e, a, b, c, w[1] );
    F0( c, d, e, a, b, w[2] );
    F0( b, c, d, e, a, w[3] );
    F1( a, b, c, d, e, w[4] );
    F1( e, a, b, c, d, w[5] );
    F1( d, e, a, b, c, w[6] );
    F1( c, d, e, a, b, w[7] );
    F1( b, c, d, e, a, w[8] );
    F1( a, b, c, d, e, w[9] );
    F1( e, a, b, c, d, w[10] );
    F1( d, e, a, b, c, w[11] );
    F1( c, d, e, a, b, w[12] );
    F1( b, c, d, e, a, w[13] );
    F1( a, b, c, d, e, w[14] );
    F1( e, a, b, c, d, w[15] );

    W( w[ 0 ], w[ 13 ], w[ 8 ], w[ 2 ], w[ 0 ] );
    W( w[ 1 ], w[ 14 ], w[ 9 ], w[ 3 ], w[ 1 ] );
    W( w[ 2 ], w[ 15 ], w[ 10 ], w[ 4 ], w[ 2 ] );
    W( w[ 3 ], w[ 0 ], w[ 11 ], w[ 5 ], w[ 3 ] );
    W( w[ 4 ], w[ 1 ], w[ 12 ], w[ 6 ], w[ 4 ] );
    W( w[ 5 ], w[ 2 ], w[ 13 ], w[ 7 ], w[ 5 ] );
    W( w[ 6 ], w[ 3 ], w[ 14 ], w[ 8 ], w[ 6 ] );
    W( w[ 7 ], w[ 4 ], w[ 15 ], w[ 9 ], w[ 7 ] );
    W( w[ 8 ], w[ 5 ], w[ 0 ], w[ 10 ], w[ 8 ] );
    W( w[ 9 ], w[ 6 ], w[ 1 ], w[ 11 ], w[ 9 ] );
    W( w[ 10 ], w[ 7 ], w[ 2 ], w[ 12 ], w[ 10 ] );
    W( w[ 11 ], w[ 8 ], w[ 3 ], w[ 13 ], w[ 11 ] );
    W( w[ 12 ], w[ 9 ], w[ 4 ], w[ 14 ], w[ 12 ] );
    W( w[ 13 ], w[ 10 ], w[ 5 ], w[ 15 ], w[ 13 ] );
    W( w[ 14 ], w[ 11 ], w[ 6 ], w[ 0 ], w[ 14 ] );
    W( w[ 15 ], w[ 12 ], w[ 7 ], w[ 1 ], w[ 15 ] );

    F1( d, e, a, b, c, w[0] );
    F1( c, d, e, a, b, w[1] );
    F1( b, c, d, e, a, w[2] );
    F1( a, b, c, d, e, w[3] );
    F1( e, a, b, c, d, w[4] );
    F1( d, e, a, b, c, w[5] );
    F1( c, d, e, a, b, w[6] );
    F1( b, c, d, e, a, w[7] );
    F2( a, b, c, d, e, w[8] );
    F2( e, a, b, c, d, w[9] );
    F2( d, e, a, b, c, w[10] );
    F2( c, d, e, a, b, w[11] );
    F2( b, c, d, e, a, w[12] );
    F2( a, b, c, d, e, w[13] );
    F2( e, a, b, c, d, w[14] );
    F2( d, e, a, b, c, w[15] );

    W( w[ 0 ], w[ 13 ], w[ 8 ], w[ 2 ], w[ 0 ] );
    W( w[ 1 ], w[ 14 ], w[ 9 ], w[ 3 ], w[ 1 ] );
    W( w[ 2 ], w[ 15 ], w[ 10 ], w[ 4 ], w[ 2 ] );
    W( w[ 3 ], w[ 0 ], w[ 11 ], w[ 5 ], w[ 3 ] );
    W( w[ 4 ], w[ 1 ], w[ 12 ], w[ 6 ], w[ 4 ] );
    W( w[ 5 ], w[ 2 ], w[ 13 ], w[ 7 ], w[ 5 ] );
    W( w[ 6 ], w[ 3 ], w[ 14 ], w[ 8 ], w[ 6 ] );
    W( w[ 7 ], w[ 4 ], w[ 15 ], w[ 9 ], w[ 7 ] );
    W( w[ 8 ], w[ 5 ], w[ 0 ], w[ 10 ], w[ 8 ] );
    W( w[ 9 ], w[ 6 ], w[ 1 ], w[ 11 ], w[ 9 ] );
    W( w[ 10 ], w[ 7 ], w[ 2 ], w[ 12 ], w[ 10 ] );
    W( w[ 11 ], w[ 8 ], w[ 3 ], w[ 13 ], w[ 11 ] );
    W( w[ 12 ], w[ 9 ], w[ 4 ], w[ 14 ], w[ 12 ] );
    W( w[ 13 ], w[ 10 ], w[ 5 ], w[ 15 ], w[ 13 ] );
    W( w[ 14 ], w[ 11 ], w[ 6 ], w[ 0 ], w[ 14 ] );
    W( w[ 15 ], w[ 12 ], w[ 7 ], w[ 1 ], w[ 15 ] );

    F2( c, d, e, a, b, w[0] );
    F2( b, c, d, e, a, w[1] );
    F2( a, b, c, d, e, w[2] );
    F2( e, a, b, c, d, w[3] );
    F2( d, e, a, b, c, w[4] );
    F2( c, d, e, a, b, w[5] );
    F2( b, c, d, e, a, w[6] );
    F2( a, b, c, d, e, w[7] );
    F2( e, a, b, c, d, w[8] );
    F2( d, e, a, b, c, w[9] );
    F2( c, d, e, a, b, w[10] );
    F2( b, c, d, e, a, w[11] );
    F3( a, b, c, d, e, w[12] );
    F3( e, a, b, c, d, w[13] );
    F3( d, e, a, b, c, w[14] );
    F3( c, d, e, a, b, w[15] );

    W( w[ 0 ], w[ 13 ], w[ 8 ], w[ 2 ], w[ 0 ] );
    W( w[ 1 ], w[ 14 ], w[ 9 ], w[ 3 ], w[ 1 ] );
    W( w[ 2 ], w[ 15 ], w[ 10 ], w[ 4 ], w[ 2 ] );
    W( w[ 3 ], w[ 0 ], w[ 11 ], w[ 5 ], w[ 3 ] );
    W( w[ 4 ], w[ 1 ], w[ 12 ], w[ 6 ], w[ 4 ] );
    W( w[ 5 ], w[ 2 ], w[ 13 ], w[ 7 ], w[ 5 ] );
    W( w[ 6 ], w[ 3 ], w[ 14 ], w[ 8 ], w[ 6 ] );
    W( w[ 7 ], w[ 4 ], w[ 15 ], w[ 9 ], w[ 7 ] );
    W( w[ 8 ], w[ 5 ], w[ 0 ], w[ 10 ], w[ 8 ] );
    W( w[ 9 ], w[ 6 ], w[ 1 ], w[ 11 ], w[ 9 ] );
    W( w[ 10 ], w[ 7 ], w[ 2 ], w[ 12 ], w[ 10 ] );
    W( w[ 11 ], w[ 8 ], w[ 3 ], w[ 13 ], w[ 11 ] );
    W( w[ 12 ], w[ 9 ], w[ 4 ], w[ 14 ], w[ 12 ] );
    W( w[ 13 ], w[ 10 ], w[ 5 ], w[ 15 ], w[ 13 ] );
    W( w[ 14 ], w[ 11 ], w[ 6 ], w[ 0 ], w[ 14 ] );
    W( w[ 15 ], w[ 12 ], w[ 7 ], w[ 1 ], w[ 15 ] );

    F3( b, c, d, e, a, w[0] );
    F3( a, b, c, d, e, w[1] );
    F3( e, a, b, c, d, w[2] );
    F3( d, e, a, b, c, w[3] );
    F3( c, d, e, a, b, w[4] );
    F3( b, c, d, e, a, w[5] );
    F3( a, b, c, d, e, w[6] );
    F3( e, a, b, c, d, w[7] );
    F3( d, e, a, b, c, w[8] );
    F3( c, d, e, a, b, w[9] );
    F3( b, c, d, e, a, w[10] );
    F3( a, b, c, d, e, w[11] );
    F3( e, a, b, c, d, w[12] );
    F3( d, e, a, b, c, w[13] );
    F3( c, d, e, a, b, w[14] );
    F3( b, c, d, e, a, w[15] );

    /*Add the old state to the new one*/
    state[ 0 ] += a;
    state[ 1 ] += b;
    state[ 2 ] += c;
    state[ 3 ] += d;
    state[ 4 ] += e;
}

void sha1_init()
{
    _ctx->state[ 0 ] = SHA1_IV0;
    _ctx->state[ 1 ] = SHA1_IV1;
    _ctx->state[ 2 ] = SHA1_IV2;
    _ctx->state[ 3 ] = SHA1_IV3;
    _ctx->state[ 4 ] = SHA1_IV4;
}

void sha1_update()
{    
    // Get the address and data length
    uint64_t ea = spe_recv_int64();
    uint32_t size = spe_recv_int32();
    
    uint32_t totalBytesRead = 0;
    
    // Only accept multiples of 64 bytes
    if( size == 0 || size % 64 != 0 ) {
        spe_send_int32( ERROR_INVALID_LENGTH );
        return;
    }
    
    while( size > 0 ) {
        uint32_t bytesRead = size < DATA_BUFFER_SIZE ? size : DATA_BUFFER_SIZE;
        spe_read( ea + totalBytesRead, _dataBuffer, bytesRead );
                
        totalBytesRead += bytesRead;
        size -= bytesRead;
        
        while( bytesRead >= 64 ) {
            sha1_compress( _ctx->state, (vector unsigned int *)_dataBuffer );
            bytesRead -= 64;
        }
    }
    
    spe_send_int32( SUCCESS );
}

void sha1_get_state()
{
    uint64_t ea = spe_recv_int64();
    vector unsigned int state[ 2 ];
   
    unsigned int *p = (unsigned int *)state;
    p[ 0 ] = spu_extract( _ctx->state[ 0 ], 0 );
    p[ 1 ] = spu_extract( _ctx->state[ 1 ], 0 );
    p[ 2 ] = spu_extract( _ctx->state[ 2 ], 0 );
    p[ 3 ] = spu_extract( _ctx->state[ 3 ], 0 );
    p[ 4 ] = spu_extract( _ctx->state[ 4 ], 0 );
    
    spe_write( ea, state, 32 );
    spe_send_int32( SUCCESS );
}
