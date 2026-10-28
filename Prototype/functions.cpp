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
bool createUser(User current_user, // The user executing the command
        std::vector<User>* users, // Existing user list, passed by reference
        std::string username, 
        std::string password, 
        std::string email, 
        std::string type){
    
    // Confirm that the current user is an admin
    if(!(current_user.getUserType()=="admin")){
        return false;
    }
    
    // Check that the user being created has a unique username
    for(auto it = users->begin(); it != users->end(); it++){
        if(it->getUsername()==username){
            return false;
        }
    }

    // Check email, password, and user type format
    if(!passwordValid || !emailValid || !typeValid){
        return false;
    }

    // Create the user
    users->push_back(User(username, type, email, password));
    return true;
}

// Remove an existing user
bool deleteUser(User current_user, // The user executing the command
        std::vector<User>* users,  // Existing user list, passed by reference
        std::string username // The username of the user to delete
    ){
    
    // Confirm that the current user is an admin
    if(!(current_user.getUserType()=="admin")){
        return false;
    }
    
    // Find and remove the user from the list
    for(auto it = users->begin(); it != users->end(); it++){
        if(it->getUsername()==username){
            users->erase(it);
            return true;
        }
    }

    return false;
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