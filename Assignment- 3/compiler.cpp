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
	for(int i=0; i<32; i++){
		if(input== ("$r" + to_string(i))){
		return i;

		}
	}

	return 100;

}

int str_to_int(string s){
	stringstream num(s);
	int helper= 0;
	num >> helper;
	return helper;
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

	//string k;
	//cin >> k;

	//storing all the commands in a vector
	ifstream myfile("rand.txt");
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
	//To help us when to end loop
	int i=0;

	//The number of instructions 
	int instruction=0;

	//executing the program
	while(i< instruction_set.size()){
		string s= instruction_set[i];
		//method to remove the whitespaces
		s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());

		//s now contains the current line after removing whitespace


		if(s.substr(0,3)=="add" && s.substr(0,4)!= "addi"){

			//throw syntax error if no commas
			register_set[map(s.substr(3,3))]= register_set[map(s.substr(7,3))]+ register_set[map(s.substr(11,3))];
			i+=1;
		}

		if(s.substr(0,4)== "addi"){

			int helper=str_to_int(s.substr(12,s.size()-12));
			register_set[map(s.substr(4,3))]= register_set[map(s.substr(8,3))] +helper;
			i+=1;
		}

		if(s.substr(0,3)=="sub"){

			register_set[map(s.substr(3,3))]= register_set[map(s.substr(7,3))]- register_set[map(s.substr(11,3))];
			i+=1;
		}
		if(s.substr(0,3)=="mul"){

			register_set[map(s.substr(3,3))]= register_set[map(s.substr(7,3))]* register_set[map(s.substr(11,3))];

			i+=1;
		}

		if(s.substr(0,1)=="j"){
			//Should I print if this comes or not, will be chill
			int helper= str_to_int(s.substr(1,1));
			i= helper-1;
		}

		if(s.substr(0,3)== "beq"){
			if(register_set[map(s.substr(3,3))]== register_set[map(s.substr(7,3))]){
				int helper= str_to_int(s.substr(11,1));
				i= helper-1;
			}
		}

		if(s.substr(0,3)== "bne"){
			if(register_set[map(s.substr(3,3))]!= register_set[map(s.substr(7,3))]){
				int helper= str_to_int(s.substr(11,1));
				i= helper-1;
			}
		}

		if(s.substr(0,3)== "slt"){
			if(register_set[map(s.substr(7,3))]< register_set[map(s.substr(11,3))]){
				register_set[map(s.substr(3,3))]= 1;
			}
			else{
				register_set[map(s.substr(3,3))]= 0;
			}
			i= i+1;
		}

		if(s.substr(0,2)== "lw"){
			register_set[map(s.substr(2,3))]= data_set[str_to_int(s.substr(6, s.size()-6))];
			i+=1;
		}

		if(s.substr(0,2)== "sw"){
			if(str_to_int(s.substr(6, s.size()-6))>237144){
				cout << "Memory overflow";
				return 0;
			}
			else{
				data_set[str_to_int(s.substr(6, s.size()-6))]= register_set[map(s.substr(2,3))];
			}
			i+=1;
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

}
