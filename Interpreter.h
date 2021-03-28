// scheme interpreter class that interpret scheme codes. 
// This clss can make hash table, memory node .. etc.
// Given code converted to parsing tree.
#include <cctype>
#include "Node.h"
#include "HashTable.h"
#include "Stack.h"
enum size {
	memorySize = 100,
	BuiltInNum = 19,
};

enum State {
	NOLINKED = 0,
	NOTYET = -1,
	LINKON=1,
	EMPTY = 0,
	FULL = 1,
};

class Interpreter {
public:
  //constructor
	Interpreter();
	~Interpreter();

	//enter One line code and store in private member "str"
	void getCode();
	
	// store given token in hash table
	int hashing(std::string s);

	// include getCode, read method.
	bool interpret();

	//print memory list, freelist, hash table, list  
	// for debuging
	void printAll();

	//for debuging
	// print memory list recursively
	void printMem(int root);

	// read token and make memory node, hash.
	int read();

	// allocate memory next node index.
	int alloc();

	//reset memory list, root node.
	void reset();

	//iterator that give token.
	std::string getNextValue();

	// calculate string to hash value.
	unsigned int strToInt(std::string symbol);

	//check table is full.
	bool checkFull();

	//check given input has only one symbol.
	bool checkOneSym();
	
	// preprocess given input which input has ' or input is user-define function
	std::string preprocessing(std::string str);
	
	// return string a+b 
	std::string concatenate(std::string a, std::string b);

	// evaluate given input and return hashValue which result is stored
	int eval(int root);

	// convert hash symbol to number
	double getVal(int val);

	//check given hash symbol is number
	bool isNumber(int value);

	// print given inputs result
	void printResult(int result,int startList);
	void print(int result);
  
	// check left , right function has same structure.
	int checkStructure(int left, int right);

	// check memory tree and if no left memory, return true.
	bool isMemoryFull();

	// collect memory node
	void garbageCollect();

	// check memory is linked
	void checkLinked(int root);
	
	// if param and arg is same or eval(arg) is related with param variable, then return true
	bool checkParamIn(int param, int arg);
private:
	std::string* str;

	// freeList's root.
	int memInd;

	// used in get Token. current index in given Input string.
	int valueInd;

	// list Root.
	int memRoot;

	// list.
	Node* memory;
	HashTable table;
  Stack<Hash> stack;
};

//ofstream overloading with hash
std::ostream& operator<<(std::ofstream& os, Hash h);