//Stack class. Container which Last-in-first-out. 
//Many method that makes Stack useful container such as Isempty,push,pop,expandCapacity...
// This is template class

#pragma once
template <class T>
class Stack{
public:
    //default constructor that recieve argument capacity.
	// stack is made which size is input capacity.
	Stack(int stackCapacity = 10);

	//return whether it is empty
	bool isEmpty() const;

	// get top element of Stack.
	T& top() const;

	// push item. If stack is full, capacity is increased.
	void push( T& item);
	
	//pop item.
	void pop();

	// Make capacity double 
	void expandCapacity();

	// return top Index
	int getTopInd() const;

	//return element which index is argument.
	T getItem(int index);

private:
	int topInd;

	// container with array.
	T* stack;

	//size of container
	int capacity;
};

// default constructor. define capacity, allocate memory.
template <class T>
Stack<T>::Stack(int stackCapacity) {
	capacity = stackCapacity;
	if (capacity < 1) throw "Stack capacity mustbe over 0";
	stack = new T[capacity];
	topInd = -1;
}

template <class T>
bool Stack<T>::isEmpty() const {
	return topInd == -1;
}

//Return top element of Stack. If container is empty, make err.
template <class T>
T& Stack<T>::top() const {
	try {
		if (isEmpty()) throw "Stack is empty";
	}
	catch(std::string a){
		std::cout << "error msg: " << a <<std::endl;
		//abort();
	}
	return stack[topInd];
}

//add item in container,.
template <class T>
void Stack<T>::push(T& item) {
	if (topInd == capacity - 1) {
		expandCapacity();
	}
	stack[++topInd] = item;
}


// make capacity double
template <class T>
void Stack<T>::expandCapacity() {
	capacity *= 2;
	T* newStack = new T[capacity];
	for (int i = 0; i <= topInd; i++) newStack[i] = stack[i];
	delete[]stack;
	stack = newStack;
}

template <class T>
void Stack<T>::pop() {
	if (isEmpty()) throw "Stack is empty";
	//std::cout << "pop" << std::endl;
	stack[topInd--].~T();
}

template <class T>
int Stack<T>::getTopInd() const {
	return topInd;
}

//return index's element
template <class T>
T Stack<T>::getItem(int index) {
	return stack[index];
}
