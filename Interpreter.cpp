#include <iostream>
#include "Interpreter.h"

// default constructor
Interpreter::Interpreter() {
	//initialization
	table = HashTable(hashSize);
	// built-in function, variables
	std::string sym[BuiltInNum] = { "lambda","#t", "#f", "else", "+", "-", "*", "eq?", "equal?", "number?","symbol?","null?", "cons", "cond", "car","cdr","define","quote","=" };

	for (int i = 0; i < BuiltInNum; i++) {
		hashing(sym[i]);
	}
	valueInd = 0;
	str = nullptr;
	memInd = 1;
	memory = new Node[memorySize];
	memory[0].left = 0;
	memory[0].right = 0;
	memRoot = 0;
}

Interpreter::~Interpreter() {
	delete[] memory;
  table.deleteTable();
} 

// If memory's root is 1 then only one sym is entered
bool Interpreter::checkOneSym() {
	return memInd == 1;
}

// allocate memory next Index
int Interpreter::alloc() {
	// if memory is full, execute garbage collect
	if (isMemoryFull() || memInd >= memorySize-1) {
		garbageCollect();
		memInd = 1;
	}
	// find remaining memory node 
	int a = (memInd++) % memorySize;
	while (memory[a].left != NULL|| memory[a].linked!=EMPTY || a == 0) {
		a = (a + 1) % memorySize;
		memInd++;
	}
	memory[a].linked = NOTYET;
	return a;
}

// Reinitialize member variables
void Interpreter::reset() {
	valueInd = 0;
	for (int i = 1; i < memorySize; i++) {
		memory[i].linked = NOLINKED;
	}
	delete str;
}

// getCode, then read, store memory.
bool Interpreter::interpret() {
	getCode();
  //preprocessing
	*str = preprocessing("");
	valueInd = 0;
	int readResult = read();
	int result = eval(readResult);
	print(result);
	reset();
	return result;
}

// get code and change all characters to lowerCase.
void Interpreter::getCode() {
	std::string* input = new std::string();
	std::cout << ">";
	std::getline(std::cin, *input);
	// given input is empty string, then get code again
	while (*input == "") {
	std::cout << ">";
		std::getline(std::cin, *input);
	};
		//change input string to lower character
	for (int i = 0; i < (*input).length(); i++) {
		(*input)[i] = std::tolower((*input)[i]);
	}
	while ((*input)[(*input).length()-1] == ' ') {
		(*input).pop_back();
	}
	str = input;
}

// calculate given tokens to hash value. Then insert hash to table. return minus value
int Interpreter::hashing(std::string s) {
	int hv = strToInt(s);
	hv=table.insert(hv, s);
	return -hv;
}

// token Iterator parsing given code to token.
std::string Interpreter::getNextValue() {
	std::string input = *str;
	//token
	std::string hashValue="";

	while (true) {
    //end of given code
		if (valueInd == input.size()) {
			break;
		}
		//If hashValue is empty then return ( / ), else ( / ) is operated as delimiter.
		else if (input[valueInd] == '('||input[valueInd] == ')') {
			if(hashValue.empty())
			  hashValue.push_back(input[valueInd++]);
			break;
		}

		else if (input[valueInd] == '\'' && hashValue.empty()) {
			hashValue.push_back(input[valueInd++]);
			break;
		}

		//push input character to hashvalue
		else if (input[valueInd] != ' ') {
			hashValue.push_back(input[valueInd++]);
		}

		
		else {
			if (!hashValue.empty())
				break;
			
			while (valueInd != input.size()-1 &&input[valueInd] == ' ') {
			  valueInd++;
			}
		}
	}
 	return hashValue;

};

// store token to hash table and make parsing tree.
// make node and recursively executed.
int Interpreter::read() {
	int root = NULL;
	int temp;
	bool first = true;
	
	std::string a = getNextValue();
	//If only one symbol is entered.

	// skip blank token.
		while (a == " ") {
			a = getNextValue(); 
		}
		// make new node branch 
		if (a == "(") {
			a = getNextValue();
			// If token is not ")" then add hash to hash table or if token is "(" then recursively execute read function.
			while (a != ")") {
				if (checkFull()) return NULL;

				// make root. 
				if (first == true) {
					temp = alloc();

					//std::cout << "root: " << temp<<std::endl;
					root = temp;
					first = false;
					if (memRoot == 0) {
						memRoot = root;
					}
				}
				else {
					memory[temp].right = alloc();
					temp = memory[temp].right;
				}
				// recursively make list.
				if (a == "(") {
					valueInd--;
					memory[temp].left = read();
				}
				
				else {
					memory[temp].left = hashing(a);
				}
				memory[temp].right = NULL;
				a = getNextValue();
			}
			return root;
		}
		else {
			return hashing(a);
		}
}

