#include <iostream>
using namespace std;

#include "Message.h"

Message::Message(){
    author = "";
    subject = "";
    body = "";
    id = 0;
    return;
}

Message::Message(const string &athr, const string &sbjct, const string &bod, unsigned ids) :
    author(athr), subject(sbjct), body(bod), id(ids){
}

Message::~Message(){
    if(childList.size() != 0){
        for(unsigned int i = 0; i < childList.size(); i++)
            delete childList.at(i);
    }
}

string Message::toFormattedString() const{
    string finalString;
    stringstream formatted;

    formatted << ":id: " << id << "\n:subject: " << subject << 
    "\n:from: " << author + "\n";
    
    if(childList.size() != 0){
        formatted << ":children:";
        for(unsigned int i = 0; i < childList.size(); i++){
            formatted << " " << childList.at(i) -> getID();
        }
        formatted << "\n";
    }
    formatted << ":body: " + body  + "\n";
    formatted << "<end>" << "\n";

    finalString = formatted.str();
    return finalString;
}

void Message::print(unsigned indentation) const{
    string indentSpaces = "";
    string bodyLine;
    stringstream bodyString(body);
    
    for (unsigned int i = 0; i < indentation; i++){
        indentSpaces += "  ";
    }
    
    cout << indentSpaces << "Message #" << getID() << ": " << getSubject() << endl;
    
    getline(bodyString, bodyLine);
    cout << indentSpaces << "from " << author << ": " << bodyLine << endl;
    
    while(getline(bodyString, bodyLine)){
        cout << indentSpaces << bodyLine << endl;
    }
    
    if (!childList.empty()){
        for (unsigned int j = 0; j < childList.size(); j++){
            cout << endl;
            childList.at(j) -> print(indentation + 1);
        }
    }

    return;
}

const string& Message::getSubject() const{
    return subject;
}

unsigned Message::getID() const{
    return id;
}

void Message::addChild(Message *child){
    childList.push_back(child);
    return;
}