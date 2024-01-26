#include "chacha20.h"

unsigned int RotL(unsigned int const n, int const d){
    int l = 8 * sizeof(n);
    int s = d % (8 * sizeof(n));
    return (n << s) | (n >> (l - s));
}

void _quarterRound(unsigned int *a, unsigned int *b, unsigned int *c, unsigned int *d){
    *a += *b; *d ^= *a; *d = RotL(*d, 16);
    *c += *d; *b ^= *c; *b = RotL(*b, 12);
    *a += *b; *d ^= *a; *d = RotL(*d, 8);
    *c += *d; *b ^= *c; *b = RotL(*b, 7);
}



void QuarterRound(unsigned int *S, int const idx_a, int const idx_b, int const idx_c, int const idx_d){
}