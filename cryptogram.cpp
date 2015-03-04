#include "cryptogram.h"
/* 
RSA* createRSA(unsigned char *key ,int public_b)
{
    RSA *rsa= NULL;
    BIO *keybio ;
    keybio = BIO_new_mem_buf(key, -1);
    if (keybio==NULL)
    {
        printf( "Failed to create key BIO");
        return 0;
    }
    if(public_b)
    {
        rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa,NULL, NULL);
    }
    else
    {
        rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa,NULL, NULL);
    }
 
    return rsa;
}
*/
bool create_RSA_key(char *pubAddr,char *privAddr)
{
    int             ret = 0;
    RSA             *r = NULL;
    BIGNUM          *bne = NULL;
    BIO             *bp_public = NULL, *bp_private = NULL;

    int             bits = 2048;
    unsigned long   e = RSA_F4;
    bool retVal=true;
    // 1. generate rsa key
    bne = BN_new();              
    ret = BN_set_word(bne,e);  
    if(ret != 1){
        cerr<<"BN" << endl;
        return false;
    }                
    r = RSA_new();  
    ret = RSA_generate_key_ex(r, bits, bne, NULL);                                                                      
    if(ret != 1){    
        cerr<<"RSA generate" << endl;                                                                                                  
        return false;                                                                                                        
    }                                                                                                                
    // 2. save public key
    bp_public = BIO_new_file(pubAddr, "w+");   
    cerr<< "address is: " << pubAddr << endl;
    ret = PEM_write_bio_RSAPublicKey(bp_public, r);                                                            
    if(ret != 1){     
        cerr<< "write public key" << endl;                                                                                        
        return false;                                                                                               
    }
    // 3. save private key 
    bp_private = BIO_new_file(privAddr, "w+");                                                                  
    ret = PEM_write_bio_RSAPrivateKey(bp_private, r, NULL, NULL, 0, NULL, NULL);
    cerr << "write private key" << endl;                           
    retVal=(ret == 1);                                                                                    
    // 4.Free
    BIO_free_all(bp_public);                                                                                      
    BIO_free_all(bp_private);                                                                                    
    RSA_free(r);                                                                                                
    BN_free(bne);                                                                                              
    return retVal;  
} 


int convert_size_t_to_int( size_t what )
{
    if( what > INT_MAX ) {
       return -1;
    }
    return static_cast<int>( what );
}
/*
int public_key_decrypt(unsigned char * enc_data,int data_len,unsigned char * key, unsigned char *decrypted)
{
    cerr << "ecrypting data is:\n" << enc_data << endl;
    cerr << "key is :\n" << key << endl;
    RSA * rsa = createRSA(key,1);
    cerr << "after create RSA" << endl;
    int  result = RSA_public_decrypt(data_len,enc_data,decrypted,rsa,RSA_PKCS1_PADDING);
    cerr << "after decreptino result is: " << result << endl;
    return result;
}
*/
/*
void decrypt_client_massage_contain_public_key(char* massage){
    cerr << "massage is\n" <<  massage << endl;
    vector<string> token = mytokenizer_char(massage,"#");
    cerr << "token[1] is :" << token[1] << endl;
    int massge_size= atoi( token[1].c_str() );
    cerr << "before constructor: massage size is : " << massge_size << endl;
    string encrypt(&massage[9+ numDigits(massge_size)+1],massge_size);
    cerr <<"encrypt massage recieve is :" << encrypt << endl;
    token = mytokenizer_char(&massage[256],"#");
    cerr << "token[0] is :" << token[0] << endl;
    int pub_key_size = atoi( token[0].c_str() );
    string publicKey(&massage[256+ numDigits(pub_key_size)+2],pub_key_size);
    cerr << "public key is:" << publicKey.c_str() << endl;
    char decrypted [MAX_STR_SIZE];
    clear_buff(decrypted, MAX_STR_SIZE);
    if( public_key_decrypt((unsigned char * )&massage[9+ numDigits(massge_size)+1],massge_size,(unsigned char * )&massage[256+ numDigits(pub_key_size)+2],(unsigned char * )decrypted) ==-1)
        cout << "error in decryption" << endl;
    else
        cout << "recieved decrypted massage is: " << decrypted << endl;

}
*/
char* get_public_key(RSA* keypair,int* public_length){
    BIO *pub = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPublicKey(pub, keypair);
    size_t pub_len = BIO_pending(pub);
    *public_length= convert_size_t_to_int(pub_len);    
    char* pub_key = (char*)malloc(pub_len + 1);
    BIO_read(pub, pub_key, pub_len);
    pub_key[pub_len] = '\0';
    BIO_free_all(pub);
    return pub_key;
}

