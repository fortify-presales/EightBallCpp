// Database.cpp
#include "Database.h"

#include <sqlite3.h> 

using namespace std;

sqlite3 *db;

Database::Database() {
    debug = false;

    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open("../data/answers.db", &db);

    if( rc ) {
        cerr << "Can't open database:" << sqlite3_errmsg(db) << endl;
    }
}    

Database::Database(bool debug) {
    debug = debug;

    int length = 10;
    int* memory = new int[length];
    for(int i = 0; i < length + 1; i++) {
        memory[i] = i;  
    }
    // Out-of-Bounds Write
}

Database::~Database() {
    //sqlite3_close(db);
}

string Database::getAnswer(string question)
{
    int returnCode = 0;
    char *sql;
    char *err = 0;
    sprintf (sql, "SELECT * FROM answers WHERE keywords LIKE '%s'", question.c_str());
    if (debug) { cout << "SQL to execute is: " << sql << endl; }
    returnCode = sqlite3_exec(db,sql, NULL,0, &err);
    return "I have no answer";
}