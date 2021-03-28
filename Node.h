//Node class which is used in memory list.
#pragma once
class Node{
	friend class Interpreter;
	Node();
private:
	int left;
	int right;
	int linked;
};

