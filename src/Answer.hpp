// Answer.hpp
#ifndef ANSWER_HPP
#define ANSWER_HPP

#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>

using namespace std;

class Answer {
  protected:
    bool debug = false;

  public:
    void setDebug(bool debug) {
      debug = debug;
    }
    bool getDebug() {
      return debug;
    }
    virtual string getRandomAnswer() = 0;
    virtual string getAnswerFromKeywords(string keywords) = 0;
};

#endif