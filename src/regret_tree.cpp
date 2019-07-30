#include <iostream>
#include <fstream>

#include "regret_tree.h"

RegretTree::RegretTree() : regret_tree(kNodes) {}

int RegretTree::RegretIndex(int * cards, bool is_small_blind) {
	int index = 0;
	int cards_suit[2], cards_val[2];

	// Make cards[0] the smallest card.
	if (cards[0] > cards[1]) {
		int temp = cards[1];
		cards[1] = cards[0];
		cards[0] = temp;
	}

	cards_suit[0] = FastMod(cards[0], 4);
	cards_suit[1] = FastMod(cards[1], 4);
	cards_val[0] = cards[0] / 4.01 + 2;
	cards_val[1] = cards[1] / 4.01 + 2;

	if (cards_suit[0] == cards_suit[1]) index += 78;

	if (cards_val[0] == cards_val[1]) {
		index = cards_val[0] - 2;
	}
	else {
		index += 13;
		int rows = cards_val[0] - 2;

		// Perform arithmetic sum to get to the right row or column.
		index += rows * (25 - rows) / 2;

		// The last few steps to get to the right position.
		index += cards_val[1] - rows - 3;

	}

	if (!is_small_blind) index += 169;

	return index;
}

bool RegretTree::OpenRegretTree(const char* file_name) {
	std::cout << "Loading " << file_name << " file...";

	FILE* fin = fopen(file_name, "rb");

	if (!fin) {
		printf("failed.\n\n");
		return false;
	}
	
	fread(regret_tree.data(), regret_tree.size()*sizeof(int), 1, fin);

	printf("complete.\n\n");

	return true;
}

void RegretTree::SaveRegretTree(const char* file_name) {
	std::ofstream o(file_name, std::ios::binary);

	char bytes[4];

	for (int i = 0; i < kNodes; i++) {
		bytes[3] = (regret_tree[i] >> 24) & 0xFF;
		bytes[2] = (regret_tree[i] >> 16) & 0xFF;
		bytes[1] = (regret_tree[i] >> 8) & 0xFF;
		bytes[0] = regret_tree[i] & 0xFF;
		o.write(bytes, 4);
	}

	std::cout << file_name << " saved!\n\n";
}

int* RegretTree::GetNodePointer(int* cards, bool is_small_blind) {
	return &regret_tree[RegretIndex(cards, is_small_blind)];
}