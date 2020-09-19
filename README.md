# Building An Assembler
Implementation of the basic functionality of An Assembler, conversion of assembly code to machine code. 

## Details

This code has two part. 
###### 1. hashtable.h
 Use the hash table to store user-defined symbol, opcode and register's inforamation. 
###### 2. validating assembly code.cpp
Check for valid syntax and convert the assembly code to machine code.

## Classes and Functions

### hashtable.h
####  class Hash
1.computeHash(key)--Computes a hashindex forgiven key 

2.hasKey(key)--Returns true if key is stored in Table else False

3.addRecord(key,Object)--Inserts a new record into the table

4.getRecord(key)--Returns a record corresponding to the given key

5.replaceRecord(key,Object)--Replaces the record for given key

Collision handling strategy:- Separate Chaining

#### class HTWrapper
1.saveToFile(filename, HashTable)--Save the hash table to a file for later use.

2.loadFromFile(filename)--load a hash table previously stored in the input file









