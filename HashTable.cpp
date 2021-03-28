#include "HashTable.h"



//constructor
HashTable::HashTable() {
  table = new Hash[hashSize];
  _size = hashSize;
}

HashTable::HashTable(int size) {
  _size = size;
  table = new Hash[size];
}

// delete table pointer.
void HashTable::deleteTable() {
  std::cout << " table destructure" << std::endl;
  delete[] table;
}

//check given index's hash is empty.
bool HashTable::empty(int ind) {
  if (table[ind%_size].hashValue == NULL&&ind%_size!=0)
    return true;
  return false;
}

// insert hash in given value. if given index's hash is already exist,
// then check next index. if all hashtable is full,then return null.
// If given symbol is already exist, then return that index.
int HashTable::insert(int _value, std::string symbol, int link) {
  int value = _value;
  std::string _symbol = symbol;
  bool cond = empty(value);
  
  // find possible index.
  while (!cond) {
    if (table[value].symbol == symbol) {
      return value;
    }
    value = (value + 1)%hashSize;
    cond = empty(value);
    if (value == _value%_size) {
      std::cout << "hash table is full" << std::endl;
      return NULL;
    }
  }

  // set value.
  table[value].set(-value, link, _symbol);
  return value;
}

Hash HashTable::get(int ind) {
  return table[ind];
};

// check all hash is full.
bool HashTable::full() {
  for (int i = 1; i < _size; i++) {
    if (table[i].hashValue == NULL) {
      return false;
    }
  }
  return true;
}

Hash& HashTable::operator[] (int ind){
  if (ind < 0) ind = -ind;
  return table[ind];
}