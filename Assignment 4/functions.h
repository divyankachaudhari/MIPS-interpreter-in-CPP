#ifndef FUNCTIONS_H
#define FUNCTIONS_H

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

template< typename T >
string int_to_hex( T i );

void print_register(vector<int> a, vector<int> b);

int map(string input);

int str_to_int(string s);

void print_memory(vector<int> DRAM_memory);
int add(string &s, int &clockNumber, int &saveCycles, int &i, int &busyRegister, vector<int> &register_set);

int addi(string &s, int &clockNumber, int &saveCycles, int &i, int &busyRegister, vector<int> &register_set);
int sub(string &s, int &clockNumber, int &saveCycles, int &i, int &busyRegister, vector<int> &register_set);
int mul(string &s, int &clockNumber, int &saveCycles, int &i, int &busyRegister, vector<int> &register_set);

int j(int &i, string &s, int &clockNumber, int &saveCycles);
int beq(string &s, int &clockNumber, int &saveCycles, int &i, int &busyRegister, vector<int> &register_set);

int bne(string &s, int &clockNumber, int &saveCycles, int &i, int &busyRegister, vector<int> &register_set);

int slt(string &s, int &clockNumber, int &saveCycles, int &i, int &busyRegister, vector<int> &register_set);

int lw(string &s, int &clockNumber, int &saveCycles, int &i, int &columnAccessDelay, int &rowAccessDelay, int &currentRow, int &busyRegister, vector<int> &register_set, vector<int> &previous_register_set, int &rowBufferUpdates, vector<int> &DRAM_memory);

int sw(string &s, int &clockNumber, int &saveCycles, int &i, int &columnAccessDelay, int &rowAccessDelay, int &currentRow, int &busyRegister, vector<int> &register_set, vector<int> &previous_register_set, int &busyMemory, int &rowBufferUpdates, vector<int> &DRAM_memory);





#endif
