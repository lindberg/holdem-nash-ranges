#include <iostream>
#include <fstream>

#include "regret_tree.h"
#include "helper_functions.h"

RegretTree::RegretTree() : regret_tree(kNodes) {}
const char* kTreeFileName = "regret_tree.dat";

int RegretTree::RegretIndex(int c1, int c2, bool is_small_blind, bool is_fold) {
	int c1_suit = FastMod(c1, 4);
	int c2_suit = FastMod(c2, 4);
	int c1_val = c1 / 4.01 + 2;
	int c2_val = c2 / 4.01 + 2;
	int index = 0;

	// Make c1 the smallest card.
	if (c1 > c2) {
		int temp = c2;
		c2 = c1;
		c1 = temp;
	}

	if (c1_suit == c2_suit) index += 78;

	if (c1_val == c2_val) {
		index = c1_val - 1;
	}
	else {
		index += 13;

		int rows = c1_val - 2;

		// Perform arithmetic sum to get to the right row or column
		// (depending on if we are looking at suited or non-suited
		// cards).
		index += rows * (25 - rows) / 2;

		// The last few steps to get to the right position.
		index += c2_val - rows - 3;
	}

	if (!is_small_blind) index += 169;

	// Every branch consists of 2 actions.
	--index *= 2;

	if (is_fold) index += 1;

	return index;
}

bool RegretTree::OpenRegretTree(const char* file_name) {
	FILE* fin = fopen(kTreeFileName, "rb");
	if (!fin) {
		return false;
	}

	fread(regret_tree.data(), regret_tree.size()*sizeof(int), 1, fin);

	fclose(fin);

	return true;
}

void RegretTree::SaveRegretTree(const char* file_name) {
	std::ofstream o(file_name, std::ios::binary);

	char bytes[4];

	for (int i = 0; i < kNodes; i++) {
		bytes[0] = (regret_tree[i] >> 24) & 0xFF;
		bytes[1] = (regret_tree[i] >> 16) & 0xFF;
		bytes[2] = (regret_tree[i] >> 8) & 0xFF;
		bytes[3] = regret_tree[i] & 0xFF;
		o.write(bytes, 4);
	}

	o.close();
}

int* RegretTree::GetNodePointer(int card1, int card2, bool is_small_blind, bool is_fold) {
	return &regret_tree[RegretIndex(card1, card2, is_small_blind, is_fold)];
}