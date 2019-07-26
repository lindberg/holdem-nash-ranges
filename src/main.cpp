#include <iostream>
#include <cstdlib>
#include <ctime>

#include "regret_system.h"

int main() {
	RegretSystem system_;

	system_.RunIterations(1000000);

	std::cout << "done" << std::endl;

	while (1) {}
	return 0;
}