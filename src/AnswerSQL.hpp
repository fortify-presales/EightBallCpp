// AnswerSQL.hpp
#ifndef ANSWERSQL_HPP
#define ANSWERSQL_HPP

#include "Answer.hpp"

class AnswerSQL: public Answer {
  private:
    void openDb();
    void closeDb();
  public:
    AnswerSQL();
    AnswerSQL(bool debug);
    ~AnswerSQL();
    string getRandomAnswer();
    string getAnswerFromKeywords(std::string keywords);
    void doSomethingElse();
};

#endif