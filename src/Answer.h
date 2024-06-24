// Answer.h
#ifndef ANSWER_H
#define ANSWER_H

#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>

using namespace std;

class Answer
{
public:
  Answer();
  Answer(bool debug);
  string getAnswer();
private:
  bool debug;
};

#endif