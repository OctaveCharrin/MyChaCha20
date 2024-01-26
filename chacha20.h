// chacha20.h

#pragma once

unsigned int RotL(unsigned int const n, int const d);
void _quarterRound(unsigned int *a, unsigned int *b, unsigned int *c, unsigned int *d);
void QuarterRound(unsigned int *S, int const idx1, int const idx2, int const idx3, int const idx4);