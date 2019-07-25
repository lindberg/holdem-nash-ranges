#include <iostream>
#include <fstream>

// FastMod from https://stackoverflow.com/a/33333636/1619888
int FastMod(const int input, const int ceil);

// Returns the index of regret_tree where the node we are searching for is.
// c1 and c2 are the cards represented by integers 1-52. For more information,
// see README.md.
int RegretIndex(int c1, int c2, bool is_small_blind, bool is_fold);

bool OpenRegretTree(int* regret_tree, const char* file_name);
void SaveRegretTree(const int* regret_tree, const char* file_name);

const int kNodes = 676;

int main() {
	int regret_tree[kNodes];

	memset(regret_tree, 0, sizeof(regret_tree));

	if (!OpenRegretTree(regret_tree, "regret_tree.dat")) {
		SaveRegretTree(regret_tree, "regret_tree.dat");
	}

	int c1 = 13;
	int c2 = 33;

	std::cout << RegretIndex(c1, c2, false, true) << std::endl;
	
	while (1) {

	}

	return 0;
}

int RegretIndex(int c1, int c2, bool is_small_blind, bool is_fold) {
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

int FastMod(const int input, const int ceil) {
	// Apply the modulo operator only when needed
	// (i.e. when the input is greater than the ceiling).
	return input >= ceil ? input % ceil : input;
	// NB: the assumption here is that the numbers are positive.
}

bool OpenRegretTree(int* regret_tree, const char* file_name) {
	FILE* fin = fopen("regret_tree.dat", "rb");
	if (!fin) {
		return false;
	}

	fread(regret_tree, sizeof(regret_tree), 1, fin);

	fclose(fin);

	return true;
}

void SaveRegretTree(const int* regret_tree, const char* file_name) {
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
