#pragma once

#include <string>

#include "emmintrin.h"

std::string HandToString(int* cards);
std::string IndexToString(int index);

// FastMod from https://stackoverflow.com/a/33333636/1619888
// Apply the modulo operator only when needed
// (i.e. when the input is greater than the ceiling).
int FastMod(const uint32_t input, const int ceil);

__declspec(align(16)) static __m128i cur_seed;

void srand_sse(unsigned int seed);
void rand_sse(unsigned int* result);

int FastRand();