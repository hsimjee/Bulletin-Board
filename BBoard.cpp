#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

#include "BBoard.h"

BBoard::BBoard(){
    title = "CS12 Bulletin Board";
    currentUser = 0;
    messageList.clear();
    return;
}

BBoard::BBoard(const string &str){
    title = str;
    currentUser = new User();
    messageList.clear();
    return;
}

BBoard::~BBoard(){
    for (unsigned int i = 0; i < messageList.size(); i++){
        delete messageList.at(i);
    }
    
}

bool BBoard::loadUsers(const string &file){
    string nm;
    string ps;
    ifstream inFS;
    
    inFS.open(file.c_str());
    if (!inFS.is_open()){
        return false;
    }
    while (inFS >> nm){
        inFS >> ps;
        addUser(nm, ps);
    }
    inFS.close();
    
    if(userList.size() > 0)
        return true;
    return false;
}

bool BBoard::loadMessages(const string &datafile){
    unsigned id;
    string sbjct;
    string athr;
    string body;
    string input;
    string messageType;
    string inputTitle;
    string children;
    unsigned count = 0;
    int childVecInput;
    unsigned numMessages = 0;
    ifstream inFS;
    stringstream inputStream;
    stringstream childrenID;
    bool repeat = true;
    vector< vector<int> > tempChildrenVector;
    vector<int> childLoc;
    
    inFS.open(datafile.c_str());

    if(!inFS.is_open()) {
        cout << "Error opening file" << endl;
        return false;
    }
    
    getline(inFS, input);
    inputStream.str(input);
    inputStream >> numMessages;
    inputStream.clear();
    messageList.resize(numMessages);
    tempChildrenVector.resize(numMessages);
    
    while(getline(inFS, input)){
        messageType = input;
        if(count != numMessages){
            repeat = true;
        }
        while(repeat){
            getline(inFS, input);
            inputStream.str(input);   
            inputStream >> inputTitle;
            if (inputTitle == ":id:"){
                inputStream >> id;
            }
                
            if(inputTitle == ":subject:"){
                getline(inputStream, sbjct);
            }
            
            if(inputTitle == ":from:"){
                getline(inputStream, athr);
            }
            
            if (inputTitle == ":children:"){
                getline(inputStream, children);
                    
                istringstream childrenID(children);
                
                childLoc.clear();
                
                while(childrenID >> childVecInput){
                    childLoc.push_back(childVecInput);
                }
                
                tempChildrenVector.at(id - 1) = childLoc;
                childrenID.clear();
            }
            
            if(inputTitle == ":body:"){
                getline(inputStream, body);
                string temp;
                while (temp != "<end>"){
                    getline(inFS, temp);
                    if (temp != "<end>"){
                        body += "\n" + temp;
                    }
                }
                repeat = false;
                count++;
            }
            inputStream.clear();
        }
        
        sbjct = sbjct.substr(1);
        athr = athr.substr(1);
        body = body.substr(1);
        
        if(messageType == "<begin_topic>"){
            Topic* temp = new Topic(athr, sbjct, body, id);
            messageList.at(id - 1) = temp;
        }
            
        else if(messageType == "<begin_reply>"){
            Reply* temp = new Reply(athr, sbjct, body, id);
            messageList.at(id - 1) = temp;
        }
    }
    
    for (unsigned int i = 0; i < messageList.size(); i++){
        for (unsigned int j = 0; j < (tempChildrenVector.at(i)).size(); j++){
            int childIndex = (tempChildrenVector.at(i)).at(j) - 1;
            messageList.at(i) -> addChild(messageList.at(childIndex));
        }
    }
    
    return true;
}


bool BBoard::saveMessages(const string& datafile){
    int numMessages = messageList.size();
    ofstream outFS;
    outFS.open(datafile.c_str());
    
    if(!outFS.is_open() || (numMessages == 0)){
        return false;
    }

    outFS << numMessages << endl;
    
    for (unsigned int i = 0; i < messageList.size(); i++){
        outFS << messageList.at(i) -> toFormattedString();
    }
    
    outFS.close();
    return true;
}

