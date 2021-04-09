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
vector<int> previous_register_set(32, 0);
//checks which memory address is being accessed for the data
int data_counter= 0;
int rowBufferUpdates = 0;
int busyRegister = -1;
int busyMemory = -1;
int saveCycles = 0;

//checks that the instruction set does not overflow
int memory_program= 0;
vector<int> DRAM_memory(1048576, -2147483647);
//int DRAM_memory[1024][1024] = {-2147483647};

//The combined size of the instruction set and the sata set is 2^20
//bytes, so we will have to assign some fixed value to instruction
//set and rest to the data set
//stores the instruction set
vector<string> instruction_set;
//stores the data set after the instruction set in a hash table
//unordered_map<int, int> data_set;


//function to print the register contents after each instruction
void print_register(vector<int> a, vector<int> b){

  int returnValue1=0, returnValue2=0, returnValue3=0, returnValue4=0;
	for(int i=0; i< 10; i++){
    if(a[i] != b[i]){
		  cout << "$r" << i << " = " << int_to_hex(a[i])<<  endl;
      returnValue1 = 1;
    }

	}

	for(int i=10; i< 20; i++){
    if(a[i] != b[i]){
		  cout << "$s" << i-10 << " = " << int_to_hex(a[i])<<  endl;
      returnValue2 = 1;
    }

	}

	for(int i=20; i< 30; i++){
    if(a[i] != b[i]){
		  cout << "$t" << i-20 << " = " << int_to_hex(a[i])<<  endl;
      returnValue3 = 1;
    }

	}

	for(int i=30; i< 32; i++){
    if(a[i] != b[i]){
		  cout << "$v" << i-30 << " = " << int_to_hex(a[i])<<  endl;
      returnValue4 = 1;
    }

	}

  if(returnValue1 != 1 && returnValue2 != 1 && returnValue3 != 1 && returnValue4 != 1){
    cout<< "No change in value of register" << endl;
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
void print_memory(vector<int> DRAM_memory) {
  for(int i =0; i<1048576; i+=4){
    if(DRAM_memory[i] != -2147483647){
      cout<< i << "-" << i+3 << " : " << DRAM_memory[i] <<endl;
    }
  }
}

int main(int argc, char** argv){

  // string input;
	// cin >> input;

  int rowAccessDelay = atoi(argv[1]);
  int columnAccessDelay = atoi(argv[2]);
  string input = argv[3];
	//stores the current line in the input
	string line;
	//would help in convering string to int
	int numeral= 0;



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
  int a =0,b=0,c=0,d=0,e=0,f=0,g=0,h=0,i=0, j=0, k=0;

	//The number of instructions
	int instruction=0;
  int clockNumber =0;
  int currentRow = -1;
  int printCheck =-1;
	//executing the program
	while(i< instruction_set.size()){
    previous_register_set = register_set;
		string s= instruction_set[i];
    string s1 = s;

		//method to remove the whitespaces
		s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());

		//s now contains the current line after removing whitespace
		if(s==""){
			i=i+1;
			continue;
		}
    else if(s.substr(0,5)=="main:"){
			i=i+1;
			continue;
		}
    else if(s.substr(0,5)=="exit:"){
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

			if((s.substr(6,1)!="," || s.substr(10,1)!=",") && s.substr(7,5) != "$zero" && s.substr(12,5) != "$zero"){
				cout << "Invalid Syntax at line:"<<i+1 << endl;
				return 0;
			}

			if((map(s.substr(7,3))==100 || map(s.substr(3,3))==100 || map(s.substr(11,3))==100) && s.substr(7,5) != "$zero" && s.substr(12,5) != "$zero"){
				cout << "Invalid Syntax at line:"<<i+1 << endl;
				return 0;
			}

			//throw syntax error if no commas
			if(register_set[map(s.substr(7,3))]+ register_set[map(s.substr(11,3))] > 2147483647 || register_set[map(s.substr(7,3))]+ register_set[map(s.substr(11,3))] < -2147483647){
				cout << "Word limit exceeded" << endl;
				return 0;
			}
      if(s.substr(7,5) == "$zero" && s.substr(12,5) == "$zero"){
        register_set[map(s.substr(3,3))]= 0 + 0;
      }
      else if(s.substr(7,5) == "$zero"){
        register_set[map(s.substr(3,3))]= 0 + register_set[map(s.substr(13,3))];
      }
      else if(s.substr(12,5) == "$zero"){
        register_set[map(s.substr(3,3))]= register_set[map(s.substr(7,3))]+ 0;
      }
			else {
        register_set[map(s.substr(3,3))]= register_set[map(s.substr(7,3))]+ register_set[map(s.substr(11,3))];
      }
			i+=1;
			a+=1;

      if(saveCycles > 0 && map(s.substr(3,3))!= busyRegister && map(s.substr(7,3))!= busyRegister && map(s.substr(11,3))!= busyRegister ){
        saveCycles--;
      }
      else {clockNumber+=1; saveCycles =0;}
		}

		else if(s.substr(0,4)== "addi"){

			if((s.substr(7,1)!="," || s.substr(11,1)!=",") && s.substr(8,5) != "$zero"){
				cout << "Invalid Syntax at line:"<<i+1 << endl;
				return 0;
			}

			if((map(s.substr(4,3))==100 || map(s.substr(8,3))==100) && s.substr(8,5) != "$zero" ){
				cout << "Invalid Syntax at line:"<<i+1 << endl;
				return 0;
			}



      if(s.substr(8,5) == "$zero"){
        register_set[map(s.substr(4,3))]= 0 + str_to_int(s.substr(14,s.size()-14));
      }
			else {
        int helper=str_to_int(s.substr(12,s.size()-12));
  			//cout << str_to_int("a");
  			//cout << helper;
  			if(helper == 2147483647){
  				cout << "Invalid Syntax at line:"<<i+1 << endl;
  				return 0;
  			}
        register_set[map(s.substr(4,3))]= register_set[map(s.substr(8,3))] +helper;
      }


			i+=1;
			b+=1;
      if(saveCycles > 0 && map(s.substr(4,3))!= busyRegister && map(s.substr(8,3))!= busyRegister){
        saveCycles--;
      }
      else {clockNumber+=1; saveCycles =0;}
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
      if(saveCycles > 0 && map(s.substr(3,3))!= busyRegister && map(s.substr(7,3))!= busyRegister && map(s.substr(11,3)) != busyRegister){
        saveCycles--;
      }
      else {clockNumber+=1; saveCycles =0;}
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
      if(saveCycles > 0 && map(s.substr(3,3))!= busyRegister && map(s.substr(7,3))!= busyRegister && map(s.substr(11,3)) != busyRegister){
        saveCycles--;
      }
      else {clockNumber+=1; saveCycles =0;}
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
      clockNumber+=1;
      if(saveCycles > 0) {
        saveCycles--;
      }
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
      if(saveCycles > 0 && map(s.substr(3,3))!= busyRegister && map(s.substr(7,3))!= busyRegister){
        saveCycles--;
      }
      else {clockNumber+=1; saveCycles =0;}
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
      if(saveCycles > 0 && map(s.substr(3,3))!= busyRegister && map(s.substr(7,3))!= busyRegister){
        saveCycles--;
      }
      else {clockNumber+=1; saveCycles =0;}
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
      clockNumber+=1;
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
      int memoryLocation = (str_to_int(s.substr(6, s.size()-6)));
			if(DRAM_memory[memoryLocation]== -2147483647){
				cout << "Trying to access uninitialised memory at line:" <<i+1 << endl;
				return 0;
			}
      else if(memoryLocation%4 != 0){
        cout<< "Memory location not accesible at line: " << i+1 <<endl;
        return 0;
      }
			else{
				register_set[map(s.substr(2,3))]=DRAM_memory[memoryLocation];
        busyRegister = map(s.substr(2,3));
			}
			i+=1;
			j+=1;
      clockNumber+=1;

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
      int memoryLocation = str_to_int(s.substr(6, s.size()-6));
			if(memoryLocation>1048576){
				cout << " Data Memory overflow at line:" << i+1 << endl;
				return 0;
			}
      else if(memoryLocation%4 != 0){
        cout<< "Memory location not accesible at line: " << i+1 <<endl;
        return 0;
      }
			else{
				//data_set.insert(make_pair(memoryLocation, register_set[map(s.substr(2,3))]));
         DRAM_memory[memoryLocation] = register_set[map(s.substr(2,3))];
         busyMemory = memoryLocation;
			}
			i+=1;
			k+=1;
      clockNumber+=1;
		}

		else{
			cout << "Invalid Syntax at line:"<<i+1 << endl;
			return 0;
		}

    instruction+=1;

    // if(s.substr(0,2)== "sw" || s.substr(0,2)== "lw") {
    //   cout << "Cycle " << clockNumber << ": " << "DRAM request issued" <<endl;
    // }
		//IGNORE
		//cout << instruction_set[i] << endl;
		//cout << s << endl;
		//cout << (s.substr(0,3)=="add") << endl;
    if (printCheck == -1) {
      cout<< "\nCycle description: \n" << endl;
      printCheck++;
    }
		if(s.substr(0,2)== "sw"){
      cout << "Cycle " << clockNumber << ": " << "DRAM request issued" <<endl;
      int memoryLocation = str_to_int(s.substr(6, s.size()-6));
      int prevClock = clockNumber+1;
      if(currentRow == -1){
        currentRow = memoryLocation/1024;
        rowBufferUpdates += 1;
        clockNumber+= rowAccessDelay;
        clockNumber+= columnAccessDelay;
        saveCycles = rowAccessDelay + columnAccessDelay;
      }
      else if(currentRow == memoryLocation/1024){

        clockNumber+= columnAccessDelay;
        saveCycles = columnAccessDelay;
      }
      else if(currentRow != memoryLocation/1024){
        currentRow = memoryLocation/1024;
        rowBufferUpdates += 1;
        clockNumber+= rowAccessDelay;
        clockNumber+= rowAccessDelay;
        clockNumber+= columnAccessDelay;
        saveCycles = 2*rowAccessDelay + columnAccessDelay;
      }
      else{
        cout << "Something went wrong";
        return 0;
      }

      cout << "Cycle " << prevClock<< "-" << clockNumber << ": ";
      cout<< "Memory address from " << memoryLocation << "-" << memoryLocation+3 << " = " << DRAM_memory[memoryLocation] <<endl;

    }

    else if(s.substr(0,2)== "lw"){
      cout << "Cycle " << clockNumber << ": " << "DRAM request issued" <<endl;
      int prevClock = clockNumber+1;
      int memoryLocation = str_to_int(s.substr(6, s.size()-6));
      if(currentRow == -1){
        cout<< "Uninitilised value is being accessed";
      }
      else if(currentRow == memoryLocation/1024){
        clockNumber+= columnAccessDelay;
        saveCycles = columnAccessDelay;
      }
      else if(currentRow != memoryLocation/1024){
        currentRow = memoryLocation/1024;
        rowBufferUpdates += 1;
        clockNumber+= rowAccessDelay;
        clockNumber+= rowAccessDelay;
        clockNumber+= columnAccessDelay;
        saveCycles = 2*rowAccessDelay + columnAccessDelay;
      }
      else {cout << "Something went wrong";}

      cout << "Cycle " << prevClock<< "-" << clockNumber << ": ";
       print_register(register_set, previous_register_set);
    }


		else {
      cout << "Cycle " << clockNumber-saveCycles << ": ";
      print_register(register_set, previous_register_set);
    }

	}

  cout<< "\nMemory content at the end of execution: \n"<<endl;
  print_memory(DRAM_memory);
  cout<< "\n";
	cout << "Total number of the instructions: " << instruction <<endl;
  cout<< "Total number of clock cycles: " << clockNumber << endl;
  cout<< "Total number of row buffer updates/row activations: " << rowBufferUpdates << endl;

  cout<< "Row access delay considered to be: " << rowAccessDelay<<endl;
  cout<< "Column access delay considered to be: " << columnAccessDelay<< endl;

}
