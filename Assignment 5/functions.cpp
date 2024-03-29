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

  int returnValue1=0, returnValue2=0, returnValue3=0, returnValue4=0, returnValue5=0;
	for(int i=2; i< 4; i++){
    if(a[i] != b[i]){
		  cout << "$v" << i-2 << " = " << int_to_hex(a[i])<<  endl;
      returnValue1 = 1;
    }

	}

	for(int i=4; i< 8; i++){
    if(a[i] != b[i]){
		  cout << "$a" << i-4 << " = " << int_to_hex(a[i])<<  endl;
      returnValue2 = 1;
    }

	}

	for(int i=8; i< 16; i++){
    if(a[i] != b[i]){
		  cout << "$t" << i-8 << " = " << int_to_hex(a[i])<<  endl;
      returnValue3 = 1;
    }

	}

	for(int i=16; i< 24; i++){
    if(a[i] != b[i]){
		  cout << "$s" << i-16 << " = " << int_to_hex(a[i])<<  endl;
      returnValue4 = 1;
    }

	}

  for(int i=24; i< 26; i++){
    if(a[i] != b[i]){
		  cout << "$t" << i-24 << " = " << int_to_hex(a[i])<<  endl;
      returnValue4 = 1;
    }

	}

  if(returnValue1 != 1 && returnValue2 != 1 && returnValue3 != 1 && returnValue4 != 1 && returnValue5 != 1){
    cout<< "No change in value of register" << endl;
  }



}

