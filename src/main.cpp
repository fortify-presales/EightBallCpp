#include <stdio.h>
#include <tchar.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>

#include <zlib.h>
#include "Answer.h"

using namespace std;

const string exitString = "x";
const int BUFSIZE = 255;
const bool debug = false;
const string magicEightBall = "The Magic 8 Ball says:";

int main(int argc, char* argv[]) {

    cout << "MAGIC 8 BALL" << endl;
    cout << "ZLIB VERSION: " << zlibVersion() << endl;

    int length = 10;
    int* memory = new int[length];
    for(int i = 0; i < length + 1; i++) {
        memory[i] = i;  // out-of-bounds write
    }
    // memory leak
	// delete[] memory;

    Answer answer;
	char keywords[BUFSIZE] = "";
    
    srand(time(0));

    if (argc >= 2) {
        cout << "You have entered a question with " << argc-1 << " words:" << "\n";
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
        cout << magicEightBall << endl;
        cout << answer.getAnswer() << endl;
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
                cout << magicEightBall << endl;
                cout << answer.getAnswer() << endl;
            }
        }
    }
	
    return EXIT_SUCCESS;
}