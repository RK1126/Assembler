//validating  assembly code
//TDM-GCC 4.9.2 64 BIT RELEASE
#include"hashtable.h"
#include<vector>
#include<sstream>
#include<iterator>
#include<string>
#include<ctime>  // for timestamp
#include<bitset>
struct opcode{	
	int mac_code;
	int valid_operands;
	int inst_size;		
};

template<typename T, typename U>
class opcodefile: public HTWrapper<T, U>{
	
	public:
		void saveToFile(string filename, Hash<T, U> hash_class_object);
		Hash<string, struct opcode> loadFromFile(string filename);
};

template<typename T, typename U>
void opcodefile<T,U>::saveToFile(string filename, Hash<T, U> hash_class_object)
{
	ofstream fileobj;
	int array_length = hash_class_object.retAL();
	list<pair<T, U> >*main_column;
	main_column= hash_class_object.retcol();
	 	
	fileobj.open(filename.c_str());
	for(int i=0; i<array_length; i++)
	{	
		typename list<pair<T,U> >::iterator it;
		for(it=main_column[i].begin(); it!=main_column[i].end(); ++it)
		{   T key = (*it).first;
			int value_1 = (*it).second.mac_code;
			int value_2 = (*it).second.valid_operands;
			int value_3 = (*it).second.inst_size;
			
			fileobj<<key<<" "<<value_1<<" "<<value_2<<" "<<value_3<<" "<<endl;
		}
	}	
  fileobj.close();	
}

template<typename T, typename U>
Hash<string, struct opcode> opcodefile<T,U>::loadFromFile(string filename)
{
	ifstream fileobj(filename.c_str());
	Hash<string, struct opcode>obj;
	string a;
	struct opcode b;	
	// read and insert
	// check for type and then add record
	while(fileobj>>a>>b.mac_code>>b.valid_operands>>b.inst_size)  
		obj.addRecord(a,b);
	
	fileobj.close();
	return obj;
}

template<typename T, typename U>
class Assembler:public Hash<T,U>{	
		struct opcode obj;
		Hash<string, struct opcode>opcode_table_object; // opcode hashtable object
		Hash<string, string>register_table_object, user_defined_symbol_table_object; // register hashtable object
		int locptr; // location pointer
	public:
		// main functions
		void createOpcodeFile();  // to create file of opcodes
		void createRegisterfile();  // to create file of registers
		void readCode(string filename);
		void objectCodeGenerator(string filename);
		// supportive functions
		int check(string line, int line_num);  // check for existence
		struct opcode getRecord(string key);
		void addToUserDefinedSymbolTable(string symbol, int loaction);
		int checkForMnenonics(vector<string>, int line_num);	
		// extra functions
		int to_int(string str) // built-in in new versions
		{   int num=-1;
			stringstream s(str);
			s>>num;
			return num;
		}
		string to_string(int num) // built-in in new versions
		{
			stringstream s;
	        s<<num;
	        return s.str();
		}
		vector<string>createTokens(string line)
		{
			istringstream ss(line);
    		vector<string> result((istream_iterator<string>(ss)), istream_iterator<string>());
    		return result;
		}
		string timestamp();
};

template<typename T, typename U>
string Assembler<T,U>::timestamp()  // find a easy way???
{
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];
	time (&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buffer,sizeof(buffer),"%d%m%Y %H%M",timeinfo);
	string str(buffer);
	return str;
}

template<typename T, typename U> 
struct opcode Assembler<T,U>::getRecord(string key)
{
	int index =  opcode_table_object.computeHash(key);
	list<pair<T,U> >* main_column = opcode_table_object.retcol();
	
	typename list<pair< T,U> >::iterator it;
	for(it=main_column[index].begin(); it!=main_column[index].end(); ++it)
	if((*it).first==key)	
	return (*it).second;
	
}

