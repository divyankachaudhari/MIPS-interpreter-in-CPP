#include <iostream>
#include <fstream>
#include <sstream>  
#include <string>  
#include <vector>
#include <iomanip>
#include <bits/stdc++.h>
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

//function to print the register contents after each function
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
//What the hell do we mean by memory in the mips system

int main(){
	//stores the current line in the input
	string line;
	//would help in convering string to int
	int numeral= 0;

	string k;
	cin >> k;

	instream myfile(k);
	while(getline(myfile, line)){
		
		

	}

}
