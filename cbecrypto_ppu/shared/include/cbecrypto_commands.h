#ifndef CBECRYPTO_COMMANDS_H_
#define CBECRYPTO_COMMANDS_H_


enum CBECRYPTO_COMMAND {

    CMD_EXIT = 1,
    
    CMD_SHA1_INIT = 100,
    CMD_SHA1_UPDATE,
    CMD_SHA1_GET_STATE,
    
    CMD_AES_SET_KEY = 200,
    CMD_AES_SET_MODE,
};

enum CBECRYPTO_STATUS {
    SUCCESS = 0,
    ERROR_INVALID_LENGTH,
};

#endif /*COMMANDS_H_*/
