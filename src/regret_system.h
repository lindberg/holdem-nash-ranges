#pragma once

#include <vector>

#include "regret_tree.h"

class RegretSystem {
private:
	RegretTree regret_tree_;

	std::vector<int> HR;

	int deck[52];
	int cards[9];
	int* sb_push, * sb_fold, * bb_call, * bb_fold;
	int sb_hand_strength, bb_hand_strength;
	bool draw, sb_won;
	int random_card;
public:
	RegretSystem();
	void RunIterations(int iterations);
	// Returns hand strength. pCards should be a pointer to an array
	// of 7 integers each with value between 1 and 52 inclusive.
	int LookupHand(int* pCards);
};