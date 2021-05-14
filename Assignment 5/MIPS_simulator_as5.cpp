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
#include <vector>
#include <iomanip>
#include <cctype>
#include <algorithm>
#include <unordered_map>
#include "functions.h"
using namespace std;

// template< typename T >


vector<vector<int> > register_set;
vector<vector<int> > previous_register_set;

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
vector<vector<string> > donecheck;
vector<int> busyMemories;
vector<int> rows;
int memory_program= 0;
vector<int> DRAM_memory(1048576, -2147483647);
vector<vector<string> > instruction_set;
vector<int> numbers;
vector<unordered_map<string, int> > jumpMap;

// process function which takes any line as input and then processes it
int process(int &printCheck, int &i, string &s, string &s1, int q){

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
    int a = add(s,clockNumber,saveCycles,i, busyRegister,register_set[q]);
    return a;
    instruction+=1;
  }

  else if(s.substr(0,4)== "addi"){
    int a = addi(s,clockNumber,saveCycles,i, busyRegister,register_set[q]);
    return a;
    instruction+=1;
  }

  else if(s.substr(0,3)=="sub"){
    int a = sub(s,clockNumber,saveCycles,i, busyRegister,register_set[q]);
    return a;
    instruction+=1;
  }

  else if(s.substr(0,3)=="mul"){
    int a = mul(s,clockNumber,saveCycles,i, busyRegister,register_set[q]);
    return a;
    instruction+=1;
  }

  else if(s.substr(0,1)=="j"){
    int a = j(i, s, clockNumber, saveCycles, jumpMap[q]);
    return a;
    instruction+=1;

  }

  else if(s.substr(0,3)== "beq"){
    int a = beq(s,clockNumber,saveCycles,i, busyRegister,register_set[q], jumpMap[q], donecheck[q]);
      return a;
      instruction+=1;
  }

  else if(s.substr(0,3)== "bne"){
    int a = bne(s,clockNumber,saveCycles,i, busyRegister,register_set[q], jumpMap[q], donecheck[q]);
      return a;
      instruction+=1;
  }

  else if(s.substr(0,3)== "slt"){
    int a = slt(s,clockNumber,saveCycles,i, busyRegister,register_set[q]);
      return a;
      instruction+=1;
  }

  else if(s.substr(0,2)== "lw"){
    //findNextRequests(i);
    //EDIT HERE
    //int a = efficientProcess(currentRow,i, busyRegisters, busyMemories,rows, numbers);
    int a =0;
      return a;
  }

  else if(s.substr(0,2)== "sw"){
    //findNextRequests(i);
    // EDIT HERE
    //int a = efficientProcess(currentRow,i, busyRegisters, busyMemories,rows, numbers);
    int a =0;
      return a;
  }

  else{
    cout << "Invalid Syntax at line:"<<i+1 << endl;
    return 0;
  }
}

