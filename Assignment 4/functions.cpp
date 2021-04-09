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

template< typename T >
string int_to_hex( T i ) {
  stringstream stream;
  stream << setfill ('0') << setw(8)
         << hex << i;
  return stream.str();
}

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

void print_memory(vector<int> DRAM_memory) {
  for(int i =0; i<1048576; i+=4){
    if(DRAM_memory[i] != -2147483647){
      cout<< i << "-" << i+3 << " : " << DRAM_memory[i] <<endl;
    }
  }
}

int add(string &s, int &clockNumber, int &saveCycles, int &i, int &busyRegister, vector<int> &register_set){
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

  if(saveCycles > 0 && map(s.substr(3,3))!= busyRegister && map(s.substr(7,3))!= busyRegister && map(s.substr(11,3))!= busyRegister ){
    saveCycles--;
  }
  else {clockNumber+=1; saveCycles =0;}
  return 1;
}

int addi(string &s, int &clockNumber, int &saveCycles, int &i, int &busyRegister, vector<int> &register_set){

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

        if(saveCycles > 0 && map(s.substr(4,3))!= busyRegister && map(s.substr(8,3))!= busyRegister){
          saveCycles--;
        }
        else {clockNumber+=1; saveCycles =0;}
        return 1;
}

int sub(string &s, int &clockNumber, int &saveCycles, int &i, int &busyRegister, vector<int> &register_set){

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
    register_set[map(s.substr(3,3))]= 0 - 0;
  }
  else if(s.substr(7,5) == "$zero"){
    register_set[map(s.substr(3,3))]= 0 - register_set[map(s.substr(13,3))];
  }
  else if(s.substr(12,5) == "$zero"){
    register_set[map(s.substr(3,3))]= register_set[map(s.substr(7,3))]- 0;
  }
  else {
    register_set[map(s.substr(3,3))]= register_set[map(s.substr(7,3))]- register_set[map(s.substr(11,3))];
  }
  i+=1;

  if(saveCycles > 0 && map(s.substr(3,3))!= busyRegister && map(s.substr(7,3))!= busyRegister && map(s.substr(11,3))!= busyRegister ){
    saveCycles--;
  }
  else {clockNumber+=1; saveCycles =0;}
  return 1;
}

int mul(string &s, int &clockNumber, int &saveCycles, int &i, int &busyRegister, vector<int> &register_set){

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
    register_set[map(s.substr(3,3))]= 0 * 0;
  }
  else if(s.substr(7,5) == "$zero"){
    register_set[map(s.substr(3,3))]= 0 * register_set[map(s.substr(13,3))];
  }
  else if(s.substr(12,5) == "$zero"){
    register_set[map(s.substr(3,3))]= register_set[map(s.substr(7,3))]* 0;
  }
  else {
    register_set[map(s.substr(3,3))]= register_set[map(s.substr(7,3))]* register_set[map(s.substr(11,3))];
  }
  i+=1;

  if(saveCycles > 0 && map(s.substr(3,3))!= busyRegister && map(s.substr(7,3))!= busyRegister && map(s.substr(11,3))!= busyRegister ){
    saveCycles--;
  }
  else {clockNumber+=1; saveCycles =0;}
  return 1;
}

int j(int &i, string &s, int &clockNumber, int &saveCycles){

  int helper= str_to_int(s.substr(1,s.size()-1));
  if(helper == 2147483647){
    cout << "Invalid Syntax at line:"<<i+1 << endl;
    return 0;
  }

  i= helper-1;

  clockNumber+=1;
  if(saveCycles > 0) {
    saveCycles--;
  }
  return 1;
}

