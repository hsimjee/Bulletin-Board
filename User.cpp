#include <iostream>
using namespace std;

#include "User.h"

User::User(){
    username = "";
    password = "";
    return;
}

User::User(const string& uname, const string& pass){
    username = uname;
    password = pass;
    return;
}

string User::getUsername() const{
    return username;
}

bool User::check(const string &uname, const string &pass) const{
    if (uname == "")
        return false;
    if (uname == username && pass == password)
        return true;
    return false;
}

bool User::setPassword(const string &oldpass, const string &newpass){
    if (username == "")
        return false;
    if (oldpass == password){
        password = newpass;
        return true;
    }
    return false;
}