#include <iostream>
#include <string>
#include <sstream> 
using namespace std;

#include "Topic.h"

Topic::Topic() : Message(){
}

Topic::Topic(const string &athr, const string &sbjct, const string &body, unsigned id) : 
Message(athr, sbjct, body, id){
}

bool Topic::isReply() const{
    return false;
}

string Topic::toFormattedString() const{
    string finalString = "<begin_topic>\n";
    finalString += Message::toFormattedString();
    return finalString;
}