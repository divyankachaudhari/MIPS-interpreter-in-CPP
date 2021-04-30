#include <iostream>
#include <fstream>
#include <sstream>  
#include <string> 
#include <string.h>  
#include <vector>
#include <iomanip>
#include <cctype>
#include <algorithm>
#include <unordered_map>
using namespace std;

//The registers that we will be using are zero. r0- r31

//template to convert an integer to an hexadecimal string
template< typename T >
string int_to_hex( T i )
{
  stringstream stream;
  stream << setfill ('0') << setw(8) 
         << hex << i;
  return stream.str();
}

vector<int> register_set(32, 0);

//checks which memory address is being accessed for the data
int data_counter= 0;

//checks that the instruction set does not overflow
int memory_program= 0;


//The combined size of the instruction set and the sata set is 2^20
//bytes, so we will have to assign some fixed value to instruction
//set and rest to the data set
//stores the instruction set
vector<string> instruction_set;
//stores the data set after the instruction set in a hash table
unordered_map<int, int> data_set;


//function to print the register contents after each instruction
void print_register(vector<int> a){
	

	for(int i=0; i< 10; i++){
		cout << "r" << i << " " << int_to_hex(a[i])<<  endl;

	}

	for(int i=10; i< 20; i++){
		cout << "s" << i-10 << " " << int_to_hex(a[i])<<  endl;

	}

	for(int i=20; i< 30; i++){
		cout << "t" << i-20 << " " << int_to_hex(a[i])<<  endl;

	}

	for(int i=30; i< 32; i++){
		cout << "v" << i-30 << " " << int_to_hex(a[i])<<  endl;

	}


}

//maps the current register to the corresponding integer
int map(string input){
	//note that we cant use switch statement with string 
	//in C++ because it is not a supported type by the C++ compiler
	int flag=0;
	for(int i=0; i<10; i++){
		if(input== ("$r" + to_string(i))){
			flag=1;
			return i;

		}
	}

	for(int i=0; i<10; i++){
		if(input== ("$s" + to_string(i))){
			flag=1;
			return i+10;

		}
	}

	for(int i=0; i<10; i++){
		if(input== ("$t" + to_string(i))){
			flag=1;
			return i+20;

		}
	}

	for(int i=0; i<2; i++){
		if(input== ("$v" + to_string(i))){
			flag=1;
			return i+30;

		}
	}





	if(flag==0){
		return 100;
	}

	return 100;

}

int str_to_int(string s){
	for(int i=0; i<s.size(); i++){
		if( s.substr(i,1)!="-" && s.substr(i,1)!="0" && s.substr(i,1)!="1" && s.substr(i,1)!="2" && s.substr(i,1)!="3" && s.substr(i,1)!="4" && 
			s.substr(i,1)!="5" && s.substr(i,1)!="6" && s.substr(i,1)!="7" && s.substr(i,1)!="8" && s.substr(i,1)!="9"){
			return 2147483647;
		}
	}

	stringstream num(s);
	int helper= 0;
	num >> helper;

	return helper;
}

//Questions:
//How to account for comments that would exist in the code?
//not that 8($t0) stores accesses the memory address lecated at $t0+8


