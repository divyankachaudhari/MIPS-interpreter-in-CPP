///////////////////////////////////////////////////////////////////////////////
/*
Add [DONE] infront of done

1. Change register names [DONE]
2. Change the labels for jump, etc. [DONE] 
3. Change offsets [DONE]
4. For empty line and comment, DON'T BREAK findNextRequests
5. Check $zero, etc in instructions other than addi
6. Check bne, beq which runs infinite loop [DONE]
7. Output correct number of instructions [DONE]




*////////////////////////////////////////////////////////////////////////////////

//CONTAINS THE DIRECT FILE FOR FALLBA


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
unordered_map<string, int> jumpMap;

void findNextRequests(int &i){

  for(int j=i; j<instruction_set.size()+i;j++){
    string s= instruction_set[j];
    s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());
    if(s.substr(0,2)== "sw"|| s.substr(0,2)== "lw"){

      int k=0;
        for(int i= 0; i< s.size(); i++){
          if(s.substr(i,1)=="("){
            k= i;
            break;
          }
        }
      int helper=str_to_int(s.substr(6, k-6));
        if(helper == 2147483647){
            break;
        }

        if(s.substr(s.size()-1, 1)!= ")"){
          break;
        }

      int memoryLocation;
        if(s.substr(k+1, 5)== "$zero"){
          memoryLocation = str_to_int(s.substr(6, k-6));
        }
        else{
          memoryLocation = str_to_int(s.substr(6, k-6))+ register_set[map(s.substr(k+1, 3))];
        }
      //cout << memoryLocation << endl;

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
    else { break;}
  }
  //cout << i << endl;
  i = i+ busyRegisters.size();
  //cout << busyRegisters.size() << endl;

}

