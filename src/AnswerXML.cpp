// AnswerXML.cpp

#include "AnswerXML.hpp"

#include "pugiconfig.hpp"
#include "pugixml.hpp"

using namespace std;
using namespace pugi;

const int SIZEOF_ANSWERS = 20;

AnswerXML::AnswerXML() { }

AnswerXML::AnswerXML(bool debug) {
    Answer::setDebug(debug);
}

AnswerXML::~AnswerXML() { }

string AnswerXML::getRandomAnswer() {
    int index = rand() % SIZEOF_ANSWERS;
    char buffer[BUFSIZ];
    char* answer_file = getenv("EIGHTBALL_ANSWERS");
    sprintf(buffer, "/Answers/Answer[@Id='%d']", index);

    xml_document doc;
    xml_parse_result result = doc.load_file("../data/eightball.xml");

    if (getDebug()) {
        if (result) {
            cout << "XML [" << "eightball.xml" << "] parsed without errors, version: [" << doc.child("Answers").attribute("version").value() << "]\n\n";
        } else {
            cout << "XML [" << "eightball.xml" << "] parsed with errors, version: [" << doc.child("Answers").attribute("version").value() << "]\n";
            cout << "Error description: " << result.description() << "\n";
            cout << "Error offset: " << result.offset << " (error at [..." << ("eightball.xml" + result.offset) << "]\n\n";
        }
    }

    xpath_node_set answer = doc.select_nodes(buffer);

    for (xpath_node_set::const_iterator it = answer.begin(); it != answer.end(); ++it) {
        xpath_node node = *it;
        return node.node().attribute("Message").value();
    }

    return "I have no answer";
}

string AnswerXML::getAnswerFromKeywords(std::vector<std::string> keywords) {
    // TBD
    return getRandomAnswer();
}

void AnswerXML::doSomethingElse() {
    int length = 10;
    int* memory = new int[length];
    for(int i = 0; i < length + 1; i++) {
        memory[i] = i;  // out-of-bounds write
    }
}
