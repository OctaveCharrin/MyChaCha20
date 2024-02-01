#include <stdio.h>
#include <stdlib.h>

#include "chacha20.h"


void printState(unsigned int *S){
    for (int i=0; i<16; i+=1){
        if ( i%4 == 0)
            printf("\n");
        printf("0x%08x ", S[i]);
    }
    printf("\n");
}


char hexToByte(char const hexDigit) {
    if (hexDigit >= '0' && hexDigit <= '9') {
        return hexDigit - '0';
    } else if (hexDigit >= 'a' && hexDigit <= 'f') {
        return hexDigit - 'a' + 10;
    } else if (hexDigit >= 'A' && hexDigit <= 'F') {
        return hexDigit - 'A' + 10;
    } else {
        fprintf(stderr, "Invalid hexadecimal digit in key: %c\n", hexDigit);
        return -1;
    }
}


unsigned int LeByteToUInt32(unsigned char const *p){
    return ((unsigned int)p[0]) | ((unsigned int)p[1])<<8 | ((unsigned int)p[2])<<16 | ((unsigned int)p[3])<<24;
}

void UInt32ToLeByte(unsigned int const n, unsigned char *p){
    unsigned int mask = 0xff;
    for (int i=0; i<4; i+=1){
        p[i] = (unsigned char)(n>>(8*i) & mask);
    }
}

unsigned int RotL(unsigned int const n, int const d){
    int l = 8 * sizeof(n);
    int s = d % (8 * sizeof(n));
    return (n << s) | (n >> (l - s));
}


void initState(unsigned int *S, unsigned int const counter, unsigned char const key[], char const *nonce){
    S[0] = 0x61707865;
    S[1] = 0x3320646e;
    S[2] = 0x79622d32;
    S[3] = 0x6b206574;

    for (int i=0; i<8; i+=1){
        S[4+i] = LeByteToUInt32(key+i*4);
    }

    S[12] = counter;
    
    unsigned char tmp_nonce[12];
    for (int i=0; i<12; i+=1){
        tmp_nonce[i] = (unsigned char)((hexToByte(nonce[2*i]) << 4) | hexToByte(nonce[2*i+1]));
    }

    S[13] = LeByteToUInt32(tmp_nonce);
    S[14] = LeByteToUInt32(tmp_nonce + 4);
    S[15] = LeByteToUInt32(tmp_nonce + 8);

    // printState(S);
}


void _quarterRound(unsigned int *a, unsigned int *b, unsigned int *c, unsigned int *d){
    *a += *b; *d ^= *a; *d = RotL(*d, 16);
    *c += *d; *b ^= *c; *b = RotL(*b, 12);
    *a += *b; *d ^= *a; *d = RotL(*d, 8);
    *c += *d; *b ^= *c; *b = RotL(*b, 7);
}


void QuarterRound(unsigned int *S, int const idx_a, int const idx_b, int const idx_c, int const idx_d){
    _quarterRound( S + idx_a, S + idx_b, S + idx_c, S + idx_d );
}

void innerBlock(unsigned int *S){
    QuarterRound(S, 0, 4,  8, 12);
    QuarterRound(S, 1, 5,  9, 13);
    QuarterRound(S, 2, 6, 10, 14);
    QuarterRound(S, 3, 7, 11, 15);
    QuarterRound(S, 0, 5, 10, 15);
    QuarterRound(S, 1, 6, 11, 12);
    QuarterRound(S, 2, 7,  8, 13);
    QuarterRound(S, 3, 4,  9, 14);
}


void chacha20_block(unsigned int *S, unsigned const int counter, unsigned const char key[], char const  *nonce){
    unsigned int intial_state[16];
    initState(intial_state, counter, key, nonce);
    initState(S, counter, key, nonce);
    for (int i=0; i<10; i+=1){
        innerBlock(S);
    }
    for (int i=0; i<16; i+=1){
        S[i] += intial_state[i];
    }
}


void XORStateMsg(unsigned int const *S, unsigned char msg[]){
    unsigned char *p = malloc(4*sizeof(*p));
    for (int i=0; i<16; i+=1){
        UInt32ToLeByte(S[i], p);
        msg[4*i]   ^= p[0];
        msg[4*i+1] ^= p[1];
        msg[4*i+2] ^= p[2];
        msg[4*i+3] ^= p[3];
    }
    free(p);
}
