# Building An Assembler

Implemented basic functionality of An Assembler, Changing assembly code to machine code. 

## Details

This code has two part. 
###### 1. hashtable.h
 Use the hash table to store user-defined symbol, opcode and register's inforamation. 
###### 2. validating assembly code.cpp
Check for valid syntax and convert the assembly code to machine code.

## Classes and Functions

### hashtable.h
####  class Hash
1.computeHash(key)--Computes a hashindex forgiven key \n
2.hasKey(key)--Returns true if key is stored in Table,else False
3.addRecord(key,Object)--Inserts a new record into the table
4.getRecord(key)--Returns a record corresponding to the given key
5.replaceRecord(key,Object)--Replaces the record for given key

Collision handling strategy:- Separate Chaining

#### class HTWrapper
1.saveToFile(HashTable)--Save the hash table to a file for later use.
2.loadFromFile(FILE*)--load a hash table previously stored in the input file

### validating assembly code.cpp
#### class opcodefile
An extended version of HTWrapper.

#### class Assembler
1.createOpcodeFile()
2.createRegisterfile()
3.readCode(filename)
4.objectCodeGenerator(filename)
and other support functions.




