#pragma once

#include <vector>

#include "regret_tree.h"

class RegretSystem {
private:
	std::vector<int> HR;

	short rand_key = 0;
	unsigned int rand1[4];
	unsigned int rand2[4];

	//int RegretSystem::RandomNumber();

	void DealCards(int* cards);
	void Work(int c1, int c2, int c3, int c4, int stack_in_small_blinds);
public:
	RegretTree regret_tree_;

	RegretSystem();

	// Returns hand strength. pCards should be a pointer to an array
	// of 7 integers each with value between 1 and 52 inclusive.
	int LookupHand(int* cards);

	void RunIterations(int iterations, int stack_in_small_blinds);
};