// main function to process
int main(){



  rowAccessDelay = 2;
  columnAccessDelay = 5;
  //string input = argv[3];

  int N; // number of CPU cores
  int M; // Number of cycles

  cout<< " Enter the number of CPU cores: ";
  cin>> N;
  //string inputi[N];
  cout<< "\n Enter the simulation time (number of cycles): ";
  cin>> M;
  int m = M;
  // cout<< "\n Enter the names of MIPS files: ";
  // for(int i=0; i<N; i++){
  //   cin>> input[i];
  //   cout << endl <<  i << input[i] << endl;
  // } // inputting file names
  vector<string> r;
  vector<vector<string> > a(N,r);
  instruction_set= a;

  vector<vector<string> > helper(N,r);
  donecheck= helper;


  cout<< "Successfully inputted all files. \n";
  // idk i just couldn't initiliase size in global variable before taking N as input. maybe i just suck at it
  vector<vector<int> > register_set1( N , vector<int> (32, 0));
  vector<vector<int> > previous_register_set1( N , vector<int> (32, 0));
  vector<unordered_map<string, int> > jumpMap1(N);
  int countDown[N]; // using this variable to keep track of the clock cycles after which we need to move on to the next command in that file. If it's 0 then we'll move on, else the number there denotes the amount of cycles we have to wait. We'll decrement this at every clock cycle

  register_set = register_set1;
  previous_register_set = previous_register_set1;
  jumpMap = jumpMap1;
  //stores the current line in the input
  string line;

  // making an object to open multiple files
  //storing all the commands in a vector
//  string filename;
  
   cout << "Going in the ifstream loop \n";

    //int k=0;
    for(int k= 0; k<N; k++){
    cout<<"In the loop" << endl;
    cout << "Enter file number " << k << ": ";
    //cin >> filename
    //cout << k+2 << endl;
    string input;
    cin >> input;
    cout<<"filename: "<<input<<endl;

    ifstream myfile(input);
    cout<<"filename: "<<input<<endl;
    //mstring filename= "input1.txt";
    //cin >> filename;
    
    //cout << " Your file name is " ;
  //cout << input[k];
    //cout << "step 1";


    
    while(getline(myfile, line)){
      cout<<"line: "<<line<<endl;

      if(memory_program<100000){
        //vector<string> a;
        instruction_set[k].push_back(line);
        cout << instruction_set[k][0];
        memory_program+=4;
      }
      else{
        cout << "Instruction Memory Overflow" <<  endl;
        return 0;
      }
    }

 
    cout<< "step 2";
     for(int i=0; i<instruction_set[k].size(); i++){
       donecheck[k].push_back("not");
       //cout << "ok" << endl;
     }

     for(int j=0; j<32; j++){
       (register_set[k])[j] = 0;
       (previous_register_set[k])[j] = 0;
     }
     myfile.close();
  }
  cout << "Coming out of the ifstream loop \n";
  //cout << donecheck.size() << endl;

  int i[N];

  //The number of instructions
  instruction=0;
  clockNumber =0;
  currentRow = -1;
  int printCheck =-1;

  //Looping through the code and storing the name of all the labels
  int rand= 0;

// some optimisation for multiple files
for(int i= 0; i<N; i++){

  while(rand<instruction_set[i].size()){
    string s= instruction_set[i][rand];
    //method to remove the whitespaces
    s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());
    instruction_set[i][rand] = s;
    if(s==""){
          rand=rand+1;
          continue;
    }

    else if(s.substr(s.length()-1,1) == ":"){
        //jumpMap[s.substr(0, s.length()-1)] = rand;
        jumpMap[i].insert(make_pair(s.substr(0, s.length()-1), rand+1));
        //cout << s.substr(0, s.length()-1) << endl;
        //cout << jumpMap.at(s.substr(0, s.length()-1)) << endl;
        rand=rand+1;
        continue;
    }
    else{
      rand= rand+1;
    }

  }
  rand = 0;
}

  //cout << rand;
  // main loop
// changing it from instruction wise to clock cycle wise; m = M
// i[j] denotes the line number we're at at jth file
  while(m > 0){

    for(int q=0; q< N; q++){
      previous_register_set[q] = register_set[q];

      if(countDown[q] == 0){
        string s = instruction_set[q][i[q]];

        //s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end()); // already did it above
        string s1 = s;
        if(donecheck[q][i[q]]=="done"){
          i[q]=i[q]+1;
          continue;
        }

        if(s.size()>0 && s.substr(0,1)== "j"){
          string help= s.substr(1,s.size()-1);
          //cout << help << endl;

          int k= jumpMap[q].at(help);
          for(int r=k; r< instruction_set.size(); r++){
            donecheck[q][r]= "not";
          }
        }

        int k= process(printCheck, i[q], s, s1, q);
        if(k==2){continue;}
        else if(k==0){return 0;}



        if(s.substr(0,2)!= "sw" && s.substr(0,2)!= "lw" && s.substr(0,3)!= "bne" && s.substr(0,3)!= "beq" && s.substr(0,1)!= "j") {
          //cout << "Cycle " << clockNumber-saveCycles << ": ";
          print_register(register_set[q], previous_register_set[q]);
      }

      else {
        countDown[q]--;
      }

      }

//End of while loop
  }

  cout<< "Cycle number: " << (M - m) + 1 << endl;


}


  cout<< "\nMemory content at the end of execution: \n"<<endl;
  print_memory(DRAM_memory);
  cout<< "\n";
  //cout << "Total number of the instructions: " << instruction <<endl;
  cout<< "Total number of clock cycles: " << clockNumber << endl;
  cout<< "Total number of row buffer updates/row activations: " << rowBufferUpdates << endl;

  cout<< "Row access delay considered to be: " << rowAccessDelay<<endl;
  cout<< "Column access delay considered to be: " << columnAccessDelay<< endl;

}
