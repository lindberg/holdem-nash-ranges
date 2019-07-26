#pragma once

#include <vector>

#include "regret_tree.h"

class RegretSystem {
private:
	RegretTree regret_tree_;

	std::vector<int> HR;
public:
	RegretSystem();
	void RunIterations(int iterations);
	// Returns hand strength. pCards should be a pointer to an array
	// of 7 integers each with value between 1 and 52 inclusive.
	int LookupHand(int* cards);
	std::vector<int> DealCards(int n);
};