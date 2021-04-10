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
vector<int> busyRegistersother;
vector<string> donecheck;
vector<int> busyMemories;
vector<int> rows;
int memory_program= 0;
vector<int> DRAM_memory(1048576, -2147483647);
vector<string> instruction_set;
vector<int> numbers;
unordered_map<string, int> jumpMap;

int check_busy(string s){
  
  //cout << "wow" << endl;
  int busy1, busy2, busy3;
  if(map(s.substr(3,3))!=100){
    busy1= map(s.substr(3,3));
  }
  if(map(s.substr(7,3))!=100){
    busy2= map(s.substr(11,3));
  }
  //cout << s.substr(11,3) << endl;
  if(map(s.substr(11,3))!=100){
    busy3= map(s.substr(11,3));
  }
  if(find(busyRegisters.begin(), busyRegisters.end(), busy1) != busyRegisters.end()) {
    //cout << "wow1" << endl;
    return 0;
  }
  if(find(busyRegisters.begin(), busyRegisters.end(), busy2) != busyRegisters.end()) {
    //cout << "wow2" << endl;
    return 0;
  }
  if(find(busyRegisters.begin(), busyRegisters.end(), busy3) != busyRegisters.end()) {
    //cout << "wow3" << endl;
    return 0;
  }

  busyRegistersother.push_back(map(s.substr(3,3)));
  busyRegistersother.push_back(map(s.substr(7,3)));
  busyRegistersother.push_back(map(s.substr(11,3)));
  return 1;
}


void findNextRequests(int &i){

  int hell;

  for(int j=i; j<instruction_set.size()+i;j++){
    hell= j;
    string s= instruction_set[j];
    s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());
    if(s.substr(0,2)== "sw" || s.substr(0,2)== "lw"){

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

      int busyRegister = map(s.substr(2,3));

      if(find(busyRegistersother.begin(), busyRegistersother.end(), busyRegister) != busyRegistersother.end()) {
        break;
      }
      else {
          /* v does not contain x */
          if(s.substr(0,2)== "lw"){
            busyRegisters.push_back(busyRegister);
          }
          rows.push_back(memoryLocation/1024);
          numbers.push_back(j);
      }
    }

    else{
      if(s==""){
        continue;
      }

      else if(s.substr(0,1)== "#"){
        continue;
      }
      else if(s.substr(s.length()-1,1) == ":"){
        continue;
      }

      else if(s.substr(0,3)=="add" && s.substr(0,4)!= "addi"){
        int k= check_busy(s);
        if(k==0){
          break;
        }
      }

      else if(s.substr(0,4)== "addi"){
        int busy1= map(s.substr(4,3));
        int busy2;
        if(s.substr(8,5)!="$zero"){
          busy2= map(s.substr(8,3));
        }
        if(find(busyRegisters.begin(), busyRegisters.end(), busy1) != busyRegisters.end()) {
          break;
        }
        if(find(busyRegisters.begin(), busyRegisters.end(), busy2) != busyRegisters.end()) {
          break;
        }
        busyRegistersother.push_back(map(s.substr(4,3)));
        if(s.substr(8,5)!="$zero"){
          busyRegistersother.push_back(map(s.substr(8,3)));
        }

      }

      else if(s.substr(0,3)=="sub"){
        int k= check_busy(s);
        if(k==0){
          break;
        }
      }

      else if(s.substr(0,3)=="mul"){
        int k= check_busy(s);
        if(k==0){
          break;
        }
      }

      else if(s.substr(0,1)=="j"){

        string help= s.substr(1,s.size()-1);
        if(jumpMap.find(help)== jumpMap.end()){
            //cout << "Wrong branch name:" <<i+1 << endl;
            break;
        }
        j= jumpMap.at(help)-1;

      }

      else if(s.substr(0,3)== "beq"){

          if(s.substr(3,5) == "$zero"){
            if(register_set[map(s.substr(9,3))] == 0){
              string help= s.substr(13,s.size()-13);

              if(jumpMap.find(help)== jumpMap.end()){
                break;
              }

              j= jumpMap.at(help)- 1;
            }
          }

          else if(s.substr(7,5) == "$zero"){
            if(register_set[map(s.substr(3,3))] == 0){
              string help= s.substr(13,s.size()-13);

              if(jumpMap.find(help)== jumpMap.end()){
                break;
              }

              j= jumpMap.at(help)- 1;
            }
          }

          else if(s.substr(3,5) == "$zero" && s.substr(9,5) == "$zero"){
            string help= s.substr(15,s.size()-15);
            //cout << s.substr(1,s.size()-1) << endl;

            if(jumpMap.find(help)== jumpMap.end()){
              break;
            }

            j= jumpMap.at(help) -1;

          }

        //throw syntax error if no commas
        else if(register_set[map(s.substr(3,3))]== register_set[map(s.substr(7,3))]){
          string help= s.substr(11,s.size()-11);

          if(jumpMap.find(help)== jumpMap.end()){
            break;
          }
          j= jumpMap.at(help) - 1;
        }
      }

      else if(s.substr(0,3)== "bne"){


        if(s.substr(7,5) == "$zero"){
          if(register_set[map(s.substr(3,3))] != 0){
            string help= s.substr(13,s.size()-13);
            if(jumpMap.find(help)== jumpMap.end()){
              break;
            }


            j= jumpMap.at(help)-1;      
          }
        }

        else if(s.substr(3,5) == "$zero"){
          if(register_set[map(s.substr(9,3))] != 0){
            string help= s.substr(13,s.size()-13);

            if(jumpMap.find(help)== jumpMap.end()){
              break;
            }


            j= jumpMap.at(help)-1;
            //cout << i << endl;
          }
        }

        else if(register_set[map(s.substr(3,3))]!= register_set[map(s.substr(7,3))]){
          //c//out << "ok1 " << endl;
          string help= s.substr(11,s.size()-11);
          //cout << s.substr(1,s.size()-1) << endl;

          if(jumpMap.find(help)== jumpMap.end()){
            break;
          }


          j= jumpMap.at(help)-1;
        }
      }


      else if(s.substr(0,3)== "slt"){
        int k= check_busy(s);
        if(k==0){
          break;
        }
      }

      else{
        break;
      }

    }
  }
  i = i+1;
  //cout << i << endl;
  //cout << i << endl;
  //cout << busyRegisters.size() << endl;
  //cout << busyMemories.size() << endl;
  //cout << rows.size() << endl;
  //cout << numbers.size() << endl;


}

