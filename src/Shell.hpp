// Shell.hpp
#ifndef SHELL_HPP
#define SHELL_HPP

#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>

using namespace std;

class Shell {
public:
  Shell();
  Shell(bool debug);
  int execute(string command);
  int interactive();
private:
  bool debug;
};

#endif