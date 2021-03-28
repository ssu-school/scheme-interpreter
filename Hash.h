//class that store hash's information. It has symbol, link, hashValue. 

#include <string>
#include <iostream>
#pragma once

class Hash{
	friend class Interpreter;
	friend class HashTable;
public:
	Hash();
  //return symbol;
	std::string getSym();
	//return hashValue
	int getHashVal();
	int getLink();
	// set hash informations
	void set(int value, int link, std::string _symbol);
private:
	std::string symbol;
	int linkVal;
	int hashValue;
};

