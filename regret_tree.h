#pragma once

const int kNodes = 676;

// FastMod from https://stackoverflow.com/a/33333636/1619888
int FastMod(const int input, const int ceil);

// Returns the index of regret_tree where the node we are searching for is.
// c1 and c2 are the cards represented by integers 1-52. For more information,
// see README.md.
int RegretIndex(int c1, int c2, bool is_small_blind, bool is_fold);

bool OpenRegretTree(int* regret_tree, const char* file_name);
void SaveRegretTree(const int* regret_tree, const char* file_name);