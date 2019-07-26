#include <iostream>
#include <fstream>

#include "regret_tree.h"
#include "helper_functions.h"

RegretTree::RegretTree() {
	memset(regret_tree, 0, sizeof(regret_tree));
}

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
		index += 14;

		int rows = c1_val - 2;

		// Perform arithmetic sum to get to the right row or column
		// (depending on if we are looking at suited or non-suited
		// cards).
		index += rows * (25 - rows) / 2;

		// The last few steps to get to the right position.
		index += c2_val - rows - 3;
	}

	// Every branch consists of 8 bytes.
	--index *= 8;

	if (!is_small_blind) index += 1352;
	if (is_fold) index += 4;

	return index;
}

bool RegretTree::OpenRegretTree(const char* file_name) {
	FILE* fin = fopen("regret_tree.dat", "rb");
	if (!fin) {
		return false;
	}

	fread(regret_tree, sizeof(regret_tree), 1, fin);

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