// string to int(hash value)
unsigned Interpreter::strToInt(std::string symbol) {
	int length = (int)symbol.length();
	unsigned int answer = 0;
	if (length % 2 == 1) {
		answer = symbol.at(length - 1);
		length--;
	}

	for (int i = 0; i < length; i += 2) {
		answer += symbol.at(i);
		answer += ((int)symbol.at(i + 1)) << 8;
	}
	// modulo is needed 
	answer = answer % hashSize;
	if (answer == 0) answer++;
	return answer;
}

//ofstream overloading for easy cout
std::ostream& operator<<(std::ostream& os, Hash h) {
	os << "hash value: " << h.getHashVal();
	os << " symbol: " << h.getSym();
	os << " link: " << h.getLink() << std::endl;
	return os;
};


// recursively print memory list. 
void Interpreter::printMem(int root) {
	std::cout << "(";
	int tem = root;
	bool nextBlank = false;
	// if tem ==0, then It is end of list.
	while (tem != 0) {
		if (memory[tem].left > 0) {
			std::cout << " ";
			printMem(memory[tem].left);
			tem = memory[tem].right;
		}
		else {
			//if nextBlank is false, there is no need to use " " because it is front of )
	    if(nextBlank) std::cout << " ";
			std::cout << table.get(-memory[tem].left).getSym();
			tem = memory[tem].right;
			nextBlank = true;
		}
	}
	std::cout << ")";
}

//print hashTable, momoryList, list
// for debuging
void Interpreter::printAll() {
	std::cout << "\n] ";
	std::cout << "free list's root: "<< memInd<< std::endl;
	std::cout << "List's root: " << memRoot << std::endl;

	std::cout << "\n<--memory table-->" << std::endl;
	for (int i = 0; i < memorySize;i++) {
		std::cout << "left: " << memory[i].left;
		std::cout << " right: " << memory[i].right << std::endl;
	}
	std::cout << "\n<--hash table-->" << std::endl;
	for (int i = 0; i < hashSize; i++) {
		if (!table.empty(i)) {
			std::cout << table.get(i);
		}
	}
	std::cout <<"\n"<< std::endl;
}

// print result recursively.
void Interpreter::printResult(int result,int startList) {
	if (result == -hashSize) return;
	if (result == 0)
		std::cout << "()";
	else if(result<0)
	  std::cout <<table[result].symbol;
	else {
		if(startList)
			std::cout << "(";
		printResult(memory[result].left, true);
		if (memory[result].right != NULL) {
			std::cout << " ";
			printResult(memory[result].right, false);
		}
		else
			std::cout << ")";
	}
};

//print evaluation result.
void Interpreter::print(int result) {
	if(result !=-hashSize) std::cout << "]";
	printResult(result, true);
	if(result!=-hashSize) std::cout << std::endl;
}

//check table is full.
bool Interpreter::checkFull() {
	if (table.full()) {
		std::cout << "hash full" << std::endl;
		return true;
	}
	return false;
}

// concat a and b
std::string Interpreter::concatenate(std::string a, std::string b) {
	std::string str = a+" "+b;
	return str;
}

// preprocess ' -> (quote , define funciton to lambda
std::string Interpreter::preprocessing(std::string str) {
	//std::string a = getNextValue();
	std::string a;
	std::string newCommand = "";
	while ((a=getNextValue()) != "") {
	  if (a == "_memory_")
		  printAll();
		if (a == "define") {
			newCommand = concatenate(newCommand, "define ");
			a = getNextValue();

			// if define user function, store it as lambda function
			if (a == "(") {
				a = getNextValue();
				newCommand = concatenate(newCommand, a + " (lambda (" + preprocessing(newCommand) + ")");
				a = getNextValue();
			}
			else {
				newCommand = concatenate(newCommand, a);

			}
		}

		// make ' to quote
		else if (a == "'") {
			newCommand = concatenate(newCommand, "(quote");
			int leftParenNum = 0;
			do {
				a = getNextValue();
				newCommand = concatenate(newCommand, a);
				if (a == "(") {
					leftParenNum++;
				}
				else if (a == ")") {
					leftParenNum--;
				}
			} while (leftParenNum > 0);
			newCommand = concatenate(newCommand, ")");
		}
		else if (a == "")
			break;
		else {
			newCommand = concatenate(newCommand, a);
		  //a = getNextValue();
		}
	}
	//std::cout << "newCommand: "<< newCommand << std::endl;
		return newCommand;
}

