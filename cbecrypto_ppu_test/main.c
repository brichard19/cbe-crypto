#include <stdio.h>
#include <string.h>
#include "cbecrypto.h"

int main( int argc, char **argv )
{
    int i = 0;
    int err = 0;
    char msg[] = "The quick brown fox jumps over the lazy dog";
    unsigned char hash[ 20 ] = {0};
    cbecrypto_ctx ctx;
    
    err = cbecrypto_init( 1 );
    if( err != CBECRYPTO_SUCCESS ) {
        printf( "Error: %d\n", err );
    }
    
    err = cbecrypto_sha1_create( &ctx );
    if( err != CBECRYPTO_SUCCESS ) {
        printf( "Error: %d\n", err );
    }
    
    err = cbecrypto_sha1_update( ctx, (unsigned char *)msg, strlen( msg ) );
    if( err != CBECRYPTO_SUCCESS ) {
        printf( "Error: %d\n", err );
    }
   
    err = cbecrypto_sha1_update( ctx, (unsigned char *)msg, strlen( msg ) );
    if( err != CBECRYPTO_SUCCESS ) {
        printf( "Error: %d\n", err );
    }
    
    err = cbecrypto_sha1_get( ctx, hash );
    if( err != CBECRYPTO_SUCCESS ) {
        printf( "Error: %d\n", err );
    }
    
    err = cbecrypto_sha1_destroy( &ctx );
    if( err != CBECRYPTO_SUCCESS ) {
        printf( "Error: %d\n", err );
    }
    
    for( i = 0; i < 20; i++ ) {
        printf( "%.2x", hash[ i ] );
    }
    printf( "\n" );
    
    cbecrypto_cleanup();
    
    return 0;
}
