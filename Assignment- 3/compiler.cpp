#include <iostream>
#include <fstream>
#include <sstream>  
#include <string> 
#include <string.h>  
#include <vector>
#include <iomanip>
#include <cctype>
#include <algorithm>
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

vector<int> register(32, 0);
//checks which instruction the code is currently running
prog_counter= 1;

//to check that the instruction set does not overflow
memory_program= 0;

//checks which memory address is being accessed for the data
data_counter= 0;

//to check that the memory set does not overflow
memory_data= 0;

//The combined size of the instruction set and the sata set is 2^20
//bytes, so we will have to assign some fixed value to instruction
//set and rest to the data set
//stores the instruction set
vector<string> instruction_set;
//stores the data set after the instruction set
vector<int> data_set;

//function to print the register contents after each instruction
void print_register(vector<int> a){
	
	for(int i=0; i< a.size(); i++){
		cout << "r" << i << " " << int_to_hex(a[i])<<  endl;

	}


}


//maps the current register to the corresponding integer
int map(string input){
	//note that we cant use switch statement with string 
	//in C++ because it is not a supported type by the C++ compiler

	if(input== "$zero"){
		return 0;
	}
	for(int i=0; i<31; i++){
		if(input== ("$r" + to_string(i))){
			return i+1;
		}
	}
}

//Questions:
//How to account for comments that would exist in the code?
//not that 8($t0) stores accesses the memory address lecated at $t0+8
//

int main(){
	//stores the current line in the input
	string line;
	//would help in convering string to int
	int numeral= 0;

	string k;
	cin >> k;

	//storing all the commands in a vector
	instream myfile(k);
	while(getline(myfile, line)){
		if(memory_program<100000){
			instruction_set.push_back(line);
			memory_program+=4;
		}
		else{
			cout << "Memory Overflow" <<  endl;
			return 0;
		}
	}

	//executing the commands, remember that we need to remove the 
	//whitespaces
	for(int i=0; i< instruction_set.size(); i++){
		string s= instruction_set[i];
		//method to remove the whitespaces
		s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());

		//s now contains the current line after removing whitespace

		if(s.substr(0,3)=="add"){
			//throw syntax error if no commas
			register[map(s.substr(3,3))]= register[map(s.substr(7,3))]+ register[map(s.substr(11,3))];
		}
		print_register(register);
	}

}
