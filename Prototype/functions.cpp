#include "user.h"
#include "game.h"
#include "functions.h"
#include <iostream>
#include <vector>

// Log in to an existing user account
void login(){

}

// Logout function
void logout(bool& logged_in,
            std::string& current_username,
            std::string& current_user_type) {
    logged_in = false;
    current_username = "";
    current_user_type = "";

    std::cout << "Logout successful\n";
}

// listAccounts function
void listAccounts(const std::vector<User> users) {

    if (users.empty()) {
        std::cout << "No accounts found." << std::endl;
        return;
    }

    for (const User user : users) {
        std::cout << "Username: " << user.getUsername()
                  << ", Type: " << user.getUserType()
                  << ", Credit: $" << user.getCredit()
                  << std::endl;
    }
}

// Create a new user account
User* createUser(User curUser, // The user executing the command
        const std::vector<User> users, // The existing user list
        std::string username, 
        std::string password, 
        std::string email, 
        std::string type){
    
    // Confirm that the current user is an admin
    if(!(curUser.getUserType()=="admin")){
        return nullptr;
    }
    
    // Check that the user being created has a unique username
    for(User u : users){
        if(u.getUsername()==username){
            return nullptr;
        }
    }

    // Check email, password, and user type format
    if(!passwordValid || !emailValid || !typeValid){
        return nullptr;
    }

    //Create and return the user
    return &User(username, type, email, password);
}

// Remove an existing user
void deleteUser(){

}

// Put a game up for sale
void sell(){

}

// Purchase a game that is up for sale
void buy(){

}

// Refund a user's purchase
void refund(){

}

//
// HELPER FUNCTIONS
//

// Requirements for a valid password
bool passwordValid(std::string password){
    return true;
}

// Requirements for a valid email
bool emailValid(std::string email){
    return true;
}

// Requirements for a valid user type
bool typeValid(std::string type){
    return true;
}