int efficientProcess(int &currentRow, int &i, vector<int> &busyRegisters, vector<int> &busyMemories, vector<int> &rows, vector<int> &numbers){
  vector<int> deletej;
  //rewrite
  //convert them into hash sets
  //cout << "The current row: " << to_string(currentRow) << endl;
  for(int j=0; j<busyRegisters.size(); j++){
    string s = instruction_set[numbers[j]];
    s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());

    if(currentRow == rows[j]){
      //cout << currentRow << endl;
      deletej.push_back(j);
      //cout<< "Pushing in the for loop into deletej: "<< j <<endl;
      if(s.substr(0,2)== "sw"){
        int a = sw(s,clockNumber,saveCycles,numbers[j],columnAccessDelay,rowAccessDelay,currentRow,busyRegister, register_set, previous_register_set, busyMemory, rowBufferUpdates, DRAM_memory);

        if(a==0){return 0;}
        instruction++;
      }
      else if(s.substr(0,2)== "lw"){
        int a = lw(s,clockNumber,saveCycles,numbers[j],columnAccessDelay,rowAccessDelay,currentRow,busyRegister, register_set, previous_register_set, rowBufferUpdates, DRAM_memory);
          if(a==0){return 0;}
          instruction++;
      }

    }
  }

  //cout<< "printing contents of deletej"<< endl;
  // for(int j=0; j<deletej.size();j++){
  //   cout<< deletej[j] << endl;
  //
  // }
  //cout <<"enetrring delete  loop"<< endl;
  for(int j=0; j<deletej.size();j++){
    //cout<< j<< " " << deletej[j] << endl;
    busyRegisters.erase(busyRegisters.begin() + deletej[j]-j);
    //cout<< "deleted busy resgiter" << endl;
    busyMemories.erase(busyMemories.begin() +  deletej[j]-j);
    rows.erase(rows.begin() +  deletej[j]-j);
    numbers.erase(numbers.begin() +  deletej[j]-j);
    //cout<< j<< " completed" << endl;
  }
  //cout<<"exiting dlete loop"<<endl;
  deletej.clear();

    while(busyRegisters.size() !=0){
      //cout<< "Entering while loop" << endl;
      string s = instruction_set[numbers[0]];
      s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());
      if(s.substr(0,2)== "sw"){
        int a = sw(s,clockNumber,saveCycles,numbers[0], columnAccessDelay,rowAccessDelay,currentRow,busyRegister, register_set, previous_register_set, busyMemory, rowBufferUpdates, DRAM_memory);
        if(a==0){return 0;}
        instruction++;
        busyRegisters.erase(busyRegisters.begin() + 0);
        busyMemories.erase(busyMemories.begin() + 0);
        //cout<< busyMemories[0] << endl;;
        rows.erase(rows.begin() + 0);
        numbers.erase(numbers.begin() + 0);
      }
      else if(s.substr(0,2)== "lw"){
        int a = lw(s,clockNumber,saveCycles,numbers[0], columnAccessDelay,rowAccessDelay,currentRow,busyRegister, register_set, previous_register_set, rowBufferUpdates, DRAM_memory);
        instruction++;
          if(a==0){return 0;}

          busyRegisters.erase(busyRegisters.begin() + 0);
          busyMemories.erase(busyMemories.begin() + 0);
          rows.erase(rows.begin() + 0);
          numbers.erase(numbers.begin() + 0);
    }
      efficientProcess(currentRow, i, busyRegisters, busyMemories, rows, numbers);
    }

    busyRegisters.clear();
    busyMemories.clear();
    rows.clear();
    numbers.clear();
    return 1;
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

  //Looping through the code and storing the name of all the labels
  int rand= 0;

  while(rand<instruction_set.size()){
    string s= instruction_set[rand];
    //method to remove the whitespaces
    s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());

    if(s==""){
          rand=rand+1;
          continue;
    }

    else if(s.substr(s.length()-1,1) == ":"){
        //jumpMap[s.substr(0, s.length()-1)] = rand;
        jumpMap.insert(make_pair(s.substr(0, s.length()-1), rand+1));
        //cout << s.substr(0, s.length()-1) << endl;
        //cout << jumpMap.at(s.substr(0, s.length()-1)) << endl;
        rand=rand+1;
        continue;
    }
    else{
      rand= rand+1;
    }

  }

  //cout << rand; 


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

        else if(s.substr(0,1)== "#"){
          i=i+1;
          continue;
        }
        else if(s.substr(s.length()-1,1) == ":"){
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
          instruction+=1;
        }

        else if(s.substr(0,4)== "addi"){
          int a = addi(s,clockNumber,saveCycles,i, busyRegister,register_set);
          if(a==0){return 0;}
          instruction+=1;
        }

        else if(s.substr(0,3)=="sub"){
          int a = sub(s,clockNumber,saveCycles,i, busyRegister,register_set);
          if(a==0){return 0;}
          instruction+=1;
        }

        else if(s.substr(0,3)=="mul"){
          int a = mul(s,clockNumber,saveCycles,i, busyRegister,register_set);
          if(a==0){return 0;}
          instruction+=1;
        }

        else if(s.substr(0,1)=="j"){
          int a = j(i, s, clockNumber, saveCycles, jumpMap);
          if(a==0){return 0;}
          instruction+=1;

        }

        else if(s.substr(0,3)== "beq"){
          int a = beq(s,clockNumber,saveCycles,i, busyRegister,register_set, jumpMap);
            if(a==0){return 0;}
            instruction+=1;
        }

        else if(s.substr(0,3)== "bne"){
          int a = bne(s,clockNumber,saveCycles,i, busyRegister,register_set, jumpMap);
            if(a==0){return 0;}
            instruction+=1;
        }

        else if(s.substr(0,3)== "slt"){
          int a = slt(s,clockNumber,saveCycles,i, busyRegister,register_set);
            if(a==0){return 0;}
            instruction+=1;
        }

        else if(s.substr(0,2)== "lw"){
          findNextRequests(i);

          int a = efficientProcess(currentRow,i, busyRegisters, busyMemories,rows, numbers);
            if(a==0){return 0;}
        }

        else if(s.substr(0,2)== "sw"){
          findNextRequests(i);

          int a = efficientProcess(currentRow,i, busyRegisters, busyMemories,rows, numbers);
            if(a==0){return 0;}
        }

        else{
          cout << "Invalid Syntax at line:"<<i+1 << endl;
          return 0;
        }



        if(s.substr(0,2)!= "sw" && s.substr(0,2)!= "lw" && s.substr(0,3)!= "bne" && s.substr(0,3)!= "beq" && s.substr(0,1)!= "j") {
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
