#include "Hash.h"

//initialize values
Hash::Hash() {
	symbol = "";
	linkVal = NULL;
	hashValue =NULL;
}

//setting
void Hash::set(int value, int link, std::string _symbol) {
	symbol = _symbol;
	linkVal = link;
	hashValue = value;
};

std::string Hash::getSym() {
	return symbol;
};
int Hash::getHashVal() {
	return hashValue;
};

int Hash::getLink() {
	return linkVal;
};
