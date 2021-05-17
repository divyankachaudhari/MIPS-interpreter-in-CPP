///////////////////////////////////////////////////////////////////////////////
/*
//TO DO
//1. Code should not end if one of the cores has a error??
//2. Accessing memory should be offset
//3. Memory division and allotment
//4. Branch Change commands
//5. Change the map
//6. Add appropriate print commands for lw and sw
//7. lw command and another command should not collide



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

//--------------

// Assuming that we go into the sw lw loop only if DRAM has allowed it

//--------------

vector<vector<int> > register_set;
vector<vector<int> > previous_register_set;

int data_counter= 0;
int offset_val= 10000;
int rowBufferUpdates = 0;
int busyRegister = -1;
int busyMemory = -1;
vector<int> saveCycles;
vector<vector<int> > saveCycles_vec;
vector<string> Command_executed;
int rowAccessDelay;
int columnAccessDelay;
int clockNumber;
int currentRow;
int instruction;

vector<int> busyRegisters;
vector<int> busyRegistersother;
vector<vector<string> > donecheck;
vector<vector<int> > depends;
vector<int> busyMemories;
vector<int> rows;
int memory_program= 0;

vector<int> MemoryManager;
vector<int> MemoryManagerSavedCycles;


vector<int> DRAM_memory(1048576, -2147483647);
vector<vector<string> > instruction_set;
vector<int> numbers;
vector<unordered_map<string, int> > jumpMap;

// process function which takes any line as input and then processes it
int process(int &printCheck, int &i, string &s, string &s1, int q, vector<int>& countDown){

  //method to remove the whitespaces
  s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());

  // if (printCheck == -1) {
  //   cout<< "\nCycle description: \n" << endl;
  //   printCheck++;
  // }
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
    //cout << "1" << endl;
    cout << "Invalid Syntax at line:" << i+1<< endl;
    return 0;

  }

  else if(s.substr(0,1)!="j" && s.size()<7){
    //cout << "2" << endl;
    cout << "Invalid Syntax at line:"<<i+1 << endl;
    return 0;
  }

  else if(s.substr(0,3)=="add" && s.substr(0,4)!= "addi"){
    int a = add(s,clockNumber,saveCycles[q],i, busyRegister,register_set[q]);
    return a;
    instruction+=1;
  }

  else if(s.substr(0,4)== "addi"){
    //cout << "wow" << endl;
    int a = addi(s,clockNumber,saveCycles[q] ,i, busyRegister,register_set[q]);
    return a;
    instruction+=1;
  }

  else if(s.substr(0,3)=="sub"){
    int a = sub(s,clockNumber,saveCycles[q],i, busyRegister,register_set[q]);
    return a;
    instruction+=1;
  }

  else if(s.substr(0,3)=="mul"){
    int a = mul(s,clockNumber,saveCycles[q],i, busyRegister,register_set[q]);
    return a;
    instruction+=1;
  }

  else if(s.substr(0,1)=="j"){
    int a = j(i, s, clockNumber, saveCycles[q], jumpMap[q]);
    return a;
    instruction+=1;

  }

  else if(s.substr(0,3)== "beq"){
    int a = beq(s,clockNumber,saveCycles[q],i, busyRegister,register_set[q], jumpMap[q], donecheck[q]);
      return a;
      instruction+=1;
  }

  else if(s.substr(0,3)== "bne"){
    int a = bne(s,clockNumber,saveCycles[q],i, busyRegister,register_set[q], jumpMap[q], donecheck[q]);
      return a;
      instruction+=1;
  }

  else if(s.substr(0,3)== "slt"){
    int a = slt(s,clockNumber,saveCycles[q],i, busyRegister,register_set[q]);
      return a;
      instruction+=1;
  }

  else if(s.substr(0,2)== "lw"){
    //findNextRequests(i);
    //cout << "OMP" << endl;

    //Command_executed[q].push_back(s);

    //EDIT HERE
    //int a = efficientProcess(currentRow,i, busyRegisters, busyMemories,rows, numbers);
    int k=0;
    for(int i= 0; i< s.size(); i++){
      if(s.substr(i,1)=="("){
        k= i;
        break;
      }
    }

    int helper=str_to_int(s.substr(6, k-6));
    if(helper == 2147483647){
      //cout << "3" << endl;
        cout << "Invalid Syntax at line:"<<i+1 << endl;
        return 0;
    }

    int memoryLocation;
    if(s.substr(k+1, 5)== "$zero"){
      memoryLocation = str_to_int(s.substr(6, k-6));
    }
    else{
      memoryLocation = str_to_int(s.substr(6, k-6))+ register_set[q][map(s.substr(k+1, 3))];
    }
    MemoryManager.push_back(memoryLocation/1024);
    int a= lw(s, clockNumber, saveCycles[q], saveCycles_vec[q], i, rowAccessDelay, columnAccessDelay, busyRegister, register_set[q], previous_register_set[q], DRAM_memory, depends[q], currentRow);
    //MemoryManagerSavedCycles.push_back(saveCycles[q]);
    if(saveCycles_vec[q].size()== 0){
      if(q==0){

        saveCycles_vec[q].push_back(saveCycles[q]);
      }
      else{
        int temp= saveCycles_vec[q-1][saveCycles_vec[q-1].size()-1] + saveCycles[q];
        saveCycles_vec[q].push_back(temp);
      }
    }

    else {
      int temp= saveCycles_vec[q][saveCycles_vec[q].size()-1] + saveCycles[q];
      saveCycles_vec[q].push_back(temp);
    }
   
    MemoryManagerSavedCycles.push_back(saveCycles_vec[q][saveCycles_vec[q].size()-1]);


    //cout << "Current Row lw: " << memoryLocation/1024 << endl;
    return a;
  }

  else if(s.substr(0,2)== "sw"){

    //Command_executed[q].push_back(s);

    //EDIT HERE
    //int a = efficientProcess(currentRow,i, busyRegisters, busyMemories,rows, numbers);
    int k=0;
    for(int i= 0; i< s.size(); i++){
      if(s.substr(i,1)=="("){
        k= i;
        break;
      }
    }

    int helper=str_to_int(s.substr(6, k-6));
    if(helper == 2147483647){
      //cout << "4" << endl;
        cout << "Invalid Syntax at line:"<<i+1 << endl;
        return 0;
    }

    int memoryLocation;
    if(s.substr(k+1, 5)== "$zero"){
      memoryLocation = str_to_int(s.substr(6, k-6));
    }
    else{
      memoryLocation = str_to_int(s.substr(6, k-6))+ register_set[q][map(s.substr(k+1, 3))];
    }
    //MemoryManager.push_back(memoryLocation/1024);
    //cout << MemoryManager.size() << endl;
    //cout << "R"
    //cout << "This is memory val: " << memoryLocation << endl;
    int helper10= MemoryManager.size();
    for(int tem=0; tem<helper10; tem++){
      //cout << tem << endl;
      //cout << MemoryManager.size().endl;
      if(tem== MemoryManager.size()-1){
          //cout << " okkkkk " << endl;
          Command_executed.push_back(s);
          MemoryManager.push_back(memoryLocation/1024);
          if(MemoryManager[MemoryManager.size()-2]== memoryLocation/1024){
            //cout << "ok" << endl;

            MemoryManagerSavedCycles.push_back(columnAccessDelay);
          }
          
          else{
            MemoryManagerSavedCycles.push_back(columnAccessDelay+ 2*rowAccessDelay);
          }
        }

      else if(MemoryManager[tem]== memoryLocation/1024){

        if(MemoryManager[tem+1]!= memoryLocation/1024){
          MemoryManager.push_back(0);
          MemoryManagerSavedCycles.push_back(0);
          Command_executed.push_back("");
          for(int helpi= MemoryManager.size()-1; helpi>tem+1; helpi--){
            //MemoryManager[tem+1]= (memoryLocation/1024)
            Command_executed[helpi]= Command_executed[helpi-1];
            MemoryManager[helpi]= MemoryManager[helpi-1];
            MemoryManagerSavedCycles[helpi]= MemoryManagerSavedCycles[helpi-1];
            //MemoryManagerSavedCycles[helpi]= MemoryManagerSavedCycles+ columnAccessDelay;
          }
          Command_executed[tem+1]= s;
          MemoryManagerSavedCycles[tem+1]= columnAccessDelay;
          MemoryManager[tem+1]= memoryLocation/1024;
          break;
        }

      }


    }

    if(MemoryManager.size()==0){
      MemoryManager.push_back(memoryLocation/1024);
      MemoryManagerSavedCycles.push_back(rowAccessDelay+columnAccessDelay);
      Command_executed.push_back(s);
    }


    int a= sw(s, clockNumber, saveCycles[q], saveCycles_vec[q], i, rowAccessDelay, columnAccessDelay, busyRegister, register_set[q], previous_register_set[q], DRAM_memory, depends[q], currentRow);
    //MemoryManagerSavedCycles.push_back(saveCycles[q]);

    //int randtem= memoryLocation% offset_val;

    // for(int i=0 i<saveCycles_vec.size(); i++){
    //   for(int j=0; j<saveCycles_vec[i].size(); )
    // }


    // for(int i= 0; i<MemoryManager.size(); i++){
    //   if()
    // }




    // for(int i= 0; i<MemoryManager.size(); i++){
    //   if(MemoryManager[i]%offset_val== q){
    //     saveCycles_vec[q].push_back()
    //   }
    // }

    // if(saveCycles_vec[q].size()== 0){
    //   if(q==0){
    //     saveCycles_vec[q].push_back(saveCycles[q]);
    //   }
    //   else{
    //     int temp= saveCycles_vec[q-1][saveCycles_vec[q-1].size()-1]+ saveCycles[q];
    //     saveCycles_vec[q].push_back(temp);
    //   }
    // }

    // else {
    //   int temp= saveCycles_vec[q][saveCycles_vec[q].size()-1]+ saveCycles[q];
    //   saveCycles_vec[q].push_back(temp);
    // }
   
   // MemoryManagerSavedCycles.push_back(saveCycles_vec[q][saveCycles_vec[q].size()-1]);

    cout << "Current Row sw: " << memoryLocation/1024 << endl;

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

  vector<int> extra_int_vec;
  vector<vector<int> > tempvec(N,extra_int_vec);
  depends= tempvec;


  cout<< "Successfully inputted all files. \n";
  // idk i just couldn't initiliase size in global variable before taking N as input. maybe i just suck at it
  vector<vector<int> > register_set1( N , vector<int> (32, 0));
  vector<vector<int> > previous_register_set1( N , vector<int> (32, 0));
  vector<unordered_map<string, int> > jumpMap1(N);
  vector<int> countDown; // using this variable to keep track of the clock cycles after which we need to move on to the next command in that file. If it's 0 then we'll move on, else the number there denotes the amount of cycles we have to wait. We'll decrement this at every clock cycle

  for(int i=0; i<N; i++){
    countDown.push_back(0);
  }

  register_set = register_set1;
  previous_register_set = previous_register_set1;
  jumpMap = jumpMap1;
  //stores the current line in the input
  string line;

  // making an object to open multiple files
  //storing all the commands in a vector
//  string filename;

  // cout << "Going in the ifstream loop \n";

  //int k= 1024*1024/N;
    //int k=0;
    for(int k= 0; k<N; k++){
    cout << k << endl;
    cout<<"In the loop" << endl;
    cout << "Enter file number " << k +1 << ": ";
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


    //cout<< "step 2";
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
  //cout << "Coming out of the ifstream loop \n";
  //cout << donecheck.size() << endl;

  int i[N];

  for(int i=0; i<N; i++){
    saveCycles.push_back(-1);
  }

  vector<int> extra1;
  for(int i=0; i<N; i++){
    saveCycles_vec.push_back(extra1);
  }

  // vector<int> extra1;
  // for(int i=0; i<N; i++){
  //   MemoryManager.push_back(extra1);
  // }

  // vector<int> extra1;
  // for(int i=0; i<N; i++){
  //   MemoryManagerSavedCycles.push_back(extra1);
  // }
  //vector<string> extra2;

  // for(int i=0; i<N; i++){
    
  //   Command_executed.push_back(extra2);
  // }

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
int k[N];

//int pointer[n] = {0};
  while(m > 0){
    cout<< "\n-- Cycle number: " << (M - m) + 1 << endl;


    for(int q=0; q< N; q++){

      //cout << "k" << endl;

      //for(int r=0; r<MemoryManagerSavedCycles.size(); r++){
          if(MemoryManagerSavedCycles.size()>0 && MemoryManagerSavedCycles[0]==0){
            cout << Command_executed[0] << endl;

            // string helps= Command_executed[0];
            // int k=0;
            // for(int i= 0; i< helps.size(); i++){
            //   if(helps.substr(i,1)=="("){
            //     k= i;
            //     break;
            //   }
            // }

            // int memoryLocation;
            // if(helps.substr(k+1, 5)== "$zero"){
            //   memoryLocation = str_to_int(helps.substr(6, k-6));
            // }
            // else{
            //   memoryLocation = str_to_int(helps.substr(6, k-6))+ register_set[map(helps.substr(k+1, 3))];
            // }

            // if(helps.substr(0,2)== "lw"){
            //   if(DRAM_memory[memoryLocation]== -2147483647){
              
            //     register_set[map(s.substr(2,3))]=0;
            //     busyRegister = map(s.substr(2,3));
                
            //   }

            //   else{
            //     register_set[][map(s.substr(2,3))]=DRAM_memory[memoryLocation];
            //   }
            // }


            Command_executed.erase(Command_executed.begin());
            MemoryManagerSavedCycles.erase(MemoryManagerSavedCycles.begin());
            MemoryManager.erase(MemoryManager.begin());
            //MemoryManagerSavedCycles
          }
        //}
      //}
      

      if(i[q] > instruction_set[q].size()){
        //cout << "we continue" << endl;
        continue;
      }
      previous_register_set[q] = register_set[q];

      if(countDown[q] == 0){
        string s = instruction_set[q][i[q]];
        //cout <<"\n" << s;
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

        k[q]= process(printCheck, i[q], s, s1, q, countDown);
        //cout << "\n" << k[q];
        int returnval = k[q];
        while(returnval == 2 || returnval == 0){
          //cout << "This is for program q" << q << endl;
          if(returnval==2){
          if(i[q] >instruction_set[q].size() ){
            break;
          }
          s = instruction_set[q][i[q]];
          returnval = process(printCheck, i[q], s, s1, q, countDown);
          //cout << returnval << endl;
        }
        else if(returnval==0){return 0;}
      }
      //cout << returnval;



        if(s.substr(0,2)!= "sw" && s.substr(0,2)!= "lw" && s.substr(0,3)!= "bne" && s.substr(0,3)!= "beq" && s.substr(0,1)!= "j") {
          //cout << "Cycle " << clockNumber-saveCycles << ": ";
          cout<< "For program number " << q << ": " << endl;
          print_register(register_set[q], previous_register_set[q]);
      }
    }
      else {
        countDown[q]--;
      }



//End of while loop
  }
  // for(int q= 0; q<N; q++){
  //   for(int r=0; r<saveCycles_vec[q].size(); r++){
  //         saveCycles_vec[q][r]--;
  //         //cout << "Save cycles are " << r << ": " << saveCycles_vec[q][r] << endl;
  //       }
  //   //cout << saveCycles[q] << endl;
  // }
  if(MemoryManagerSavedCycles.size()>0){
    MemoryManagerSavedCycles[0]--;
  }

  //for(int i=0; i<MemoryManagerSavedCycles.size(); i++){
    //cout << "This is the value " << i << " : " <<  MemoryManagerSavedCycles[i] << endl;
  //}

  m--;


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
