// AnswerXML.hpp
#ifndef ANSWERXML_HPP
#define ANSWERXML_HPP

#include "Answer.hpp"

class AnswerXML: public Answer {
  public:
    AnswerXML();
    AnswerXML(bool debug);
    ~AnswerXML();
    string getRandomAnswer();
    string getAnswerFromKeywords(std::string keywords);
    void doSomethingElse();
};

#endif