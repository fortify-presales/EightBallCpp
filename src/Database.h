// Database.h
#ifndef DATABASE_H
#define DATABASE_H

#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>

using namespace std;

class Database
{
public:
  Database();
  Database(bool debug);
  ~Database();
  string getAnswer(string question);
private:
  bool debug;
};

#endif