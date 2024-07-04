// AnswerSQL.hpp
#ifndef ANSWERSQL_HPP
#define ANSWERSQL_HPP

#include "Answer.hpp"

class AnswerSQL: public Answer {
  public:
    AnswerSQL();
    ~AnswerSQL();
    string getRandomAnswer();
    string getAnswerFromKeywords(std::string keywords);
};

#endif