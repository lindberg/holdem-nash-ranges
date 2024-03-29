#pragma once

#include <vector>
#include "helper_functions.h"

const int kNodes = 338;

class RegretTree {
public:
	std::vector<int> regret_tree;

	RegretTree();

	// Returns the index of regret_tree where the node we are searching for is.
	// The cards are represented by integers 1-52. For more information,
	// see README.md.
	int RegretIndex(int * cards, bool is_small_blind);

	bool OpenRegretTree(const char* file_name);
	void SaveRegretTree(const char* file_name);
	int * GetNodePointer(int * cards, bool is_small_blind);
};