//return double value
double Interpreter::getVal(int val) {
	return stod(table[val].symbol);
}

//check given index's hash symbol is number
bool Interpreter::isNumber(int value) {
	std::string str = table[value].symbol;
	for (int i = 0; i < str.length(); i++) {
		if (!isdigit(str[i])&&str[i]!='.')
			return false;
	}
	return true;
}

// check param and arg is related.
bool Interpreter::checkParamIn(int param, int arg) {
	if (param == arg) return true;
	if (arg < 0) return false;
	while (memory[arg].right) {
		if (checkParamIn(param, memory[memory[arg].right].left)) {
			return true;
		}
		arg = memory[arg].right;
	}
	return false;
};

// evaluate given root's input
// return hashvalue
int Interpreter::eval(int root) {
	// root <0, then return hashvalue which index is root. if hash has link, then connect to link 
	if (root == 0) return 0;

	if (root < 0) {
		int link = table[root].linkVal;
		if (!link) return root;
		return link;
	}

	// like '(a), several case memory[root].left is positive. then tokenInd refer odd value. preprocess this situation.
	while (memory[root].left > 0) {
		root = memory[root].left;
	}

	int tokenInd = -table[memory[root].left].hashValue;

	if (tokenInd == PLUS)
		return hashing(std::to_string(getVal(eval(memory[memory[root].right].left)) + getVal(eval(memory[memory[memory[root].right].right].left))));

	else if (tokenInd == MINUS)
		return hashing(std::to_string(getVal(eval(memory[memory[root].right].left)) - getVal(eval(memory[memory[memory[root].right].right].left))));

	else if (tokenInd == TIMES)
		return hashing(std::to_string(getVal(eval(memory[memory[root].right].left)) * getVal(eval(memory[memory[memory[root].right].right].left))));

	// check given input is number. if number, return #t.
	else if (tokenInd == isNUMBER) {
		int result = eval(memory[memory[root].right].left);
		if (isNumber(result)) {
			return hashing("#t");
		}
		return hashing("#f");
	}

	// check given input is symbol. if number, return #t.
	else if (tokenInd == isSYMBOL) {
		int result = eval(memory[memory[root].right].left);
		if (result && !isNumber(result))
			return hashing("#t");
		return hashing("#f");
	}

	// check null. 
	else if (tokenInd == isNULL) {
		if (memory[root].right == 0 || eval(memory[root].right) == 0){
			//printAll();
				return hashing("#t");
		}
		return hashing("#f");
	}

	else if (tokenInd == SAME) {
		if (std::stod(table[eval(memory[memory[root].right].left)].symbol) == std::stod(table[eval(memory[memory[memory[root].right].right].left)].symbol))
			return hashing("#t");
		return hashing("#f");
	}

	else if (tokenInd == isEQ) {
		if (eval(memory[memory[root].right].left) == eval(memory[memory[memory[root].right].right].left))return hashing("#t");
		return hashing("#f");
	}

	else if (tokenInd == isEQUAL) {
		if (checkStructure(eval(memory[memory[root].right].left), eval(memory[memory[memory[root].right].right].left))) return hashing("#t");
		return hashing("#f");
	}
	//if cons a b, add a to list b
	else if (tokenInd == CONS) {
		int newMemory = alloc();
		memory[newMemory].left = eval(memory[memory[root].right].left);
		memory[newMemory].right = eval(memory[memory[memory[root].right].right].left);
		return newMemory;
	}

	//check several condition and if condition is true, then return value. if all condition is false return else value.
	else if (tokenInd == COND) {
		while (memory[memory[root].right].right != 0) {
			root = memory[root].right;
			// condition is true
			if (eval(memory[memory[root].left].left) == -TRUE) {
				if (isNumber(memory[memory[memory[root].left].right].left)) {
					return eval(memory[memory[memory[root].left].right].left);
				}
				//std::cout << "cond "<<memory[memory[root].left].right << std::endl;
				return eval(memory[memory[root].left].right);
			}
		}
		//all condition is false
		if (memory[memory[memory[root].right].left].left != -ELSE) {
			std::cout << "error" << std::endl;
			return 0;
		}
		return eval(memory[memory[memory[memory[root].right].left].right].left);
	}

	// first element of list
	else if (tokenInd == CAR) {
		return memory[eval(memory[memory[root].right].left)].left;
	}
  // pointer of elements that second to last element
	else if (tokenInd == CDR){
		int a = memory[eval(memory[memory[root].right].left)].right;
		return a;
	}

	// function, variable define.
	else if (tokenInd == DEFINE) {
		if (memory[memory[memory[memory[root].right].right].left].left==-LAMBDA)
			table[memory[memory[root].right].left].linkVal = memory[memory[memory[root].right].right].left;
		else table[memory[memory[root].right].left].linkVal = eval(memory[memory[memory[root].right].right].left);
		return -hashSize;
	}

  // given tokenInd is quote then memory node's right's left is first element of list
	else if (tokenInd == QUOTE) {
		return memory[memory[root].right].left;
	}

	//process user define function.
	else {
		if (!table[tokenInd].linkVal) return NULL;
		//if (!table[tokenInd].linkVal) return eval(-tokenInd);
		if (memory[table[tokenInd].linkVal].left!=-LAMBDA) {
			return eval(-tokenInd);
		}

		// link given arguments to parameter and stack existing values.
		Node param = memory[memory[memory[table[memory[root].left].linkVal].right].left];
		Node arg = memory[memory[root].right];

		//if lambda function, then return root
		if (arg.left > 0 && tokenInd == LAMBDA) {
			return root;
		}
	
		int stackNum = 0;

		// stack variables
		while (param.left&& arg.left!= 0) {
			stackNum++;
			stack.push(table[param.left]);

			// if recursively assign same variable into variable, then assign not hash value, assign variable's link
			if (arg.left == param.left) {
			  table[param.left].linkVal = arg.left == param.left? table[arg.left].linkVal :arg.left;
			}
			else {
			  table[param.left].linkVal = eval(arg.left);
			}
      

			param = memory[param.right];
			if (arg.right == 0) {
				break;
			}
			arg = memory[arg.right];
		};

		int result = eval(memory[memory[memory[table[memory[root].left].linkVal].right].right].left);

		//if result is evaluated, unstack.
		while (stackNum--) {
			table[stack.top().hashValue].linkVal = stack.top().linkVal;
			stack.pop();
		}
		return result;
	}
}

