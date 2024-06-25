#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>

#include <zlib.h>
#include "Answer.h"
#include "Shell.h"
#include "Database.h"

using namespace std;

const string exitString = "x";
const int BUFSIZE = 255;
const bool debug = false;
const bool useDb = false;
const string magicEightBall = "The Magic 8 Ball says: ";
char encryptionKey[] = "lakdsljkalkjlksdfkl";

string getVersion();

int main(int argc, char* argv[]) {

    char* path = getenv("PATH");

    cout << "MAGIC 8 BALL VERSION:" << getVersion() << endl;
    cout << "  - using ZLIB: " << zlibVersion() << endl;
    if (debug) { cout << "PATH: " << path << endl;}
    cout << "--------------------" << endl;

    Answer answer;
    Shell shell;
    Database database;
	char keywords[BUFSIZE] = "";
    
    srand(time(0));

    if (argc >= 2) {
        cout << "You have entered a question with " << argc-1 << " words: ";
        for (int i = 1; i < argc; ++i) {
            char* newArray = new char[strlen(keywords) + strlen(argv[i]) + 2];
            strcpy(newArray, keywords);
            strcat(newArray, argv[i]);
            strcat(newArray, (i == argc-1 ? "?" : " "));
            strcpy(keywords, newArray);
            //delete[] newArray;
        }
        keywords[0] = toupper(keywords[0]);

        cout << keywords << endl;
        cout << magicEightBall << answer.getAnswer() << endl;
    } else {
        bool keepGoing = true;

        while (keepGoing)
        {
            string question;

            // prompt for and get the question
            cout << "What is your question?  (Enter 'x' to exit)" << endl;
            getline(cin, question);

            // this assumes that the user enters a lower case x
            if (question.compare(exitString) == 0)
                keepGoing = false;
            else
            {
                // log questions
                shell.execute("echo " + question + " >> questions.txt");
                cout << magicEightBall;
                string ans;
                if (useDb) { 
                    ans = database.getAnswer(question);
                } else {
                    ans = answer.getAnswer();
                }    
                cout << ans << endl;
            }
        }
    }
	
    return EXIT_SUCCESS;
}

string getVersion() 
{
    return "1.0";
}