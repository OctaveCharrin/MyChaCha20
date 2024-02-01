#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chacha20.h"


void printmsg(unsigned char const msg[], int const len){
    for (int i=0; i<len; i+=1){
        printf("%02x ", msg[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[]){
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <keyfilename> <nonce> <inputfilename> <outputfilename>\n", argv[0]);
        return 1;
    }

    // Check nonce length
    char const *nonce = argv[2];
    size_t nl = strlen(nonce);
    if (nl != 24){
        fprintf(stderr, "Nonce must be 24 hexadecimal characters long, not %zu\n", nl);
        return 1;
    }

    // Read key from file
    char const *keyfilename = argv[1];
    FILE *keyfile;
    keyfile = fopen(keyfilename, "rb");
    if (keyfile == NULL) {
        perror("Error opening keyfile");
        return 1;
    }
    char key[32];
    int keyLen = 0;
    char byte = fgetc(keyfile);
    while (byte != EOF){
        key[keyLen] = byte;
        keyLen += 1;
        byte = fgetc(keyfile);
    }
    fclose(keyfile);
    if (keyLen != 32){
        fprintf(stderr, "Key must be 32-byte long, not %d\n", keyLen);
        return 1;
    }

    // Encrypt / Decrypt
    char const *inputfilename = argv[3];
    FILE *inputfile;
    inputfile = fopen(inputfilename, "rb");
    if (inputfile == NULL){
        perror("Error opening inputfile.");
    }
    char const *outputfilename = argv[4];
    FILE *outputfile;
    outputfile = fopen(outputfilename, "wb");
    if (inputfile == NULL){
        perror("Error opening outputfile.");
    }

    unsigned int counter = 1;
    unsigned int *state = malloc(16*sizeof(*state));
    unsigned char msg[64];
    int len = 0;
    byte = fgetc(inputfile);
    while(byte != EOF){
        msg[len] = byte;
        len += 1;
        if (len == 64){
            chacha20_block(state, counter, (unsigned char *)key, nonce);
            XORStateMsg(state, msg);
            fwrite(msg, sizeof(*msg), len, outputfile);
            // printmsg(msg, len);
            len = 0;
            counter += 1;
        }
        byte = fgetc(inputfile);
    }
    // Handle last block
    if (len != 0){
        chacha20_block(state, counter, (unsigned char *)key, nonce);
        XORStateMsg(state, msg);
        // printmsg(msg, len);
        fwrite(msg, sizeof(*msg), len, outputfile);
    }
    return 0;
}