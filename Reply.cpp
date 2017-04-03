#include <iostream>
#include <string>
#include <sstream> 
using namespace std;

#include "Reply.h"


Reply::Reply() : Message(){
}

Reply::Reply(const string &athr, const string &sbjct, const string &body, unsigned id) : 
Message(athr, sbjct, body, id){
}

bool Reply::isReply() const{
    return true;
}

string Reply::toFormattedString() const{
    string finalString = "<begin_reply>\n";
    finalString += Message::toFormattedString();
    return finalString;
}