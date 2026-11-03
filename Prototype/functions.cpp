#include "user.h"
#include "game.h"
#include "functions.h"
#include <iostream>
#include <vector>
#include <iostream>
#include <fstream>

<<<<<<< HEAD
// Login function
bool login(const std::string &username,
           const std::vector<User> &users,
           bool &logged_in,
           std::string &current_username,
           std::string &current_user_type)
{

    // Check if a user is already logged in
    if (logged_in)
    {
        std::cout << "Login failed: user is already logged in\n";
        return false;
    }

    // Search for the username
    for (const User &user : users)
    {
        if (user.getUsername() == username)
        {
            logged_in = true;
            current_username = user.getUsername();
            current_user_type = user.getUserType();

            std::cout << "Login successful\n";
            return true;
        }
    }

    // Username was not found
    std::cout << "Login failed: user does not exist\n";
    return false;
=======
// Log in to an existing user account
void login(){

>>>>>>> e24558dca05962a3e976b3ba220fafd7fe9644b4
}

// Logout function
void logout(bool &logged_in,
            std::string &current_username,
            std::string &current_user_type)
{
    logged_in = false;
    current_username = "";
    current_user_type = "";

    std::cout << "Logout successful\n";
}

// listAccounts function
<<<<<<< HEAD
void listAccounts(const std::vector<User> &users)
{
=======
void listAccounts(const std::vector<User> users) {
>>>>>>> e24558dca05962a3e976b3ba220fafd7fe9644b4

    if (users.empty())
    {
        std::cout << "No accounts found." << std::endl;
        return;
    }

<<<<<<< HEAD
    for (const User &user : users)
    {
=======
    for (const User user : users) {
>>>>>>> e24558dca05962a3e976b3ba220fafd7fe9644b4
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

// Build the file from a list of user accounts
void currentUserAccountsConstructor(){
    // Open file
    std::ofstream file("assets/");

    // Write to file if open
    if(file.is_open()){
        
        // Write all output pairings
        for(const auto& [key, value] : outputs){
            file << key << ": " << outputs[key];
        }

        // Write status
        file << "STATUS: " << status;

        // Close file
        file.close();
    }
}

// Get a list of user accounts from the file
void currentUserAccounts(){
    std::string file_name = "";
}

// Build the file from a list of available games
void availableGamesConstructor(){
    
}

// Get a list of games from the file
void availableGames(){
    std::string file_name = "";
}

// Build the file from a list of user accounts
void gameCollectionConstructor(){
    
}

// Get a list of games from the user's file
void gameCollection(User user){

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