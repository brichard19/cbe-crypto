#ifndef SHA1_H_
#define SHA1_H_

struct cbecrypto_sha1_ctx {
    vector unsigned int state[ 8 ];
};

void sha1_init();
void sha1_update();
void sha1_get_state();

#endif /*SHA1_H_*/