int efficientProcess(int &currentRow, int &i, vector<int> &busyRegisters, vector<int> &busyMemories, vector<int> &rows, vector<int> &numbers){
  vector<int> deletej;
  //rewrite
  //convert them into hash sets
  //cout << "The current row: " << to_string(currentRow) << endl;

  //cout << "Entering" << endl;
  for(int j=0; j<rows.size(); j++){
    //cout << "ok " << endl;
    string s = instruction_set[numbers[j]];
    s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());
    //cout << currentRow << endl;
    if(currentRow == rows[j]){
      donecheck[numbers[j]]= "done";
      deletej.push_back(j);
      //cout<< "Pushing in the for loop into deletej: "<< j <<endl;
      if(s.substr(0,2)== "sw"){
        int a = sw(s,clockNumber,saveCycles,numbers[j],columnAccessDelay,rowAccessDelay,currentRow,busyRegister, register_set, previous_register_set, busyMemory, rowBufferUpdates, DRAM_memory);
        //cout << "wow" << endl;
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
  
  for(int j=0; j<deletej.size();j++){
    //cout<< j<< " " << deletej[j] << endl;
    //cout << j << endl;

    //busyRegisters.erase(busyRegisters.begin() + deletej[j]-j);
    //busyMemories.erase(busyMemories.begin() +  deletej[j]-j);
    rows.erase(rows.begin() +  deletej[j]-j);
    numbers.erase(numbers.begin() +  deletej[j]-j);
    //cout<< j<< " completed" << endl;
  }
  //cout<<"exiting dlete loop"<<endl;
  deletej.clear();

    while(rows.size() !=0){
      //cout<< "Entering while loop" << endl;
      string s = instruction_set[numbers[0]];
      s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());
      if(s.substr(0,2)== "sw"){
        donecheck[numbers[0]]= "done";
        int a = sw(s,clockNumber,saveCycles,numbers[0], columnAccessDelay,rowAccessDelay,currentRow,busyRegister, register_set, previous_register_set, busyMemory, rowBufferUpdates, DRAM_memory);
        if(a==0){return 0;}
        instruction++;
        //busyRegisters.erase(busyRegisters.begin() + 0);
        //busyMemories.erase(busyMemories.begin() + 0);
        rows.erase(rows.begin() + 0);
        numbers.erase(numbers.begin() + 0);
      }
      else if(s.substr(0,2)== "lw"){
        donecheck[numbers[0]]= "done";
        int a = lw(s,clockNumber,saveCycles,numbers[0], columnAccessDelay,rowAccessDelay,currentRow,busyRegister, register_set, previous_register_set, rowBufferUpdates, DRAM_memory);
        instruction++;
          if(a==0){return 0;}

          //busyRegisters.erase(busyRegisters.begin() + 0);
          //busyMemories.erase(busyMemories.begin() + 0);
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


int process(int &printCheck, int &i, string &s, string &s1){

  //method to remove the whitespaces
  s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());

  if (printCheck == -1) {
    cout<< "\nCycle description: \n" << endl;
    printCheck++;
  }
  //s now contains the current line after removing whitespace
  if(s==""){
    i=i+1;
    return 2;
  }

  else if(s.substr(0,1)== "#"){
    i=i+1;
    return 2;
  }
  else if(s.substr(s.length()-1,1) == ":"){
    i=i+1;
    return 2;
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
    return a;
    instruction+=1;
  }

  else if(s.substr(0,4)== "addi"){
    int a = addi(s,clockNumber,saveCycles,i, busyRegister,register_set);
    return a;
    instruction+=1;
  }

  else if(s.substr(0,3)=="sub"){
    int a = sub(s,clockNumber,saveCycles,i, busyRegister,register_set);
    return a;
    instruction+=1;
  }

  else if(s.substr(0,3)=="mul"){
    int a = mul(s,clockNumber,saveCycles,i, busyRegister,register_set);
    return a;
    instruction+=1;
  }

  else if(s.substr(0,1)=="j"){
    int a = j(i, s, clockNumber, saveCycles, jumpMap);
    return a;
    instruction+=1;

  }

  else if(s.substr(0,3)== "beq"){
    int a = beq(s,clockNumber,saveCycles,i, busyRegister,register_set, jumpMap, donecheck);
      return a;
      instruction+=1;
  }

  else if(s.substr(0,3)== "bne"){
    int a = bne(s,clockNumber,saveCycles,i, busyRegister,register_set, jumpMap, donecheck);
      return a;
      instruction+=1;
  }

  else if(s.substr(0,3)== "slt"){
    int a = slt(s,clockNumber,saveCycles,i, busyRegister,register_set);
      return a;
      instruction+=1;
  }

  else if(s.substr(0,2)== "lw"){
    findNextRequests(i);

    int a = efficientProcess(currentRow,i, busyRegisters, busyMemories,rows, numbers);
      return a;
  }

  else if(s.substr(0,2)== "sw"){
    findNextRequests(i);

    int a = efficientProcess(currentRow,i, busyRegisters, busyMemories,rows, numbers);
      return a;
  }

  else{
    cout << "Invalid Syntax at line:"<<i+1 << endl;
    return 0;
  }
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
   for(int i=0; i<instruction_set.size(); i++){
     donecheck.push_back("not");
   }

  cout << donecheck.size() << endl;

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
    s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());
    string s1 = s;
    if(donecheck[i]=="done"){
      i=i+1;
      continue;
    }

    if(s.size()>0 && s.substr(0,1)== "j"){
      string help= s.substr(1,s.size()-1);
      cout << help << endl;

      int k= jumpMap.at(help);
      for(int r=k; r< instruction_set.size(); r++){
        donecheck[r]= "not";
      }
    }

    int k= process(printCheck, i, s, s1);
    if(k==2){continue;}
    else if(k==0){return 0;}



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
