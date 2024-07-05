// AnswerSQL.cpp

#include "AnswerSQL.hpp"

#include <sqlite3.h> 

using namespace std;

const int SIZEOF_ANSWERS = 20;

sqlite3 *db;

void AnswerSQL::openDb() {
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open("../data/answers.db", &db);

    if (rc) {
        cerr << "Can't open database:" << sqlite3_errmsg(db) << endl;
    } else {
        if (getDebug()) cout << "Database opened successfully" << endl;
    }
}

void AnswerSQL::closeDb() {
    sqlite3_close(db);
}

AnswerSQL::AnswerSQL() {
    openDb();
}    

AnswerSQL::AnswerSQL(bool debug) {
    AnswerSQL::setDebug(debug);
    openDb();
}

AnswerSQL::~AnswerSQL() {
    closeDb();
}

string AnswerSQL::getRandomAnswer() {
    int returnCode = 0;
    int index = rand() % SIZEOF_ANSWERS;
    char *sql;
    char *err = 0;
    sprintf (sql, "SELECT * FROM answers WHERE id = '%d'", index);
    if (getDebug()) { cout << "SQL to execute is: " << sql << endl; }
    returnCode = sqlite3_exec(db,sql, NULL,0, &err);
    return "I have no answer";
}

string AnswerSQL::getAnswerFromKeywords(string keywords) {
    int returnCode = 0;
    char *sql;
    char *err = 0;
    sprintf (sql, "SELECT * FROM answers WHERE keywords LIKE '%s'", keywords.c_str());
    if (getDebug()) { cout << "SQL to execute is: " << sql << endl; }
    returnCode = sqlite3_exec(db,sql, NULL,0, &err);
    return "I have no answer";
}

void AnswerSQL::doSomethingElse() {
    char *buffer = (char*)malloc(256);
    bool error = true;
    if (error) free(buffer);
    // use after free
    if (error) cout << buffer << endl;
    // double free
    free(buffer);
}