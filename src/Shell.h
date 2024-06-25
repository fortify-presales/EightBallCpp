// Shell.h
#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>

using namespace std;

class Shell
{
public:
  Shell();
  Shell(bool debug);
  int execute(string command);
  int interactive();
private:
  bool debug;
};

#endif