#include <iostream>
#include <fstream>

using namespace std;

int main() {
	const char* filename = "regrets.dat";
	ofstream o(filename, ios::binary);

	int abc = 0;
	for (int i = 0; i < 2704; i++) {
		o.write((char*)& abc, 4);
	}

	o.close();

	return 0;
}