int beq(string &s, int &clockNumber, int &saveCycles, int &i, int &busyRegister, vector<int> &register_set){

  ///////////////////////


  if((s.substr(6,1)!="," || s.substr(10,1)!=",") && s.substr(3,5) != "$zero" && s.substr(9,5) != "$zero"){
    cout << "Invalid Syntax at line:"<<i+1 << endl;
    return 0;
  }

  if((map(s.substr(7,3))==100 || map(s.substr(3,3))==100 || map(s.substr(11,3))==100) && s.substr(9,5) != "$zero" && s.substr(3,5) != "$zero"){
    cout << "Invalid Syntax at line:"<<i+1 << endl;
    return 0;
  }

  //throw syntax error if no commas
  if(register_set[map(s.substr(3,3))]== register_set[map(s.substr(7,3))]){
    int helper= str_to_int(s.substr(11,s.size()-11));
    if(helper == 2147483647){
      cout << "Invalid Syntax at line:"<<i+1 << endl;
      return 0;
    }
    i= helper-1;
  }

  else if(s.substr(3,5) == "$zero" && s.substr(9,5) == "$zero"){
    int helper = str_to_int(s.substr(15,s.size()-15));
    i = helper -1;
  }
  else if(s.substr(7,5) == "$zero"){
    if(register_set[map(s.substr(3,3))] == 0){
      int helper = str_to_int(s.substr(13,s.size()-13));
      i = helper -1;
    }
  }
  else if(s.substr(3,5) == "$zero"){
    if(register_set[map(s.substr(9,3))] == 0){
      int helper = str_to_int(s.substr(13,s.size()-13));
      i = helper -1;
    }
  }
  else i++;

  if(saveCycles > 0 && map(s.substr(3,3))!= busyRegister && map(s.substr(7,3))!= busyRegister && map(s.substr(9,3))!= busyRegister ){
    saveCycles--;
  }
  else {clockNumber+=1; saveCycles =0;}
  return 1;

}

int bne(string &s, int &clockNumber, int &saveCycles, int &i, int &busyRegister, vector<int> &register_set){

                    if((s.substr(6,1)!="," || s.substr(10,1)!=",") && s.substr(3,5) != "$zero" && s.substr(9,5) != "$zero"){
              				cout << "Invalid Syntax at line:"<<i+1 << endl;
              				return 0;
              			}

              			if((map(s.substr(7,3))==100 || map(s.substr(3,3))==100 || map(s.substr(11,3))==100) && s.substr(9,5) != "$zero" && s.substr(3,5) != "$zero"){
              				cout << "Invalid Syntax at line:"<<i+1 << endl;
              				return 0;
              			}

              			//throw syntax error if no commas
                    if(register_set[map(s.substr(3,3))]== register_set[map(s.substr(7,3))]){
              				int helper= str_to_int(s.substr(11,s.size()-11));
              				if(helper == 2147483647){
              					cout << "Invalid Syntax at line:"<<i+1 << endl;
              					return 0;
              				}
              				i= helper-1;
              			}

                    else if(s.substr(3,5) == "$zero" && s.substr(9,5) == "$zero"){
                      i++;
                    }
                    else if(s.substr(7,5) == "$zero"){
                      if(register_set[map(s.substr(3,3))] != 0){
                        int helper = str_to_int(s.substr(13,s.size()-13));
                        i = helper -1;
                      }
                    }
                    else if(s.substr(3,5) == "$zero"){
                      if(register_set[map(s.substr(9,3))] != 0){
                        int helper = str_to_int(s.substr(13,s.size()-13));
                        i = helper -1;
                      }
                    }


                    if(saveCycles > 0 && map(s.substr(3,3))!= busyRegister && map(s.substr(7,3))!= busyRegister && map(s.substr(9,3))!= busyRegister ){
                      saveCycles--;
                    }
                    else {clockNumber+=1; saveCycles =0;}

                    return 1;

}

int slt(string &s, int &clockNumber, int &saveCycles, int &i, int &busyRegister, vector<int> &register_set){

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

  clockNumber+=1;
  return 1;
}

int lw(string &s, int &clockNumber, int &saveCycles, int &i, int &columnAccessDelay, int &rowAccessDelay, int &currentRow, int &busyRegister, vector<int> &register_set, vector<int> &previous_register_set, int &rowBufferUpdates, vector<int> &DRAM_memory){
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

  clockNumber+=1;

  /// Printing


  cout << "Cycle " << clockNumber << ": " << "DRAM request issued" <<endl;
  int prevClock = clockNumber+1;

  if(currentRow == -1){
    cout<< "Uninitilised value is being accessed" << endl;
    return 0;
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
   previous_register_set = register_set;
   return 1;

}


int sw(string &s, int &clockNumber, int &saveCycles, int &i, int &columnAccessDelay, int &rowAccessDelay, int &currentRow, int &busyRegister, vector<int> &register_set, vector<int> &previous_register_set, int &busyMemory, int &rowBufferUpdates, vector<int> &DRAM_memory){

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

  clockNumber+=1;

  /// Printing

  cout << "Cycle " << clockNumber << ": " << "DRAM request issued" <<endl;
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
  return 1;
}
