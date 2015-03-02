#ifndef CRYPTOGRAM_H_
#define CRYPTOGRAM_H_

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <openssl/des.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

#define KEY_LENGTH  512
#define PUB_EXP     3
#define PRINT_KEYS
#define WRITE_TO_FILE

RSA* create_RSA_key();
char* encrypt_massage_with_private_key(RSA*keypair,char* msg ,int* encrypt_len);
char* decrypt_massage_with_public_key(RSA* keypair,char* encrypted,int encrypt_len);

#endif