int main(){
	//stores the current line in the input
	string line;
	//would help in convering string to int
	int numeral= 0;

	string input;
	cin >> input;

	//storing all the commands in a vector
	ifstream myfile(input);
	while(getline(myfile, line)){
		if(memory_program<100000){
			instruction_set.push_back(line);
			memory_program+=4;
		}
		else{
			cout << "Instruction Memory Overflow" <<  endl;
			return 0;
		}
	}

	//executing the commands, remember that we need to remove the 
	//whitespaces
	//To help us when to end loop
	int i=0;
	int a= 0;
	int b= 0;
	int c= 0;
	int d= 0;
	int e= 0;
	int f= 0;
	int g= 0;
	int h= 0;
	int j= 0;
	int k= 0;


	//The number of instructions 
	int instruction=0;

	//executing the program
	while(i< instruction_set.size()){
		string s= instruction_set[i];
		//method to remove the whitespaces
		s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());

		//s now contains the current line after removing whitespace
		if(s==""){
			i=i+1;
			continue;
		}

		else if(s.substr(0,1)== "#"){
			i=i+1;
			continue;
		}

		else if(s.size()<2){
			cout << "Invalid Syntax at line:" << i+1<< endl;
			return 0;

		}

		else if(s.substr(0,1)!="j" && s.size()<7){
			cout << "Invalid Syntax at line:"<<i+1 << endl;
			return 0;
		}

		else if(s.substr(0,3)=="add" && s.substr(0,4)!= "addi"){

			if(s.substr(6,1)!="," || s.substr(10,1)!=","){
				cout << "Invalid Syntax at line:"<<i+1 << endl;
				return 0;
			}

			if(map(s.substr(7,3))==100 || map(s.substr(3,3))==100 || map(s.substr(11,3))==100){
				cout << "Invalid Syntax at line:"<<i+1 << endl;
				return 0;
			}

			//throw syntax error if no commas
			if(register_set[map(s.substr(7,3))]+ register_set[map(s.substr(11,3))] > 2147483647 || register_set[map(s.substr(7,3))]+ register_set[map(s.substr(11,3))] < -2147483647){
				cout << "Word limit exceeded" << endl;
				return 0;
			}
			register_set[map(s.substr(3,3))]= register_set[map(s.substr(7,3))]+ register_set[map(s.substr(11,3))];
			i+=1;
			a+=1;
		}

		else if(s.substr(0,4)== "addi"){

			if(s.substr(7,1)!="," || s.substr(11,1)!=","){
				cout << "Invalid Syntax at line:"<<i+1 << endl;
				return 0;
			}

			if(map(s.substr(4,3))==100 || map(s.substr(8,3))==100 ){
				cout << "Invalid Syntax at line:"<<i+1 << endl;
				return 0;
			}

			int helper=str_to_int(s.substr(12,s.size()-12));
			//cout << str_to_int("a");
			//cout << helper;
			if(helper == 2147483647){
				cout << "Invalid Syntax at line:"<<i+1 << endl;
				return 0;
			}
			register_set[map(s.substr(4,3))]= register_set[map(s.substr(8,3))] +helper;
			i+=1;
			b+=1;
		}

		else if(s.substr(0,3)=="sub"){

			if(s.substr(6,1)!="," || s.substr(10,1)!=","){
				cout << "Invalid Syntax at line:"<<i+1 << endl;
				return 0;
			}

			if(map(s.substr(7,3))==100 || map(s.substr(3,3))==100 || map(s.substr(11,3))==100){
				cout << "Invalid Syntax at line:"<<i+1 << endl;
				return 0;
			}

			if(register_set[map(s.substr(7,3))]- register_set[map(s.substr(11,3))] > 2147483647 || register_set[map(s.substr(7,3))]- register_set[map(s.substr(11,3))] < -2147483647){
				cout << "Word limit exceeded" << endl;
				return 0;
			}
			register_set[map(s.substr(3,3))]= register_set[map(s.substr(7,3))]- register_set[map(s.substr(11,3))];
			i+=1;
			c+=1;
		}
		else if(s.substr(0,3)=="mul"){

			if(s.substr(6,1)!="," || s.substr(10,1)!=","){
				cout << "Invalid Syntax at line:"<<i+1 << endl;
				return 0;
			}

			if(map(s.substr(7,3))==100 || map(s.substr(3,3))==100 || map(s.substr(11,3))==100){
				cout << "Invalid Syntax at line:"<<i+1 << endl;
				return 0;
			}

			register_set[map(s.substr(3,3))]= register_set[map(s.substr(7,3))]* register_set[map(s.substr(11,3))];

			i+=1;
			d+=1;
		}

		else if(s.substr(0,1)=="j"){
			//Should I print if this comes or not, will be chill
			int helper= str_to_int(s.substr(1,s.size()-1));
			if(helper == 2147483647){
				cout << "Invalid Syntax at line:"<<i+1 << endl;
				return 0;
			}

			i= helper-1;
			e+=1;
		}

		else if(s.substr(0,3)== "beq"){

			if(s.substr(6,1)!="," || s.substr(10,1)!=","){
				cout << "Invalid Syntax at line:"<<i+1 << endl;
				return 0;
			}

			if(map(s.substr(7,3))==100 || map(s.substr(3,3))==100){
				cout << "Invalid Syntax at line:"<<i+1 << endl;
				return 0;
			}

			if(register_set[map(s.substr(3,3))]== register_set[map(s.substr(7,3))]){
				int helper= str_to_int(s.substr(11,s.size()-11));
				if(helper == 2147483647){
					cout << "Invalid Syntax at line:"<<i+1 << endl;
					return 0;
				}
				i= helper-1;
			}
			f+=1;
		}

		else if(s.substr(0,3)== "bne"){

			if(s.substr(6,1)!="," || s.substr(10,1)!=","){
				cout << "Invalid Syntax at line:"<<i+1 << endl;
				return 0;
			}

			if(map(s.substr(7,3))==100 || map(s.substr(3,3))==100){
				cout << "Invalid Syntax at line:"<<i+1 << endl;
				return 0;
			}

			if(register_set[map(s.substr(3,3))]!= register_set[map(s.substr(7,3))]){
				int helper= str_to_int(s.substr(11,s.size()-11));
				if(helper == 2147483647){
					cout << "Invalid Syntax at line:"<<i+1 << endl;
					return 0;
				}
				i= helper-1;
			}
			g+=1;
		}

		else if(s.substr(0,3)== "slt"){

			if(s.substr(6,1)!="," || s.substr(10,1)!=","){
				cout << "Invalid Syntax at line:"<<i+1 << endl;
				return 0;
			}

			if(map(s.substr(7,3))==100 || map(s.substr(3,3))==100 || map(s.substr(11,3))==100){
				cout << "Invalid Syntax at line:"<<i+1 << endl;
				return 0;
			}


			if(register_set[map(s.substr(7,3))]< register_set[map(s.substr(11,3))]){
				register_set[map(s.substr(3,3))]= 1;
			}
			else{
				register_set[map(s.substr(3,3))]= 0;
			}
			i= i+1;
			h+=1;
		}

		else if(s.substr(0,2)== "lw"){

			if(s.substr(5,1)!=","){
				cout << "Invalid Syntax at line:"<<i+1 << endl;
				return 0;
			}

			if(map(s.substr(2,3))==100){
				cout << "Invalid Syntax at line:"<<i+1 << endl;
				return 0;
			}
			int helper=str_to_int(s.substr(6, s.size()-6));
			if(helper == 2147483647){
					cout << "Invalid Syntax at line:"<<i+1 << endl;
					return 0;
			}

			if(data_set.find(str_to_int(s.substr(6, s.size()-6)))== data_set.end()){
				cout << "Trying to access uninitialised memory at line:" <<i+1 << endl;
				return 0;
			}
			else{
				register_set[map(s.substr(2,3))]=data_set.at(str_to_int(s.substr(6, s.size()-6)));

			}
			i+=1;
			j+=1;

		}

		else if(s.substr(0,2)== "sw"){

			if(s.substr(5,1)!=","){
				cout << "Invalid Syntax at line:"<<i+1 << endl;
				return 0;
			}

			if(map(s.substr(2,3))==100){
				cout << "Invalid Syntax at line:"<<i+1 << endl;
				return 0;
			}

			int helper=str_to_int(s.substr(6, s.size()-6));
			if(helper == 2147483647){
					cout << "Invalid Syntax at line:"<<i+1 << endl;
					return 0;
			}

			if(str_to_int(s.substr(6, s.size()-6))>237144){
				cout << " Data Memory overflow at line:" << i+1 << endl;
				return 0;
			}
			else{
				data_set.insert(make_pair(str_to_int(s.substr(6, s.size()-6)), register_set[map(s.substr(2,3))]));
			}
			i+=1;
			k+=1;
		}
		else{
			cout << "Invalid Syntax at line:"<<i+1 << endl;
			return 0;
		}



		//IGNORE
		//cout << instruction_set[i] << endl;
		//cout << s << endl;
		//cout << (s.substr(0,3)=="add") << endl;
		instruction+=1;
		cout << "Instruction:" << instruction <<endl;
		print_register(register_set) ;
		cout << "\n";
	}

	cout << "The no. of the clock cycles is:" << instruction <<endl;
	cout << "No. of add:" << a <<endl;
	cout << "No. of addi:" << b <<endl;
	cout << "No. of sub:" << c <<endl;
	cout << "No. of mul:" << d <<endl;
	cout << "No. of j:" << e <<endl;
	cout << "No. of beq:" << f <<endl;
	cout << "No. of bne:" << g <<endl;
	cout << "No. of slt:" << h <<endl;
	cout << "No. of lw:" << j <<endl;
	cout << "No. of sw:" << k <<endl;

}
