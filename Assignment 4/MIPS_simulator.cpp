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
#include "functions.h"
using namespace std;

// template< typename T >


vector<int> register_set(32, 0);
vector<int> previous_register_set(32, 0);

int data_counter= 0;
int rowBufferUpdates = 0;
int busyRegister = -1;
int busyMemory = -1;
int saveCycles = 0;
int rowAccessDelay;
int columnAccessDelay;
int clockNumber;
int currentRow;
int instruction;

vector<int> busyRegisters;
vector<int> busyMemories;
vector<int> rows;
int memory_program= 0;
vector<int> DRAM_memory(1048576, -2147483647);
vector<string> instruction_set;
vector<int> numbers;


void findNextRequests(int &i){

  for(int j=i; j<instruction_set.size()+i;j++){
    string s= instruction_set[j];
    s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());
    if(s.substr(0,2)== "sw"|| s.substr(0,2)== "lw"){

      int memoryLocation = str_to_int(s.substr(6, s.size()-6));
      int busyRegister = map(s.substr(2,3));


      if(find(busyMemories.begin(), busyMemories.end(), memoryLocation) != busyMemories.end()) {
          break;
      }
      else if(find(busyRegisters.begin(), busyRegisters.end(), busyRegister) != busyRegisters.end()) {
        break;
      }
      else {
          /* v does not contain x */
          busyRegisters.push_back(busyRegister);
          busyMemories.push_back(memoryLocation);
          rows.push_back(memoryLocation/1024);
          numbers.push_back(j);
      }
    }
    else break;
  }
  i = i+ busyRegisters.size();
  instruction = instruction + busyRegisters.size();
}

int efficientProcess(int &currentRow, int &i, vector<int> &busyRegisters, vector<int> &busyMemories, vector<int> &rows){
  //rewrite
  //convert them into hash sets
  for(int j=0; j<busyRegisters.size(); j++){
    string s = instruction_set[numbers[j]];
    s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());
    if(currentRow == rows[j]){
      if(s.substr(0,2)== "sw"){
        int a = sw(s,clockNumber,saveCycles,numbers[j],columnAccessDelay,rowAccessDelay,currentRow,busyRegister, register_set, previous_register_set, busyMemory, rowBufferUpdates, DRAM_memory);
        if(a==0){return 0;}
      }
      else if(s.substr(0,2)== "lw"){
        int a = lw(s,clockNumber,saveCycles,numbers[j],columnAccessDelay,rowAccessDelay,currentRow,busyRegister, register_set, previous_register_set, rowBufferUpdates, DRAM_memory);
          if(a==0){return 0;}
      }
      busyRegisters.erase(busyRegisters.begin() + j);
      busyMemories.erase(busyMemories.begin() + j);
      rows.erase(rows.begin() + j);
      numbers.erase(numbers.begin() + j);
    }
  }


    while(busyRegisters.size() !=0){
      string s = instruction_set[numbers[0]];
      s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());
      if(s.substr(0,2)== "sw"){
        int a = sw(s,clockNumber,saveCycles,numbers[0], columnAccessDelay,rowAccessDelay,currentRow,busyRegister, register_set, previous_register_set, busyMemory, rowBufferUpdates, DRAM_memory);
        if(a==0){return 0;}
        busyRegisters.erase(busyRegisters.begin() + 0);
        busyMemories.erase(busyMemories.begin() + 0);
        rows.erase(rows.begin() + 0);
        numbers.erase(numbers.begin() + 0);
      }
      else if(s.substr(0,2)== "lw"){
        int a = lw(s,clockNumber,saveCycles,numbers[0], columnAccessDelay,rowAccessDelay,currentRow,busyRegister, register_set, previous_register_set, rowBufferUpdates, DRAM_memory);
          if(a==0){return 0;}

          busyRegisters.erase(busyRegisters.begin() + 0);
          busyMemories.erase(busyMemories.begin() + 0);
          rows.erase(rows.begin() + 0);
          numbers.erase(numbers.begin() + 0);
    }
      efficientProcess(currentRow, i, busyRegisters, busyMemories, rows);
    }

    return 1;
    busyRegisters.clear();
    busyMemories.clear();
    rows.clear();
    numbers.clear();
}




int main(int argc, char** argv){

  rowAccessDelay = atoi(argv[1]);
  columnAccessDelay = atoi(argv[2]);
  string input = argv[3];
	//stores the current line in the input
	string line;


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

  int i=0;

	//The number of instructions
 instruction=0;
  clockNumber =0;
  currentRow = -1;
  int printCheck =-1;


	while(i< instruction_set.size()){
        previous_register_set = register_set;
    		string s= instruction_set[i];
        string s1 = s;

    		//method to remove the whitespaces
    		s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());

        if (printCheck == -1) {
          cout<< "\nCycle description: \n" << endl;
          printCheck++;
        }
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
          int a = add(s,clockNumber,saveCycles,i, busyRegister,register_set);
          if(a==0){return 0;}
        }

    		else if(s.substr(0,4)== "addi"){
          int a = addi(s,clockNumber,saveCycles,i, busyRegister,register_set);
          if(a==0){return 0;}
    		}

    		else if(s.substr(0,3)=="sub"){
          int a = sub(s,clockNumber,saveCycles,i, busyRegister,register_set);
          if(a==0){return 0;}
    		}

    		else if(s.substr(0,3)=="mul"){
          int a = mul(s,clockNumber,saveCycles,i, busyRegister,register_set);
          if(a==0){return 0;}
    		}

    		else if(s.substr(0,1)=="j"){
          int a = j(i, s, clockNumber, saveCycles);
          if(a==0){return 0;}

    		}

    		else if(s.substr(0,3)== "beq"){
          int a = beq(s,clockNumber,saveCycles,i, busyRegister,register_set);
            if(a==0){return 0;}
    		}

    		else if(s.substr(0,3)== "bne"){
          int a = bne(s,clockNumber,saveCycles,i, busyRegister,register_set);
            if(a==0){return 0;}
    		}

    		else if(s.substr(0,3)== "slt"){
          int a = slt(s,clockNumber,saveCycles,i, busyRegister,register_set);
            if(a==0){return 0;}
    		}

    		else if(s.substr(0,2)== "lw"){
          findNextRequests(i);

          int a = efficientProcess(currentRow,i, busyRegisters, busyMemories,rows);
            if(a==0){return 0;}
    		}

    		else if(s.substr(0,2)== "sw"){
          findNextRequests(i);

          int a = efficientProcess(currentRow,i, busyRegisters, busyMemories,rows);
            if(a==0){return 0;}
    		}

    		else{
    			cout << "Invalid Syntax at line:"<<i+1 << endl;
    			return 0;
    		}

        instruction+=1;

        if(s.substr(0,2)!= "sw" && s.substr(0,2)!= "lw") {
          cout << "Cycle " << clockNumber-saveCycles << ": ";
          print_register(register_set, previous_register_set);
        }

//End of while loop
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
