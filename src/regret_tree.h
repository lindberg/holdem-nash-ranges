#pragma once

#include <vector>

const int kNodes = 676;
extern const char* kTreeFileName;

class RegretTree {
public:
	std::vector<int> regret_tree;

	RegretTree();

	// Returns the index of regret_tree where the node we are searching for is.
	// c1 and c2 are the cards represented by integers 1-52. For more information,
	// see README.md.
	int RegretIndex(int c1, int c2, bool is_small_blind, bool is_fold);

	bool OpenRegretTree(const char* file_name);
	void SaveRegretTree(const char* file_name);
	int * GetNodePointer(int card1, int card2, bool is_small_blind, bool is_fold);
};