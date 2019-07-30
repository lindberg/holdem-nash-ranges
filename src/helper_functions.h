#pragma once

#include <string>

// FastMod from https://stackoverflow.com/a/33333636/1619888
// Apply the modulo operator only when needed
// (i.e. when the input is greater than the ceiling).
int FastMod(const int input, const int ceil);

std::string HandToString(int* cards);
std::string IndexToString(int index);