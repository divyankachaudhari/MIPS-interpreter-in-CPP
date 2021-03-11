#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <bits/stdc++.h>
using namespace std;

//template to convert an integer to an hexadecimal string
template< typename T >
string int_to_hex( T i )
{
  stringstream stream;
  stream << "0x" 
         << setfill ('0') << setw(8) 
         << hex << i;
  return stream.str();
}

vector<int> register(32, 0);

//function to print the register contents after each function
void print_register(vector<int> a){
	
	for(int i=0; i< a.size(); i++){
		cout << "R" << i << " " << int_to_hex(a[i])<<  endl;

	}


}

int map(string input){
	//note that we cant use switch statement with string 
	//in C++ because it is not a supported type by the C++ compiler

	//hardcode all the registers to the corresponding register number
	if(input== "$zero"){
		return 0;
	}
	if(input== "at"){
		return 1;
	}

	if(input== )
}

//Questions:
//How to account for comments that would exist in the code?
//What the hell do we mean by memory in the mips system

int main(){
	//stores the current line in the input
	string line;
	//would help in convering string to int
	int numeral= 0;

	//Defines what operations need to be performed on the current line
	string current_state;

	string k;
	cin >> k;

	instream myfile(k);
	while(getline(myfile, line)){
		if(line== ".data"){
			//check if this if condition works because im not sure
			//check the contents of the newlines after ignoring the tabs
			current_state
		}
		else if(line== ".text"){

		}
		else if(line== ".globl main"){

		}
		else if(line== ".ent main"){

		}
		else if(line== ".main"){

		}
		else if(line== ".end main"){

		}

		else {continue;}

	}

}
