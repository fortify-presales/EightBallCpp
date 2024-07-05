#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#ifdef _WIN32
    #include <io.h>
#endif
#include <sys/stat.h>
#include <sys/types.h>
#include <zlib.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "Answer.hpp"
#include "AnswerSQL.hpp"
#include "AnswerXML.hpp"
#include "Shell.hpp"

using namespace std;

const string exitString = "x";
const int BUFSIZE = 255;
const string magicEightBall = "The Magic 8 Ball says: ";
const char encryptionKey[] = "lakdsljkalkjlksdfkl";

bool debug = false;
bool useDb = false;

string getVersion();

int main(int argc, char* argv[]) {

    cout << std::boolalpha;  

    char* path = getenv("PATH");
    if (getenv("8BALL_DEBUG")) { debug = true; };
    if (getenv("8BALL_USEDB")) { useDb = true; }

    SSL_library_init();

    cout << " .-'''-." << endl;
    cout << "/   _   \\" << endl;
    cout << "|  (8)  | Magic 8 Ball - Version " << getVersion() << endl;
    cout << "\\   ^   /" << endl;
    cout << " '-...-'" << endl;

    if (debug) { 
        cout << "  - ZLIB: " << zlibVersion() << endl;
        cout << "  - DEBUG: " << debug << endl;
        cout << "  - USEDB: " << useDb << endl;
        cout << " ---" << endl;
    }

    Shell shell;
    AnswerXML aXML = AnswerXML(debug);
    AnswerSQL aSQL = AnswerSQL(debug);
	char keywords[BUFSIZE] = "";
    
    srand(time(0));

    if (argc >= 2) {
        cout << "You have entered a question with " << argc-1 << " words: ";
        for (int i = 1; i < argc; ++i) {
            char* newArray = (char*)malloc (strlen(keywords) + strlen(argv[i]) + 2);
            strcpy(newArray, keywords);
            strcat(newArray, argv[i]);
            strcat(newArray, (i == argc-1 ? "?" : " "));
            strcpy(keywords, newArray);
            free(newArray);
        }
        keywords[0] = toupper(keywords[0]);

        cout << keywords << endl;
        if (useDb) {
            cout << magicEightBall << aSQL.getRandomAnswer() << endl;
        } else {
            cout << magicEightBall << aXML.getRandomAnswer() << endl;
        }    
    } else {
        bool keepGoing = true;

        while (keepGoing) {
            string question;

            // prompt for and get the question
            cout << "What is your question?  (Enter 'x' to exit)" << endl;
            getline(cin, question);

            // this assumes that the user enters a lower case x
            if (question.compare(exitString) == 0)
                keepGoing = false;
            else {
                // log questions
                umask(0);
                shell.execute("echo " + question + " >> questions.txt");
                cout << magicEightBall;
                string ans;
                if (useDb) { 
                    ans = aSQL.getAnswerFromKeywords(question);
                } else {
                    ans = aXML.getAnswerFromKeywords(question);
                }    
                cout << ans << endl;
            }
        }
    }
	
    return EXIT_SUCCESS;
}

string getVersion() {
    return "1.0";
}