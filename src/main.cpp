#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <vector>
#ifdef _WIN32
    #include <io.h>
#endif
#include <sys/stat.h>
#include <sys/types.h>
#include <zlib.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <boost/algorithm/string.hpp>

#include "Answer.hpp"
#include "AnswerSQL.hpp"
#include "AnswerXML.hpp"
#include "Shell.hpp"

const string exitString = "x";
const int BUFSIZE = 255;
const string magicEightBall = "The Magic 8 Ball says: ";
const char encryptionKey[] = "lakdsljkalkjlksdfkl";

bool debug = false;
bool useDb = true;  // default is to use locally created SQLite database

string getVersion();

int main(int argc, char* argv[]) {

    std::cout << std::boolalpha;  

    char* path = getenv("PATH");
    if (getenv("8BALL_DEBUG")) { debug = true; };

    SSL_library_init();

    std::cout << " .-'''-." << std::endl;
    std::cout << "/   _   \\" << std::endl;
    std::cout << "|  (8)  | Magic 8 Ball - Version " << getVersion() << std::endl;
    std::cout << "\\   ^   /" << std::endl;
    std::cout << " '-...-'" << std::endl;

    if (debug) { 
        std::cout << "  - ZLIB: " << zlibVersion() << std::endl;
        //std::cout << "  - PATH: " << path;
        std::cout << " ---" << std::endl;
    }

    Shell shell;
    AnswerXML aXML = AnswerXML(debug);
    AnswerSQL aSQL = AnswerSQL(debug);
    vector<std::string> keywords;
    srand(time(0));

    if (argc >= 2) {
        std::cout << "You have entered a question with " << argc-1 << " words." << endl;
        for (int i = 1; i < argc; ++i) {
            keywords.push_back(argv[i]);
        }
        if (useDb) {
            std::cout << magicEightBall << aSQL.getAnswerFromKeywords(keywords) << std::endl;
        } else {
            std::cout << magicEightBall << aXML.getAnswerFromKeywords(keywords) << std::endl;
        }    
    } else {
        bool keepGoing = true;

        while (keepGoing) {
            string question;

            // prompt for and get the question
            std::cout << "What is your question? (Enter 'x' to exit)" << std::endl;
            std::getline(cin, question);
            // split the question into keywords
            boost::split(keywords, question, boost::is_any_of(" "));

            // this assumes that the user enters a lower case x
            if (question.compare(exitString) == 0)
                keepGoing = false;
            else {
                // log questions
                umask(0);
                shell.execute("echo " + question + " >> questions.txt");
                if (useDb) {
                    std::cout << magicEightBall << aSQL.getAnswerFromKeywords(keywords) << std::endl;
                } else {
                    std::cout << magicEightBall << aXML.getAnswerFromKeywords(keywords) << std::endl;
                }   
            }
        }
    }
	
    return EXIT_SUCCESS;
}

string getVersion() {
    return "1.0";
}