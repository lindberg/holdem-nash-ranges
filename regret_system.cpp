#include <iostream>

using namespace std;

//FastMod from https://stackoverflow.com/a/33333636/1619888
int FastMod(const int input, const int ceil);

// Returns the index of regret_table where the node we are searching for is.
// c1 and c2 are the cards represented by integers 1-52. For more information,
// see README.md.
int RegretIndex(int c1, int c2, bool is_small_blind, bool is_fold);

int main() {
	// Example hand T9s from README.md
	int c1 = 13;
	int c2 = 33;

	cout << RegretIndex(c1, c2, false, true) << endl;

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
	// apply the modulo operator only when needed
	// (i.e. when the input is greater than the ceiling)
	return input >= ceil ? input % ceil : input;
	// NB: the assumption here is that the numbers are positive
}