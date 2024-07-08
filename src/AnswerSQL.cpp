// AnswerSQL.cpp

#include "AnswerSQL.hpp"

#include <sqlite3.h> 
#include <vector>

using namespace std;

const int SIZEOF_ANSWERS = 20;
const std::string DEFAULT_DATABASE = "../data/answers.db";

sqlite3 *db;
sqlite3_stmt *stmt;

void AnswerSQL::openDb() {
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open(DEFAULT_DATABASE.c_str(), &db);

    if (rc) {
        std::cerr << "Can't open database:" << sqlite3_errmsg(db) << endl;
    } else {
        if (getDebug()) cout << "Database opened successfully" << endl;
    }

    if (SQLITE_OK == rc) {
        std::string query = "CREATE TABLE IF NOT EXISTS Answers(Id INTEGER PRIMARY KEY, Type TEXT, Keywords TEXT, Message TEXT);";
        rc = sqlite3_exec(db, query.c_str(), NULL, 0, &zErrMsg);
    } else {
        std::cerr << "Can't create Answers table:" << sqlite3_errmsg(db) << endl;
    }

    if (SQLITE_OK == rc) {
        std::string query = "DELETE FROM Answers;";
        rc = sqlite3_exec(db, query.c_str(), NULL, 0, &zErrMsg);
    } else {
        std::cerr << "Can't delete from Answers table:" << sqlite3_errmsg(db) << endl;
    }

    if (SQLITE_OK == rc) {
        std::string query = "INSERT INTO Answers (Id, Type, Keywords, Message) " \
            "VALUES (1, 'Affirmative', 'rich,wealthy', 'It is certain'); " \
            "INSERT INTO Answers (Id, Type, Keywords, Message) "  \
            "VALUES (2, 'Affirmative', 'handsome,pretty', 'It is decidedly so.'); " \
            "INSERT INTO Answers (Id, Type, Keywords, Message) "  \
            "VALUES (3, 'Affirmative', 'job,career', 'Without a doubt.'); " \
            "INSERT INTO Answers (Id, Type, Keywords, Message) "  \
            "VALUES (4, 'Affirmative', 'love,relationship', 'Yes, definitely.'); " \
            "INSERT INTO Answers (Id, Type, Keywords, Message) "  \
            "VALUES (5, 'Affirmative', 'friend,mate', 'You may rely on it.'); " \
            "INSERT INTO Answers (Id, Type, Keywords, Message) "  \
            "VALUES (6, 'Affirmative', 'married,divorced,kids', 'As I see it, yes.'); " \
            "INSERT INTO Answers (Id, Type, Keywords, Message) "  \
            "VALUES (7, 'Affirmative', 'car,house', 'Most likely.'); " \
            "INSERT INTO Answers (Id, Type, Keywords, Message) "  \
            "VALUES (8, 'Affirmative', 'weather,hot,cold', 'Outlook good.'); " \
            "INSERT INTO Answers (Id, Type, Keywords, Message) "  \
            "VALUES (9, 'Affirmative', 'sure,really', 'Yes.'); " \
            "INSERT INTO Answers (Id, Type, Keywords, Message) "  \
            "VALUES (10, 'Affirmative', 'direction,point,locaion', 'Signs point to yes.'); " \
            "INSERT INTO Answers (Id, Type, Keywords, Message) "  \
            "VALUES (11, 'Non Commital', 'weather,friend,best', 'Reply hazy, try again.'); " \
            "INSERT INTO Answers (Id, Type, Keywords, Message) "  \
            "VALUES (12, 'Non Commital', 'rich,money,wealthy', 'Ask again later.'); " \
            "INSERT INTO Answers (Id, Type, Keywords, Message) "  \
            "VALUES (13, 'Non Commital', 'death,dead,body,live', 'Better not tell you now.'); " \
            "INSERT INTO Answers (Id, Type, Keywords, Message) "  \
            "VALUES (14, 'Non Commital', 'job,career,married,divorced,kids', 'Cannot predict now.'); " \
            "INSERT INTO Answers (Id, Type, Keywords, Message) "  \
            "VALUES (15, 'Non Commital', 'think,look,life,body', 'Concentrate and ask again.'); " \
            "INSERT INTO Answers (Id, Type, Keywords, Message) "  \
            "VALUES (16, 'Negative', 'rich,wealthy,lottery', 'Dont count on it.'); " \
            "INSERT INTO Answers (Id, Type, Keywords, Message) "  \
            "VALUES (17, 'Negative', 'job,career,money,car,house', 'My reply is no.'); " \
            "INSERT INTO Answers (Id, Type, Keywords, Message) "  \
            "VALUES (18, 'Negative', 'divorced,married,kids,team,win,lose', 'My sources say no.'); " \
            "INSERT INTO Answers (Id, Type, Keywords, Message) "  \
            "VALUES (19, 'Negative', 'long,live,life,job', 'Outlook not so good.'); " \
            "INSERT INTO Answers (Id, Type, Keywords, Message) "  \
            "VALUES (20, 'Negative', 'lottery,ticket,win', 'Very doubtful.'); ";
        rc = sqlite3_exec(db, query.c_str(), NULL, 0, &zErrMsg);
    } else {
        std::cerr << "Can't INSERT data into Answers table:" << sqlite3_errmsg(db) << endl;
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
    // random "Id" of answer
    int index = rand() % SIZEOF_ANSWERS;
    std::string answer = "I have no answer";
    std::string query = "SELECT Message FROM Answers WHERE id = " + std::to_string(index);
    if (getDebug()) { cout << "SQL to execute is: " << query << endl; }

    sqlite3_prepare(db, query.c_str(), -1, &stmt, NULL);

    char buff[100];
	while (sqlite3_step(stmt) != SQLITE_DONE) {
		int i;
		int num_cols = sqlite3_column_count(stmt);
		
		for (i = 0; i < num_cols; i++) {
			switch (sqlite3_column_type(stmt, i)) {
			case (SQLITE3_TEXT):
                sprintf(buff, "%s", sqlite3_column_text(stmt, i));
                answer = buff;
				break;
			case (SQLITE_INTEGER):
			case (SQLITE_FLOAT):
				break;
			default:
				break;
			}
		}
	}

	sqlite3_finalize(stmt);
    return answer;
}

string AnswerSQL::getAnswerFromKeywords(std::vector<std::string> keywords) {
    std::string answer = "I have no answer";
    std::string query = "SELECT * FROM Answers WHERE ";
    for (int i = 0; i < keywords.size(); i++) {
        query = query + "Keywords LIKE '%" + keywords[i] + "%'";
        if (i != (keywords.size()-1)) query += " OR ";
    }
    query += ";";
    if (getDebug()) { cout << "SQL to execute is: " << query << endl; }

    sqlite3_prepare(db, query.c_str(), -1, &stmt, NULL);

    char buff[100];
	while (sqlite3_step(stmt) != SQLITE_DONE) {
		int i;
		int num_cols = sqlite3_column_count(stmt);
		
		for (i = 0; i < num_cols; i++) {
			switch (sqlite3_column_type(stmt, i)) {
			case (SQLITE3_TEXT):
                sprintf(buff, "%s", sqlite3_column_text(stmt, i));
                answer = buff;
				break;
			case (SQLITE_INTEGER):
			case (SQLITE_FLOAT):
                // TODO: we should error here
				break;
			default:
				break;
			}
		}
	}

	sqlite3_finalize(stmt);
    return answer;
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