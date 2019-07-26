#include <iostream>
#include "regret_tree.h"

int main() {
	RegretTree regret_tree_;

	if (!regret_tree_.OpenRegretTree("regret_tree.dat")) {
		regret_tree_.SaveRegretTree("regret_tree.dat");
	}

	int c1 = 13;
	int c2 = 33;

	std::cout << regret_tree_.RegretIndex(c1, c2, false, true) << std::endl;

	return 0;
}
