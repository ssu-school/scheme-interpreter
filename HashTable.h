// hashTable that stores hash. It has methods that return hash, insert hash, empty.. etc.
#pragma once
#include "Hash.h"

#define hashSize 500


//built-in function, built-in variables
enum implicit_variable {
  LAMBDA =69,
  TRUE = 231,
  FALSE=147,
  ELSE=220,
  PLUS=43,
  SAME = 61,
  MINUS=45,
  TIMES=42,
  isEQ=92,
  isEQUAL=214,
  isNUMBER=107,
  isSYMBOL=110,
  isNULL=381,
  CONS=65,
  COND=225,
  CAR=46,
  CDR=313,
  DEFINE=404,
  QUOTE=473,
};

class HashTable{
public:
  HashTable();
  HashTable(int size);

  // destructor that is called when interpret end.
  void deleteTable();

  // insert symbol in hash table. if hashtable's value is already exist, then inserted next position.
  int insert(int value, std::string symbol, int link = 0);

  // if hashTable[ind] is empty then return true
  bool empty(int ind);

  //return given index's hash object. 
  Hash get(int ind);
   
  // check table is full.
  bool full();

  Hash& operator[] (int ind);
private:
  Hash* table;
  int _size;
};

