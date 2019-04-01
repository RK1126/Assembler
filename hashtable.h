// let's do it oop way
#ifndef hashtable_h
#define hashtable_h
#include<string>
#include<iostream>
#include<list>
#include<typeinfo>
#include<fstream>
using namespace std;

template<typename T, typename U>
class Hash{
	private:
	int array_length;
	list<pair<T, U> >* main_column;
	public:
		
		Hash(int length=10)
		{
			array_length = length;
			//create an array
			main_column = new list<pair<T,U> >[array_length];
			// values will be insert in rows(list)	
		}
		int computeHash(T key);
		void addRecord(T key, U object);
		bool hasKey(T key);
		U getRecord(T key);
		void replaceRecord(T key, U object);
		int retAL(){
			return array_length;
		}
		list<pair<T, U> >* retcol(){
			return main_column;
		}
		
};
	
template<typename T, typename U>
class HTWrapper{
	public:
	void saveToFile(string filename, Hash<T,U> hash_class_object);
	Hash<T,U> loadFromFile(string filename);
};

template<typename T, typename U>
void HTWrapper<T,U>::saveToFile(string filename, Hash<T,U> hash_class_object)
{   // write to file.
	ofstream fileobj;
	int array_length = hash_class_object.retAL();  // hash_class_object-> hash class object
	list<pair<T, U> >*main_column;
	main_column= hash_class_object.retcol();
	 
	fileobj.open(filename.c_str());
	for(int i=0; i<array_length; i++)
	{
		
		typename list<pair<T,U> >::const_iterator it;
		for(it=main_column[i].begin(); it!=main_column[i].end(); ++it)
		{   T key = (*it).first;
			U value = (*it).second;
			
			fileobj<<key<<" "<<value<<endl;
		}
	}	
  fileobj.close();	
}

template<typename T, typename U>
Hash<T,U> HTWrapper<T, U>::loadFromFile(string filename)
{    
	// reading from file and displaying hash table
	ifstream fileobj(filename.c_str());
	Hash<T, U>obj;
	T a;
	U b;	
	// read and insert
	while(fileobj>>a>>b)  // check for type and then add record
	{
		obj.addRecord(a,b);
	}
	//cout<<"Hash Table from file:-"<<endl;
	//obj.display();
	fileobj.close();
	return obj;
}

template<typename T, typename U>
int Hash<T,U>::computeHash(T key)
{
	if(typeid(key).name()==typeid(string).name())// for string
	{
		int sum = 0;
        for (int k = 0; k < key.length(); k++)
            sum = sum + int(key[k]);
        return  sum % array_length;
	}
	else// for int and float
		//return key % array_length;
		cout<<"int and float"<<endl;
		// for character  
}

template<typename T, typename U>
bool Hash<T,U>::hasKey(T key)
{
 	int index = computeHash(key);
		typename list<pair<T, U> >::const_iterator it;
		int count=0;
		for(it=main_column[index].begin(); it!=main_column[index].end(); ++it)
			if((*it).first==key)
			{
				count = 1;
				return true ;
			}
		if(count!=1)
		return false;	
}

template<typename T, typename U>
void Hash<T,U>::addRecord(T key, U object)
{
	int index = computeHash(key);
	// check for existence of key
	bool exist = hasKey(key);
	if(exist)
		cout<<"Duplicate key entries are not allowed."<<endl;
	else
		main_column[index].push_back(make_pair(key, object));
	//cout<<"Key -> "<<key<<" is added"<<endl;	
}

template<typename T, typename U>
U Hash<T,U>::getRecord(T key)
{
	bool exist = hasKey(key);
	if(!exist)
	return "-1";
	else
	{
		int index =  computeHash(key);
		typename list<pair< T,U> >::const_iterator it;
		
		for(it=main_column[index].begin(); it!=main_column[index].end(); ++it)
		if((*it).first==key)
		return (*it).second;				
	}
}

template<typename T, typename U>
void Hash<T,U>::replaceRecord(T key, U object)
{   
	bool exist = hasKey(key);
	if(!exist)
	cout<<"Enter a valid Key"<<endl;
	else
	{
		int index = computeHash(key);
		typename list<pair< T,U> >::iterator it;
		for(it=main_column[index].begin(); it!=main_column[index].end(); ++it)
		if((*it).first==key)
		(*it).second =object;
	}
	
}

#endif