int map(string input){
	//note that we cant use switch statement with string
	//in C++ because it is not a supported type by the C++ compiler
	int flag=0;

  for(int i=0; i<2; i++){
    if(input== ("$v" + to_string(i))){
      flag=1;
      return i+2;

    }
  }

  for(int i=0; i<4; i++){
    if(input== ("$a" + to_string(i))){
      flag=1;
      return i+4;

    }
  }

	for(int i=0; i<8; i++){
		if(input== ("$s" + to_string(i))){
			flag=1;
			return i+16;
		}
	}

	for(int i=0; i<8; i++){
		if(input== ("$t" + to_string(i))){
			flag=1;
			return i+8;

		}
	}

  for(int i=8; i<10; i++){
    if(input== ("$t" + to_string(i))){
      flag= 1;
      return i+ 16;
    }
  }


  if( input== "$at"){
    flag=1;
    return 1;
  }

  if( input== "$gp"){
    flag=1;
    return 28;
  }

  if( input== "$sp"){
    flag=1;
    return 29;
  }

  if( input== "$fp"){
    flag=1;
    return 30;
  }

  if( input== "$ra"){
    flag=1;
    return 31;
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
  if((s.substr(6,1)!="," || s.substr(10,1)!=",") && s.substr(7,5) != "$zero" && s.substr(13,5) != "$zero" && s.substr(11,5) != "$zero"){
    cout << "Invalid Syntax at line:"<<i+1 << endl;
    return 0;
  }

  if((map(s.substr(7,3))==100 || map(s.substr(3,3))==100 || map(s.substr(11,3))==100) && s.substr(7,5) != "$zero" && s.substr(13,5) != "$zero" && s.substr(11,5) != "$zero"){
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

  // if(saveCycles > 0 && map(s.substr(3,3))!= busyRegister && map(s.substr(7,3))!= busyRegister && map(s.substr(11,3))!= busyRegister ){
  //   saveCycles--;
  // }
  // else {clockNumber+=1; saveCycles =0;}
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

        // if(saveCycles > 0 && map(s.substr(4,3))!= busyRegister && map(s.substr(8,3))!= busyRegister){
        //   saveCycles--;
        // }
        // else {clockNumber+=1; saveCycles =0;}

        //cout << "The clock number is:" << clockNumber << endl;

        return 1;
}

int sub(string &s, int &clockNumber, int &saveCycles, int &i, int &busyRegister, vector<int> &register_set){

  if((s.substr(6,1)!="," || s.substr(10,1)!=",") && s.substr(7,5) != "$zero" && s.substr(13,5) != "$zero" && s.substr(11,5) != "$zero"){
    cout << "Invalid Syntax at line:"<<i+1 << endl;
    return 0;
  }

  if((map(s.substr(7,3))==100 || map(s.substr(3,3))==100 || map(s.substr(11,3))==100) && s.substr(7,5) != "$zero" && s.substr(13,5) != "$zero" && s.substr(11,5) != "$zero"){
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

  // if(saveCycles > 0 && map(s.substr(3,3))!= busyRegister && map(s.substr(7,3))!= busyRegister && map(s.substr(11,3))!= busyRegister ){
  //   saveCycles--;
  // }
  // else {clockNumber+=1; saveCycles =0;}
  return 1;
}

int mul(string &s, int &clockNumber, int &saveCycles, int &i, int &busyRegister, vector<int> &register_set){

  if((s.substr(6,1)!="," || s.substr(10,1)!=",") && s.substr(7,5) != "$zero" && s.substr(13,5) != "$zero" && s.substr(11,5) != "$zero"){
    cout << "Invalid Syntax at line:"<<i+1 << endl;
    return 0;
  }

  if((map(s.substr(7,3))==100 || map(s.substr(3,3))==100 || map(s.substr(11,3))==100) && s.substr(7,5) != "$zero" && s.substr(13,5) != "$zero" && s.substr(11,5) != "$zero"){
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

  // if(saveCycles > 0 && map(s.substr(3,3))!= busyRegister && map(s.substr(7,3))!= busyRegister && map(s.substr(11,3))!= busyRegister ){
  //   saveCycles--;
  // }
  // else {clockNumber+=1; saveCycles =0;}
  return 1;
}

int j(int &i, string &s, int &clockNumber, int &saveCycles, unordered_map<string, int> jumpMap){

  string help= s.substr(1,s.size()-1);
  //cout << s.substr(1,s.size()-1) << endl;

  if(jumpMap.find(help)== jumpMap.end()){
      cout << "Wrong branch name:" <<i+1 << endl;
      return 0;
  }


  i= jumpMap.at(help);

  //clockNumber+=1;
  //if(saveCycles > 0) {
   // saveCycles--;
  //}
  return 1;
}

int beq(string &s, int &clockNumber, int &saveCycles, int &i, int &busyRegister, vector<int> &register_set, unordered_map<string, int> jumpMap, vector<string> &donecheck){

  ///////////////////////

  string help;
  if((s.substr(6,1)!="," || s.substr(10,1)!=",") && s.substr(3,5) != "$zero" && s.substr(9,5) != "$zero" && s.substr(7,5)!= "$zero"){
    cout << "Invalid Syntax at line:"<<i+1 << endl;
    return 0;
  }

  if((map(s.substr(7,3))==100 || map(s.substr(3,3))==100) && s.substr(9,5) != "$zero" && s.substr(3,5) != "$zero" && s.substr(7,5)!= "$zero"){
    cout << "Invalid Syntax at line:"<<i+1 << endl;
    //cout << "wow" << endl;
    return 0;
  }

  if(s.substr(3,5) == "$zero"){
    if(register_set[map(s.substr(9,3))] == 0){
      help= s.substr(13,s.size()-13);

      if(jumpMap.find(help)== jumpMap.end()){
        cout << "Wrong branch name:" <<i+1 << endl;
        return 0;
      }


      i= jumpMap.at(help);
    }

    else{
      i++;
    }
  }

  else if(s.substr(7,5) == "$zero"){
    if(register_set[map(s.substr(3,3))] == 0){
      help= s.substr(13,s.size()-13);

      if(jumpMap.find(help)== jumpMap.end()){
        cout << "Wrong branch name:" <<i+1 << endl;
        return 0;
      }


      i= jumpMap.at(help);
    }
    else{
      i++;
    }
  }

  else if(s.substr(3,5) == "$zero" && s.substr(9,5) == "$zero"){
    help= s.substr(15,s.size()-15);
    //cout << s.substr(1,s.size()-1) << endl;

    if(jumpMap.find(help)== jumpMap.end()){
      cout << "Wrong branch name:" <<i+1 << endl;
      return 0;
    }


    i= jumpMap.at(help);

  }

  //throw syntax error if no commas
  else if(register_set[map(s.substr(3,3))]== register_set[map(s.substr(7,3))]){
    help= s.substr(11,s.size()-11);
    //cout << s.substr(1,s.size()-1) << endl;


    if(jumpMap.find(help)== jumpMap.end()){
      cout << "Wrong branch name:" <<i+1 << endl;
      return 0;
    }


    i= jumpMap.at(help);
  }

  else i++;

  if(jumpMap.find(help)!= jumpMap.end()){
    for(int r=jumpMap.at(help); r<donecheck.size(); r++){
      donecheck[r]="not";
    }
  }

  // if(saveCycles > 0 && map(s.substr(3,3))!= busyRegister && map(s.substr(7,3))!= busyRegister && map(s.substr(9,3))!= busyRegister ){
  //   saveCycles--;
  // }
  // else {clockNumber+=1;
  //   saveCycles =0;}
  return 1;

}

int bne(string &s, int &clockNumber, int &saveCycles, int &i, int &busyRegister, vector<int> &register_set, unordered_map<string, int> jumpMap, vector<string> &donecheck){
    string help;
    if((s.substr(6,1)!="," || s.substr(10,1)!=",") && s.substr(3,5) != "$zero" && s.substr(9,5) != "$zero" && s.substr(7,5)!= "$zero"){
      //cout << "ok" << endl;
			cout << "Invalid Syntax at line:"<<i+1 << endl;
      //cout << "okdw" << endl;
			return 0;
		}

		if((map(s.substr(7,3))==100 || map(s.substr(3,3))==100) && s.substr(9,5) != "$zero" && s.substr(3,5) != "$zero" && s.substr(7,5)!= "$zero"){
			cout << "Invalid Syntax at line:"<<i+1 << endl;
      //cout << "ok" << endl;
			return 0;
		}

		//throw syntax error if no commas
    if(s.substr(7,5) == "$zero"){
      if(register_set[map(s.substr(3,3))] != 0){
        help= s.substr(13,s.size()-13);
        //cout << help << endl;

        if(jumpMap.find(help)== jumpMap.end()){

          cout << "Wrong branch name:" <<i+1 << endl;
          return 0;
        }


        i= jumpMap.at(help);
        //cout << i;

        //cout << i << endl;
      }

      else{
        i++;
      }
    }

    else if(s.substr(3,5) == "$zero"){
      if(register_set[map(s.substr(9,3))] != 0){
        help= s.substr(13,s.size()-13);

        if(jumpMap.find(help)== jumpMap.end()){
          cout << "Wrong branch name:" <<i+1 << endl;
          return 0;
        }


        i= jumpMap.at(help);
        //cout << i << endl;
      }
      else{
        i++;
      }
    }

    else if(s.substr(3,5) == "$zero" && s.substr(9,5) == "$zero"){
      i++;
    }

    else if(register_set[map(s.substr(3,3))]!= register_set[map(s.substr(7,3))]){
      //c//out << "ok1 " << endl;
			help= s.substr(11,s.size()-11);
      //cout << s.substr(1,s.size()-1) << endl;

      if(jumpMap.find(help)== jumpMap.end()){
        cout << "Wrong branch name:" <<i+1 << endl;
        return 0;
      }


      i= jumpMap.at(help);
		}

    else i++;

    if(jumpMap.find(help)!= jumpMap.end()){
      for(int r=jumpMap.at(help); r<donecheck.size(); r++){
        donecheck[r]="not";
      }
    }


    //if(saveCycles > 0 && map(s.substr(3,3))!= busyRegister && map(s.substr(7,3))!= busyRegister && map(s.substr(9,3))!= busyRegister ){
    //  saveCycles--;
    //}
    //else {clockNumber+=1;
    //  saveCycles =0;}

    return 1;

}

int slt(string &s, int &clockNumber, int &saveCycles, int &i, int &busyRegister, vector<int> &register_set){

  if((s.substr(6,1)!="," || s.substr(10,1)!=",") && s.substr(7,5) != "$zero" && s.substr(13,5) != "$zero" && s.substr(11,5) != "$zero"){
    cout << "Invalid Syntax at line:"<<i+1 << endl;
    return 0;
  }

  if((map(s.substr(7,3))==100 || map(s.substr(3,3))==100 || map(s.substr(11,3))==100) && s.substr(7,5) != "$zero" && s.substr(13,5) != "$zero" && s.substr(11,5) != "$zero"){
    cout << "Invalid Syntax at line:"<<i+1 << endl;
    return 0;
  }

  if(s.substr(7,5) == "$zero" && s.substr(12,5) == "$zero"){
    register_set[map(s.substr(3,3))]= 0;
  }
  else if(s.substr(7,5) == "$zero"){
    if(0 < register_set[map(s.substr(13,3))]){
      register_set[map(s.substr(3,3))]= 1;
    }
    else{
      register_set[map(s.substr(3,3))]= 0;
    }
  }
  else if(s.substr(12,5) == "$zero"){
    if(register_set[map(s.substr(7,3))]< 0){
      register_set[map(s.substr(3,3))]= 1;
    }
    else{
      register_set[map(s.substr(3,3))]= 0;
    }
  }
  else {
    if(register_set[map(s.substr(7,3))]< register_set[map(s.substr(11,3))]){
      register_set[map(s.substr(3,3))]= 1;
    }
    else{
      register_set[map(s.substr(3,3))]= 0;
    }
  }
  i+=1;

  if(saveCycles > 0 && map(s.substr(3,3))!= busyRegister && map(s.substr(7,3))!= busyRegister && map(s.substr(11,3))!= busyRegister ){
    saveCycles--;
  }
  else {clockNumber+=1; saveCycles =0;}
  return 1;
}

int lw(string &s, int &clockNumber, int &saveCycles, vector<int> &saveCycles_vec, int &i, int rowAccessDelay, int columnAccessDelay, int &busyRegister, vector<int> &register_set, vector<int> &previous_register_set, vector<int> &DRAM_memory, vector<int> &depends, int &currentRow, int offset_val, int q){
  if(s.substr(5,1)!=","){
    cout << "Invalid Syntax at line:"<<i+1 << endl;
    return 0;
  }

  if(map(s.substr(2,3))==100){
    cout << "Invalid Syntax at line:"<<i+1 << endl;
    return 0;
  }

  int k=0;
  for(int i= 0; i< s.size(); i++){
    if(s.substr(i,1)=="("){
      k= i;
      break;
    }
  }

  int helper=str_to_int(s.substr(6, k-6));
  if(helper == 2147483647){
      cout << "Invalid Syntax at line:"<<i+1 << endl;
      return 0;
  }

  if(s.substr(s.size()-1, 1)!= ")"){
    cout << "Invalid Syntax at line:"<<i+1 << endl;
    return 0;
  }

  int memoryLocation;
  if(s.substr(k+1, 5)== "$zero"){
    memoryLocation = str_to_int(s.substr(6, k-6));
  }
  else{
    memoryLocation = str_to_int(s.substr(6, k-6))+ register_set[map(s.substr(k+1, 3))];
  }
  if(DRAM_memory[memoryLocation]== -2147483647){
    //cout << "Trying to access uninitialised memory at line:" <<i+1 << endl;
    register_set[map(s.substr(2,3))]=0;
    busyRegister = map(s.substr(2,3));
    //return 0;
  }
  if(memoryLocation%4 != 0){
    cout<< "Memory location not accesible at line: " << i+1 <<endl;
    return 0;
  }

  else if(memoryLocation> 1024*1024){
    cout << "Data memory overflow" << endl;
  }
  // else{
  //   register_set[map(s.substr(2,3))]=DRAM_memory[memoryLocation];
  //   busyRegister = map(s.substr(2,3));
  // }
  i+=1;

  //clockNumber+=1;

   //Printing
  //cout << "The clock number is:" << clockNumber << endl;


  cout << "DRAM request issued" <<endl;
  //int prevClock = clockNumber+1;

  // if(currentRow == -1){
  //   //cout << "1" << endl;
  //   currentRow = memoryLocation/1024;

  //   saveCycles = columnAccessDelay+ rowAccessDelay;
  //   //return 0;
  // }
  // else if(currentRow == memoryLocation/1024){
  //   //clockNumber+= columnAccessDelay;
  //   //cout << "2" << endl;
  //   saveCycles = columnAccessDelay;
  // }
  // else if(currentRow != memoryLocation/1024){
  //   currentRow = memoryLocation/1024;
  //   //rowBufferUpdates += 1;
  //   //clockNumber+= rowAccessDelay;
  //   //clockNumber+= rowAccessDelay;
  //   //clockNumber+= columnAccessDelay;
  //   saveCycles = 2*rowAccessDelay + columnAccessDelay;
  // }

  ///else {cout << "Something went wrong";}

  //cout << "Cycle " << prevClock<< "-" << clockNumber << ": ";
   print_register(register_set, previous_register_set);
   //previous_register_set = register_set;
   return 1;

}


int sw(string &s, int &clockNumber, int &saveCycles, vector<int> &saveCycles_vec, int &i, int rowAccessDelay, int columnAccessDelay, int &busyRegister, vector<int> &register_set, vector<int> &previous_register_set, vector<int> &DRAM_memory, vector<int> &depends, int &currentRow, int offset_val, int q){

  if(s.substr(5,1)!=","){
    cout << "Invalid Syntax at line:"<<i+1 << endl;
    return 0;
  }

  if(map(s.substr(2,3))==100){
    cout << "Invalid Syntax at line:"<<i+1 << endl;
    return 0;
  }

  int k=0;
  for(int i= 0; i< s.size(); i++){
    if(s.substr(i,1)=="("){
      k= i;
      break;
    }
  }

  int helper=str_to_int(s.substr(6, k-6));
  if(helper == 2147483647){
      cout << "Invalid Syntax at line:"<<i+1 << endl;
      return 0;
  }

  int memoryLocation;
  if(s.substr(k+1, 5)== "$zero"){
    memoryLocation = str_to_int(s.substr(6, k-6));
  }
  else{
    memoryLocation = str_to_int(s.substr(6, k-6))+ register_set[map(s.substr(k+1, 3))];
  }

  if(memoryLocation>1048576){
    cout << " Data Memory overflow at line:" << i+1 << endl;
    return 0;
  }
  else if(memoryLocation%4 != 0){
    cout<< "Memory location not accesible at line: " << i+1 <<endl;
    return 0;
  }

  else if(memoryLocation> 1024*1024){
    cout << "Data memory overflow" << endl;
  }
  else{
    //data_set.insert(make_pair(memoryLocation, register_set[map(s.substr(2,3))]));

     DRAM_memory[memoryLocation+ q*offset_val] = register_set[map(s.substr(2,3))];
     //busyMemory = memoryLocation;
  }
  i+=1;

    cout << "DRAM request issued" <<endl;
  //int prevClock = clockNumber+1;

  // if(currentRow == -1){
  //   saveCycles = columnAccessDelay+ rowAccessDelay;
  //   currentRow= memoryLocation/1024;
  //   //return 0;
  // }
  // else if(currentRow == memoryLocation/1024){
  //   //clockNumber+= columnAccessDelay;
  //   saveCycles = columnAccessDelay;
  // }
  // else if(currentRow != memoryLocation/1024){
  //   currentRow = memoryLocation/1024;
  //   //rowBufferUpdates += 1;
  //   //clockNumber+= rowAccessDelay;
  //   //clockNumber+= rowAccessDelay;
  //   //clockNumber+= columnAccessDelay;
  //   saveCycles = 2*rowAccessDelay + columnAccessDelay;
  // }

  //else {cout << "Something went wrong";}

  //cout << "Cycle " << prevClock<< "-" << clockNumber << ": ";
   print_register(register_set, previous_register_set);
   //previous_register_set = register_set;
   return 1;
}
