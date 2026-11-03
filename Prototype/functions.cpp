#include "user.h"
#include "game.h"
#include "functions.h"
#include <iostream>
#include <vector>
#include <cmath>

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
void listAccounts(const std::vector<User> &users)
{

    if (users.empty())
    {
        std::cout << "No accounts found." << std::endl;
        return;
    }

    for (const User &user : users)
    {
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
 // TODO: Write successful sell transaction to daily transaction file.
 // TODO: Do not make the game available until the next session.

void sell(bool logged_in,
          const std::string& current_user_type,
          const std::string& gamename,
          const std::string& gamedescription,
          double gameprice,
          const std::vector<Game>& available_games,
          std::vector<Game>& pending_games)
{
    // Check if the user is logged in
    if (!logged_in) {
        std::cout << "Error: User must be logged in to sell a game.\n";
        return;
    }

    // Standard-buy accounts cannot sell games
    if (current_user_type == "BS") {
        std::cout << "Error: Standard-buy accounts cannot sell games.\n";
        return;
    }

    // Check if the game name is valid
    if (gamename.empty() || gamename.length() > 25) {
        std::cout << "Error: Game name must be between 1 and 25 characters.\n";
        return;
    }

    // Check if the price is valid
    if (!std::isfinite(gameprice) ||
        gameprice < 0 || gameprice > 999.99) {
        std::cout << "Error: Game price must be between $0.00 and $999.99.\n";
        return;
    }

    // Check if the game name already exists
    for (const Game& game : available_games) {
        if (game.getGameName() == gamename) {
            std::cout << "Error: Game name already exists.\n";
            return;
        }
    }

    // Check games listed during the current session
    for (const Game& game : pending_games) {
        if (game.getGameName() == gamename) {
            std::cout << "Error: Game name already exists.\n";
            return;
        }
    }

    // Create a new game
    Game new_game(gamename, gamedescription, gameprice);

    // TODO: Save the sell transaction to the daily transaction file.
    // Only record the game as pending after the file write succeeds.

    std::cout << "Game passed validation, but the sale "
              << "has not been saved yet.\n";
}


// Purchase a game that is up for sale
void buy(){

}

// Refund a user's purchase
void refund(){

}

// Build the file from a list of user accounts
void currentUserAccountsConstructor(){

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