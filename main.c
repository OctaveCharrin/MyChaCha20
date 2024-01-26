#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chacha20.h"

void printBits(unsigned int num) {
    int bit;

    // Size of an unsigned int in bits
    int size = sizeof(unsigned int) * 8;

    // Iterate through each bit in the integer
    for (int i = size - 1; i >= 0; i--) {
        // Extract the i-th bit using bitwise AND
        bit = (num >> i) & 1;

        // Print the bit
        printf("%d", bit);

        // Add a space for better readability
        if (i % 8 == 0) {
            printf(" ");
        }
    }

    printf("\n");
}

int main(int argc, char *argv[]){
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <keyfilename> <nonce> <inputfilename> <outputfilename>\n", argv[0]);
        return 1;
    }

    char const *inputfilename = argv[3];
    char const *outputfilename = argv[4];

    // Check nonce length
    char const *nonce = argv[2];
    size_t nl = strlen(nonce);
    if (nl != 24){
        fprintf(stderr, "Nonce must be 24 hexadecimal characters long, not %zu\n", nl);
        return 1;
    }

    // Read key
    char const *keyfilename = argv[1];
    FILE *keyfile;
    keyfile = fopen(keyfilename, "rb");
    if (keyfile == NULL) {
        perror("Error opening file");
        return 1;
    }
    char key[64];
    int keyLen = 0;
    char byte = fgetc(keyfile);
    while (byte != EOF){
        printf("%02x ", byte);
        key[keyLen] = byte;
        keyLen += 1;
        byte = fgetc(keyfile);
        
    }
    fclose(keyfile);
    if (keyLen != 64){
        fprintf(stderr, "Key must be 64-byte long, not %d\n", keyLen);
        return 1;
    }

    unsigned int a, b, c, d; 
    a = 0x11111111;
    b = 0x01020304;
    c = 0x9b8d6f43;
    d = 0x01234567;

    printf("a = 0x%x\nb = 0x%x\nc = 0x%x\nd = 0x%x\n", a, b, c, d);

    quarterRound(&a, &b, &c, &d);
    printf("then\n");
    printf("a = 0x%x\nb = 0x%x\nc = 0x%x\nd = 0x%x\n", a, b, c, d);

    unsigned int state[16];

    return 0;
}