void BBoard::login(){
    cout << "Welcome to " << title << endl;
    
    while (true){
    string name;
    string pas;
    
    cout << "Enter your username ('Q' or 'q' to quit): ";
    cin >> name;
    
    if (name == "Q" || name == "q"){
        cout << "Bye!" << endl;
        currentUser = new User();
        return;
    }
    
    cout << "Enter your password: ";
    cin >> pas;
    
    if (getUser(name, pas) != NULL){
        cout << endl;
        currentUser = getUser(name, pas);
        cout << "Welcome back " << currentUser -> getUsername() << "!" << endl;
        return;
    }
    
    else{
        cout << "Invalid Username or Password!" << endl;
        cout << endl;
    }
    }
    
    return;
}

void BBoard::run(){
    
    if (currentUser -> getUsername() == "" || currentUser == 0 || 
    currentUser == NULL){
        return;
    }
        
    char sel;
    
    while (true){
    cout << endl;
    cout << "Menu" << endl;
    cout << "- Display Messages ('D' or 'd')" << endl;
    cout << "- Add New Topic ('N' or 'n')" << endl;
    cout << "- Add Reply to a Topic ('R' or 'r')" << endl;
    cout << "- Quit ('Q' or 'q')" << endl;
    cout << "Choose an action: ";
    
    cin >> sel;
    
    if (sel == 'Q' || sel == 'q'){
        cout << "Bye!" << endl;
        return;
    }
    if (sel == 'N' || sel == 'n')
        addTopic();
    if (sel == 'R' || sel == 'r')
        addReply();
    if (sel == 'D' || sel == 'd'){
        cout << endl;
        display();
    }
    }
}

void BBoard::display() const{
    if (messageList.empty()){
        cout << "Nothing to Display.";
        cout << endl;
        return;
    }

    for (unsigned int i = 0; i < messageList.size(); i++){
        if(!(messageList.at(i) -> isReply()) && messageList.at(i) != NULL){
            cout << "---------------------------------------------------------" << endl;
            messageList.at(i) -> print(0);
        }
    }
    cout << "---------------------------------------------------------" << endl;
    return;
}

void BBoard::addUser(const string &name, const string &pass){
    User newUser;
    newUser = User(name, pass);
    userList.push_back(newUser);
    return;
}

const User* BBoard::getUser(const string &name, const string &pw) const{
    const User* currUser;
    for (unsigned int i = 0; i < userList.size(); i++){
        if ((userList.at(i)).check(name, pw)){
            currUser = &userList.at(i);
            return currUser;
        }
    }
            
    return NULL;
}

void BBoard::addTopic(){
    string sub;
    string bdy;
    string body = "";
    string nm;
    int id = messageList.size() + 1;
    
    nm = currentUser -> getUsername();
    
    cout << "Enter Subject: ";
    cin.ignore();
    getline(cin, sub);
    
    cout << "Enter Body: ";
    getline(cin, bdy);
    
    while(bdy != ""){
        body += bdy;
        getline(cin, bdy);
        if(bdy != ""){
            body += "\n";
        }
    }
    
    Topic* temp = new Topic(nm, sub, body, id);
    
    messageList.push_back(temp);
    return;
}

void BBoard::addReply(){
    string sub = "Re: ";
    string bdy;
    string body = "";
    string nm;
    int id;
    unsigned replyID;
    bool repeat = true;
    
    nm = currentUser -> getUsername();
   
    
    while(repeat){
        cout << endl << "Enter Message ID (-1 for Menu): ";
        cin >> id;
        
        int compareToID = messageList.size();
        
        if (id <= -1){
            return;
        }
    
        if (id > compareToID){
            cout << "Invalid Message ID!!" << endl;
            cout << "---------------------------------------------------------" << endl;
        }
        
        else
            repeat = false;
    }
    
    sub += messageList.at(id - 1) -> getSubject();
    
    cout << "Enter Body: ";
    cin.ignore();
    getline(cin, bdy);
    
    while(bdy != ""){
        body += bdy;
        getline(cin, bdy);
        if(bdy != ""){
            body += "\n";
        }
    }
    
    replyID = messageList.size() + 1;
    
    Reply* temp = new Reply(nm, sub, body, replyID);
    
    messageList.push_back(temp);
    messageList.at(id - 1) -> addChild(temp);
    
    cout << "Message Recorded!" << endl;
    return;
}