char* encrypt_massage_with_private_key(RSA*keypair,char* msg ,int* encrypt_len){
    char* encrypt = new char[RSA_size(keypair)];
    char*err = new char[130];
    if((*encrypt_len = RSA_private_encrypt(strlen(msg)+1, (unsigned char*)msg, (unsigned char*)encrypt,
                                         keypair, RSA_PKCS1_PADDING)) == -1) {
        ERR_load_crypto_strings();
        ERR_error_string(ERR_get_error(), err);
        fprintf(stderr, "Error encrypting message: %s\n", err);
        return NULL;
    }
    return encrypt;
}

char* decrypt_massage_with_public_key(RSA* keypair,char* encrypted,int encrypt_len){
    char* decrypt =new char[encrypt_len];
    char*err = new char[130];
    if(RSA_public_decrypt(encrypt_len, (unsigned char*)encrypted, (unsigned char*)decrypt,
                           keypair, RSA_PKCS1_OAEP_PADDING) == -1) {
        ERR_load_crypto_strings();
        ERR_error_string(ERR_get_error(), err);
        fprintf(stderr, "Error decrypting message: %s\n", err);
        return NULL;
    }
    return decrypt;
}

/*
int main(void) {
    size_t pri_len;            // Length of private key
    size_t pub_len;            // Length of public key
    char   *pri_key;           // Private key
    char   *pub_key;           // Public key
    char   msg[KEY_LENGTH/8];  // Message to encrypt
    char   *encrypt = NULL;    // Encrypted message
    char   *decrypt = NULL;    // Decrypted message
    char   *err;               // Buffer for any error messages

    // Generate key pair
    printf("Generating RSA (%d bits) keypair...", KEY_LENGTH);
    fflush(stdout);
    RSA *keypair = RSA_generate_key(KEY_LENGTH, PUB_EXP, NULL, NULL);

    // To get the C-string PEM form:
    BIO *pri = BIO_new(BIO_s_mem());
    BIO *pub = BIO_new(BIO_s_mem());

    PEM_write_bio_RSAPrivateKey(pri, keypair, NULL, NULL, 0, NULL, NULL);
    PEM_write_bio_RSAPublicKey(pub, keypair);

    pri_len = BIO_pending(pri);
    pub_len = BIO_pending(pub);

    pri_key = (char*)malloc(pri_len + 1);
    pub_key = (char*)malloc(pub_len + 1);

    BIO_read(pri, pri_key, pri_len);
    BIO_read(pub, pub_key, pub_len);

    pri_key[pri_len] = '\0';
    pub_key[pub_len] = '\0';

    #ifdef PRINT_KEYS
        printf("\n%s\n%s\n", pri_key, pub_key);
    #endif
    printf("done.\n");

    // Get the message to encrypt
    printf("Message to encrypt: ");
    fgets(msg, KEY_LENGTH-1, stdin);
    msg[strlen(msg)-1] = '\0';

    encrypt = (char*)malloc(RSA_size(keypair));
    int encrypt_len;
    err = (char*)malloc(130);
    if((encrypt_len = RSA_public_encrypt(strlen(msg)+1, (unsigned char*)msg, (unsigned char*)encrypt,
                                         keypair, RSA_PKCS1_OAEP_PADDING)) == -1) {
        ERR_load_crypto_strings();
        ERR_error_string(ERR_get_error(), err);
        fprintf(stderr, "Error encrypting message: %s\n", err);
    }

    #ifdef WRITE_TO_FILE
    // Write the encrypted message to a file
        FILE *out = fopen("out.bin", "w");
        fwrite(encrypt, sizeof(*encrypt),  RSA_size(keypair), out);
        fclose(out);
        printf("Encrypted message written to file.\n");
        free(encrypt);
        encrypt = NULL;

        // Read it back
        printf("Reading back encrypted message and attempting decryption...\n");
        encrypt =(char*) malloc(RSA_size(keypair));
        out = fopen("out.bin", "r");
        fread(encrypt, sizeof(*encrypt), RSA_size(keypair), out);
        fclose(out);
    #endif

    // Decrypt it
    decrypt =(char*) malloc(encrypt_len);
    if(RSA_private_decrypt(encrypt_len, (unsigned char*)encrypt, (unsigned char*)decrypt,
                           keypair, RSA_PKCS1_OAEP_PADDING) == -1) {
        ERR_load_crypto_strings();
        ERR_error_string(ERR_get_error(), err);
        fprintf(stderr, "Error decrypting message: %s\n", err);
        // goto free_stuff;
    }
    printf("Decrypted message: %s\n", decrypt);

    // free_stuff:
    RSA_free(keypair);
    BIO_free_all(pub);
    BIO_free_all(pri);
    free(pri_key);
    free(pub_key);
    free(encrypt);
    free(decrypt);
    free(err);

    return 0;
}
*/