// chacha20.h

#pragma once

void _quarterRound(unsigned int *a, unsigned int *b, unsigned int *c, unsigned int *d);

void QuarterRound(unsigned int *S, int const idx1, int const idx2, int const idx3, int const idx4);

void initState(unsigned int *S, unsigned int const counter, unsigned char const key[], char const *nonce);

void chacha20_block(unsigned int *state, unsigned const int counter, unsigned const char key[], char const  *nonce);

void XORStateMsg(unsigned int const *S, unsigned char msg[]);