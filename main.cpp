#include <iostream>
#include "regret_tree.h"

int main() {
	int regret_tree[kNodes];

	memset(regret_tree, 0, sizeof(regret_tree));

	if (!OpenRegretTree(regret_tree, "regret_tree.dat")) {
		SaveRegretTree(regret_tree, "regret_tree.dat");
	}

	int c1 = 13;
	int c2 = 33;

	std::cout << RegretIndex(c1, c2, false, true) << std::endl;

	return 0;
}