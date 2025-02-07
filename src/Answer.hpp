// Answer.hpp
#ifndef ANSWER_HPP
#define ANSWER_HPP

#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>

using namespace std;

class Answer {
  private:
    bool aDebug;

  public:
    void setDebug(bool debug) { aDebug = debug; }
    bool getDebug() { return aDebug; }
    virtual string getRandomAnswer() = 0;
    virtual string getAnswerFromKeywords(std::vector<std::string> keywords) = 0;
};

#endif