template<typename T, typename U>
void Assembler<T,U>::createOpcodeFile()
{ 
	opcodefile<string, struct opcode>opcode;
	int x;
	string s;
	cout<<"Do you want to insert values in opcode table if yes type 1 else 0:- ";
	cin>>x;
	if(x==1)
    while(x==1) //  opcode table to file
    {
    cout<<"Enter value for OpCode table...."<<endl;
	cout<<"Mnemonics :- ";
	cin>>s;
	cout<<"Machine Code:- ";
	cin>>obj.mac_code;
	cout<<"Valid # of operands :- ";
	cin>>obj.valid_operands;
	cout<<"Instruction Size :- ";
	cin>>obj.inst_size;
	opcode_table_object.addRecord(s, obj);
	opcode.saveToFile("opcode.txt", opcode_table_object);
	
	cout<<"Do you want to continue if yes type 1 else 0:-  ";
	cin>>x;	
	}
	else
	opcode_table_object = opcode.loadFromFile("opcode.txt");
}

template<typename T, typename U>
void Assembler<T,U>::createRegisterfile()
{
	HTWrapper<string, string>regfile;
	string s;
	int x;
	cout<<"Do you want to insert values in register table if yes type 1 else 0:- ";
	cin>>x;
	if(x==1)
	while(x==1)// register table to file
	{   string b;
		cout<<"Enter value for register table...."<<endl;
		cout<<"Register :-             ";
		cin>>s;
		cout<<"Binary Representation:-  ";
		cin>>b;
		register_table_object.addRecord(s,b);
		regfile.saveToFile("register.txt", register_table_object);
		cout<<"Do you want to continue if yes type 1 else 0:-  ";
		cin>>x;
	}
	else
	register_table_object = regfile.loadFromFile("register.txt");
}

template<typename T, typename U>
void Assembler<T,U>::readCode(string filename)
{	ifstream file(filename.c_str());
	string line;
	int error_count=0,line_num =0;
	//first pass
	while(file) //EOF file pointer
	{   
		getline(file,line);		
		if(line.size()>0)
		{	if(line=="END")
			break;
			++line_num;	
			error_count = error_count +check(line, line_num); // check for mnemonics	
		}				
	}
	file.close();  
	
	list<pair< string,string> >::iterator it;
//	check in hash table for -1 for label related error
    for(int i=0; i<10; i++)
	for(it=user_defined_symbol_table_object.retcol()[i].begin(); it!=user_defined_symbol_table_object.retcol()[i].end(); it++)
	if((*it).second=="-1")
	{
		cout<<(*it).first<<" was never declared in the program"<<endl;
		error_count++;
	}
	
	if(!error_count) // second pass
	{
		cout<<"\nChanging to machine code .... "<<endl;
		objectCodeGenerator(filename);
		cout<<"\nDone"<<endl;	
	}
	else
	cout<<"\nASSEMBLER: Found some errors in your program. Cannot proceed!"<<endl;
		
}

template<typename T, typename U>
int Assembler<T,U>::check(string line, int line_num)
{   bool exist_in_mnemonics_table, exist_in_register_table;
	int error_count =0;
	
    vector<string>tokens = createTokens(line);
    exist_in_mnemonics_table = opcode_table_object.hasKey(tokens[0]); // check for existence 
    
    if(!exist_in_mnemonics_table)  // should be label
    {	
    	char c = *(tokens[0]).rbegin();
    	if(c ==':')
    	{   addToUserDefinedSymbolTable(tokens[0].substr(0,tokens[0].length()-1), locptr);
    		if(tokens[1].length()>0) // L2: mov blah blah
    		{
    		// for mnenonics
//    		string s="";
//    		for(int i=1; i<tokens.size();i++)
//    		s = s + tokens[i]+" ";
			tokens.erase(tokens.begin());
    		error_count = error_count+ checkForMnenonics(tokens, line_num);
			}
		}
    	else
    	{
    		cout<<"Unknown Operation "<< tokens[0]<<endl;
    		error_count++;
		}	
	}
	else // when exist in mnenonics table
		error_count = error_count+checkForMnenonics(tokens, line_num);
	
	return error_count;
}

template<typename T, typename U>
void Assembler<T,U>::addToUserDefinedSymbolTable(string symbol, int location)
{   
	bool exist_in_user_defined_symbol_table = user_defined_symbol_table_object.hasKey(symbol);
	
	string loc= to_string(location);
	if(!exist_in_user_defined_symbol_table)
	user_defined_symbol_table_object.addRecord(symbol, loc);
	else
	if(user_defined_symbol_table_object.getRecord(symbol)=="-1")
	user_defined_symbol_table_object.replaceRecord(symbol, loc);	
}

