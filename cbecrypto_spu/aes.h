#ifndef AES_H_
#define AES_H_

struct cbecrypto_aes_ctx {
    int rounds;
    vector unsigned int subkeys[ 14 ];
};
#endif /*AES_H_*/


