#ifndef CRYPTOGRAM_H_
#define CRYPTOGRAM_H_

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <openssl/des.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include "utilities.h"
#define KEY_LENGTH  512
#define PUB_EXP     3
#define PRINT_KEYS
#define WRITE_TO_FILE

int convert_size_t_to_int( size_t);
char* encrypt_massage_with_private_key(RSA*,char* ,int* );
char* decrypt_massage_with_public_key(RSA* ,char* ,int );
char* get_public_key(RSA*,int* );
void show_encrypted_massage(char* key,vector<string> com);
void decrypt_client_massage_contain_public_key(char* massage);
//int public_key_decrypt(unsigned char * enc_data,int data_len,unsigned char * key, unsigned char *decrypted);
bool create_RSA_key(char *pubAddr,char *privAddr);
RSA *getPrivateKey(char *privAddr);
RSA *getPublicKey(char *pubAddr);
int private_encrypt(unsigned char * data,int data_len ,char * privateAddr,unsigned char *encrypted);
int public_decrypt(unsigned char * enc_data,int data_len,char *publicAddr,unsigned char *decrypted);
RSA *getPrivateKey(char *privAddr);
RSA *getPublicKey(char *pubAddr);
bool create_certificate(char* privatePath,string ssn,string name);
bool certificate_is_valid(char*);

#endif