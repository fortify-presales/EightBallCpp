// Answer.h
#ifndef ANSWER_H
#define ANSWER_H

#include "stdafx.h"

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