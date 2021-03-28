#include "Interpreter.h"

int main() {
	Interpreter scheme;
	while (true) {
		// if hash table is full, break
		scheme.interpret();
	}
	return 0;
}

