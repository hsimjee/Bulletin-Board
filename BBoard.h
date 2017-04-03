//BBoard.h
#include <string>
#include <vector>
using namespace std;

#include "Message.h"
#include "User.h"
#include "Topic.h"
#include "Reply.h"

class BBoard {
 private:
    string title;
    vector<User> userList;
    const User* currentUser;
    vector<Message*> messageList;

 public:
    // Constructs a board with a default title, 
    // empty user & message lists, 
    // and the "default" User
    BBoard();

    // Same as the default constructor except 
    // it sets the title of the board
    BBoard(const string &);

    // NEW
    // destructor that deallocates all Messages pointed to by messageList 
    ~BBoard();

    // Imports all the authorized users from an input file, 
    // storing them as User objects in the vector userList
    // The name of the input file is passed in as a parameter to this function. 
    // See below for file format.
    bool loadUsers(const string &);

    // NEW
// This function gets a filename (datafile) of a file that stores the messages from
// previous sessions in the given format (See File Format Specs).
// It opens and reads the file, creating Topic and Reply objects as appropriate, and
// fills the messageList vector (note: remember that messageList is a vector of 
// Message pointers, not Messages).
// If an error occurs when opening the file, it returns false. Returns true otherwise.
    bool loadMessages(const string &datafile);

//NEW
// This is the final action of the bulletin board before closing:
// It gets a filename (datafile) of a file that will store all of the the messages, 
// and after opening the file writes the messages into it with the same format.
// If an error occurs when opening the file, it returns false. Returns true otherwise.
// (Note: Since you will be opening the file for writing - i.e. an ofstream - 
// "file not found" is NOT an error; a new file will simply be created for you).
    bool saveMessages(const string &datafile);

    //NEW
//  asks for and validates current user/password
// This function asks for a username and password, and checks the userList vector 
// for a matching User.
// If a match is found, it sets currentUser to the identified User from the list 
// (remember, currentUser is now a pointer, not an actual object).
// If not found, it will keep asking until a match is found or the user types: 'q' 
// or 'Q' as the username (you may assume we do not have a member with name 'q' or 'Q')
// When the user chooses to quit, say "Bye!" and just return from the login function
// - meaning that in main(), currentUser will be unchanged from its initial value,
// which should have been set to 0 or nullptr.
    void login();

    // Contains main loop of Bulletin Board.
    // First verifies a User has been logged in.
    // Exits **function** immediately if no User logged in (Default User).
    // Continues to display menu options to user and performs requested action
    // until user chooses to quit.
    // See output samples for exact format of menu.
    void run();

 private:
 //CHANGED
    void display() const;
    //NEW
    void addUser(const string &name, const string &pass);
    //NEW
    const User * getUser(const string &name, const string &pw) const;
    //NEW
    void addTopic();
    //NEW
    void addReply();
};