// check first function and second function has same structure.
// if each function has different parameter varaible, return false.
int Interpreter::checkStructure(int first, int second) {
	if (first == second) {
		return true;
	}
	if (first < 0 && second < 0) {
		return first == second;
	}
	if (!checkStructure(memory[first].left, memory[second].left)) {
		return false;
	}
	if (!checkStructure(memory[first].right, memory[second].right)) {
		return false;
	}
	return true;
}

// check memory is left
bool Interpreter::isMemoryFull() {
	for (int i = 1; i < memorySize; i++) {
		if (memory[i].left == EMPTY)
			return false;
	}
	std::cout << "memoryFull" << std::endl;
	exit(1);
	return true;
}

// check memory is linked from hash table
void Interpreter::checkLinked(int root) {
	if (root <= 0) return;
	if (memory[root].linked == NOLINKED) {
		memory[root].linked = LINKON;
		if (memory[root].right) {
			checkLinked(memory[root].right);
		}
		if (memory[root].left) {
			checkLinked(memory[root].left);
		}
	}
}

// if memory is full, this funciton is executed.
// check memory node is linked nad if memory is not linked, collect memory node.
void Interpreter::garbageCollect() {
	std::cout << "*************garbage collect on************" << std::endl;
	// check link
	for (int i = 1; i < hashSize; i++) {
		if (table[i].linkVal != EMPTY) {
			checkLinked(table[i].linkVal);
		}
	}
	// collect memory
	for (int i = 1; i < memorySize; i++) {
		switch (memory[i].linked){
		case NOLINKED:
			memory[i].left = EMPTY;
			memory[i].right = EMPTY;
			memory[i].linked = NOLINKED;
			break;
		
		default:
			memory[i].linked = NOLINKED;
			break;
		}
	}

	// collect memory but their is no left memory, then cout no memory left.
	if (isMemoryFull()) {
		std::cout << "not enough memory" << std::endl;
	}
	
}


	