template<typename T, typename U>
int Assembler<T,U>::checkForMnenonics(vector<string> result, int line_num)
{   int error_count=0, no_of_operands;
 	bool exist_in_mnemonics_table, exist_in_register_table;
	bool valid_operand;
	
	// for valid operands
	no_of_operands = result.size()-1;
	
	int ret_operand = getRecord(result[0]).valid_operands; // returning number of operands
	if(no_of_operands==ret_operand)
	valid_operand =true;
	else
	valid_operand=false;
	
	if(!valid_operand)
	{
		cout<<"Invalid numbers of operand in line"<<line_num<<endl;
		error_count++;	
	}
	else // for valid operands
	{   
		
		if(line_num==1 and result[0]=="START" )  // if 1st line start with "START" then locptr init
			locptr = to_int(result[1]);
		else if(line_num==1)
			{
				cout<<"Code must contain \'Start\' as first string"<<endl;
				error_count++;
			}
	  	else if(result[0]=="BYTE" )
			 exist_in_register_table = register_table_object.hasKey(result[1]);
		else
			 exist_in_register_table = register_table_object.hasKey(result[1].substr(0, result[1].length()-1));
		
		// for user defined symbols
		if(!exist_in_register_table )
		if(result[0]=="JUMP" and result[1].length()>0)
			  addToUserDefinedSymbolTable(result[1], -1);	
		else if(result[0]=="BYTE" and result[1].length()>0)
				addToUserDefinedSymbolTable(result[1], locptr);				
		else if(result[0]!="START" and result[1].length()>0)
				 	addToUserDefinedSymbolTable(result[1].substr(0, result[1].length()-1), locptr);
				 	
		int instructionSize = getRecord(result[0]).inst_size;
		locptr = locptr+ instructionSize;
					
    }
	
	return error_count;	
}

template<typename T, typename U>
void Assembler<T,U>::objectCodeGenerator(string filename)
{
	ifstream file(filename.c_str());
	string line="";
	while(file)
	{
		getline(file, line);
		if(line.size()>0)
		{   if(line=="END")
			break;
			vector<string>result = createTokens(line);
			vector<string>::iterator it;
			
			if(!(user_defined_symbol_table_object.getRecord(result[0].substr(0,result[0].size()-1))=="-1"))
				result.erase(result.begin());
			
			if(result[0]=="START")  
	  		cout<<"obj "<<timestamp()<<" "<<result[1]<<endl;
		  	else if(result[0]=="BYTE" )
			{   
				string t3="";	
				if(result[2][0]=='#')
					t3=bitset<4>(result[2][1]).to_string();
				else if(user_defined_symbol_table_object.getRecord(result[2])=="-1") // for variable
						t3 = register_table_object.getRecord(result[2]); // for register
				else
						t3 = user_defined_symbol_table_object.getRecord(result[2]);
				cout<<t3<<endl;
			}
			else if(result[0]=="JUMP")
					cout<<getRecord(result[0]).mac_code<<" "<<user_defined_symbol_table_object.getRecord(result[1])<<endl;
			else
				{
					string t1="", t2="";
					//1st 
					string temp = result[1].substr(0,result[1].size()-1);
					if(user_defined_symbol_table_object.getRecord(temp)=="-1")
						t1=register_table_object.getRecord(temp);
					else
						t1 = user_defined_symbol_table_object.getRecord(temp);
					// 2nd 
					if(result[2][0]=='#')
						 t2=bitset<4>(result[2][1]).to_string();
					else if(user_defined_symbol_table_object.getRecord(result[2])=="-1") // for variable
							t2 = register_table_object.getRecord(result[2]); // for register
					else
							t2 = user_defined_symbol_table_object.getRecord(result[2]);
					
					cout<<getRecord(result[0]).mac_code<<" "<<t1<<" "<<t2<<endl;		
				}	
		}
	}
}


int main()
{	
	Assembler<string, struct opcode>a;
	a.createOpcodeFile();
	a.createRegisterfile();
	a.readCode("input.txt");	
	return 0;		
}
