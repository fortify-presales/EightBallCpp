#include "stdafx.h"
//#include "pugixml.hpp"
#include <zlib.h>

using namespace std;
//using namespace pugi;

string getAnswer();

const string exitString = "x";
const int SIZEOF_ANSWERS = 20;
const int BUFSIZE = 255;
const bool debug = false;
const string magicEightBall = "The Magic 8 Ball says:";

int main(int argc, char* argv[]) {
	
	/*
    char buffer_in[256] = {"Conan is a MIT-licensed, Open Source package manager for C and C++ development, "
                           "allowing development teams to easily and efficiently manage their packages and "
                           "dependencies across platforms and build systems."};
    char buffer_out[256] = {0};

    z_stream defstream;
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;
    defstream.avail_in = (uInt)std::strlen(buffer_in);
    defstream.next_in = (Bytef*)buffer_in;
    defstream.avail_out = (uInt)sizeof(buffer_out);
    defstream.next_out = (Bytef*)buffer_out;

    deflateInit(&defstream, Z_BEST_COMPRESSION);
    deflate(&defstream, Z_FINISH);
    deflateEnd(&defstream);

    std::cout << "Uncompressed size is: " << std::strlen(buffer_in) << std::endl;
    std::cout << "Compressed size is: " << defstream.total_out << std::endl;

    std::cout << "ZLIB VERSION: " << zlibVersion() << std::endl;

    int length = 10;
    int* memory = new int[length];
    for (int i = 0; i < length; i++) {
        memory[i] = i;
    }
    delete[] memory;
	*/
	
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
        cout << getAnswer() << endl;
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
                cout << getAnswer() << endl;
            }
        }
    }
	
    return EXIT_SUCCESS;
}

string getAnswer()
{
	return "I have no answer";
}
	
/*
string getAnswer()
{
    int index = rand() % SIZEOF_ANSWERS;
    char buffer[BUFSIZ];
    char* answer_file = getenv("EIGHTBALL_ANSWERS");
    sprintf(buffer, "/Answers/Answer[@Id='%d']", index);

    xml_document doc;
    xml_parse_result result = doc.load_file("eightball.xml");

    if (debug) {
        if (result)
        {
            cout << "XML [" << "eightball.xml" << "] parsed without errors, version: [" << doc.child("Answers").attribute("version").value() << "]\n\n";
        }
        else
        {
            cout << "XML [" << "eightball.xml" << "] parsed with errors, version: [" << doc.child("Answers").attribute("version").value() << "]\n";
            cout << "Error description: " << result.description() << "\n";
            cout << "Error offset: " << result.offset << " (error at [..." << ("eightball.xml" + result.offset) << "]\n\n";
        }
    }

    xpath_node_set answer = doc.select_nodes(buffer);

    for (xpath_node_set::const_iterator it = answer.begin(); it != answer.end(); ++it)
    {
        xpath_node node = *it;
        return node.node().attribute("Message").value();
    }

    return "I have no answer";
}
*/