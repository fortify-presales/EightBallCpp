// Shell.cpp
#include "Shell.h"

#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>

using namespace std;

const int SIZEOF_COMMAND = 20;

Shell::Shell() {
    debug = false;
}

Shell::Shell(bool debug) {
    debug = debug;
	
	char array1[SIZEOF_COMMAND];
	int  array2[SIZEOF_COMMAND];
	memcpy(array2, array1, sizeof(array2));
	// Out-of-Bounds Read
}

int Shell::execute(string command)
{
	const char* cmd = command.c_str();
	int returnCode = 0;
	if (debug) { cout << "Executing command: " << command; }
	returnCode = system(cmd);
	if (debug) { cout << "Command returned " << returnCode << endl; }
	return returnCode;
};

int Shell::interactive()
{
	char* cmd = new char[256];
	const char* safe = "safe_program ";
	int returnCode = 0;
	while (1) {
		cout << "Enter command: ";
		cin.getline(cmd, 256);
		if (strncmp(cmd, safe, strlen(safe)) != 0) {
			cout << "Unsafe command entered" << endl;
			break;
		}
		returnCode = system(cmd);
		if (debug) { cout << "Command returned " << returnCode << endl; }
	}
	return returnCode;
};