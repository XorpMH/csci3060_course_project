#include "user.h"
#include "game.h"
#include "functions.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include <iomanip>

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
void listAccounts(const std::vector<User> users) {

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
    if(!(current_user.getUserType()=="AA")){
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
    if(!(current_user.getUserType()=="AA")){
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
          const User current_user,
          const std::string& gamename,
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
    if (current_user.getUserType() == "BS") {
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
    Game new_game(gamename, current_user.getUsername(), gameprice);

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
// UUUUUUUUUUUUUUU_TT_CCCCCCCCC
void currentUserAccountsConstructor(std::vector<User>* users){
    // Open file
    std::ofstream file("assets/current_user_accounts.txt");

    // Write to file if open
    if(file.is_open()){

        // Write a line for each user
        for(auto it = users->begin(); it != users->end(); it++){
            
            // Write the username and pad anything shorter than 15 characters
            file << std::left << std::setw(15) << it->getUsername();
            
            file << " ";

            // Write user type as a 2 letter capitalized string
            file << it->getUserType();

            file << " ";

            // Write the user's credit to 2 decimal places
            file << std::left << std::setw(9) << std::fixed << std::setprecision(2) << it->getCredit();

            file << "\n";
        }

        // Close file
        file.close();
    }
}

// Get a list of user accounts from the file
std::vector<User> currentUserAccounts(){
    std::string file_path = "assets/current_user_accounts.txt";
    std::vector<User> users = {};

    try{
        // Open file
        std::ifstream file(file_path);

        // Confirm that filepath exists/file is open
        if(file.is_open()){
            
            // Loop through all lines and add users to the list
            std::string line;
            while(std::getline(file, line)){
                
                // Get user information from formatted line
                std::string username = rtrim(line.substr(0,15));
                std::string user_type = rtrim(line.substr(16,2));
                double credits = std::stod(rtrim(line.substr(19,9)));

                // Add user to list
                users.push_back(User(username, user_type, credits));
            }

            // Close file
            file.close();
        }
    }
    catch(...){}

    return users;
}

// Build the file from a list of available games
// IIIIIIIIIIIIIIIIIII_SSSSSSSSSSSSSSS_PPPPPP
void availableGamesConstructor(std::vector<Game>* games){
    // Open file
    std::ofstream file("assets/available_games.txt");

    // Write to file if open
    if(file.is_open()){

        // Write a line for each game
        for(auto it = games->begin(); it != games->end(); it++){
            
            // Write the game name and pad anything shorter than 19 characters
            file << std::left << std::setw(19) << it->getGameName();
            
            file << " ";

            // Write seller's username and pad anything shorter than 15 characters
            file << std::left << std::setw(15) << it->getSellerName();

            file << " ";

            // Write the game price to 2 decimal places
            file << std::left << std::setw(6) << std::fixed << std::setprecision(2) << it->getGamePrice();

            file << "\n";
        }

        // Close file
        file.close();
    }
}

// Get a list of games from the file
std::vector<Game> availableGames(){
    std::string file_path = "assets/available_games.txt";
    std::vector<Game> games = {};

    try{
        // Open file
        std::ifstream file(file_path);

        // Confirm that filepath exists/file is open
        if(file.is_open()){
            
            // Loop through all lines and add games to the list
            std::string line;
            while(std::getline(file, line)){
                
                // Get game information from formatted line
                std::string game_name = rtrim(line.substr(0,19));
                std::string seller_name = rtrim(line.substr(20,15));
                double price = std::stod(rtrim(line.substr(36,6)));

                // Add game to list
                games.push_back(Game(game_name, seller_name, price));
            }

            // Close file
            file.close();
        }
    }
    catch(...){}

    return games;
}

// Append to the file containing ownership of games from a game and its owner, or create it if it doesn't exist
// IIIIIIIIIIIIIIIIIII_SSSSSSSSSSSSSSS
void appendGameCollection(Game game, User owner){
    // Open file to append to it
    std::ofstream file("assets/game_collection.txt", std::ios::app);

    // Write to file if open
    if(file.is_open()){
        
        // Write the game name and pad anything shorter than 19 characters
        file << std::left << std::setw(19) << game.getGameName();

        file << " ";

        // Write the owner's name and pad anything shorter than 15 characters
        file << std::left << std::setw(15) << owner.getUsername();

        file << "\n";

        // Close file
        file.close();
    }
}

// Remove a game from a user's ownership
// IIIIIIIIIIIIIIIIIII_SSSSSSSSSSSSSSS
void removeGameCollection(Game game, User owner){
    std::string file_path = "assets/game_collection.txt";
    
    // Open original file
    std::ifstream inFile(file_path);
    std::vector<std::string> lines = {};

    // Read all lines from file
    if(inFile.is_open()){
        
        // Loop through all lines and save them to the list
        std::string line;
        while(std::getline(inFile, line)){
            
            // Add line to list 
            lines.push_back(line);
        }

        // Close original file
        inFile.close();
    }

    // Open overwrite file
    std::ofstream outFile(file_path);

    // Write to file if open
    if(outFile.is_open()){
        
        // Find the line to remove
        for(int i = 0; i < lines.size(); i++){
            
            // Construct game name and username
            std::string game_name = rtrim(lines[i].substr(0,19));
            std::string username = rtrim(lines[i].substr(20,15));

            // Write everything except the ownership to remove
            if(game.getGameName() == game_name && owner.getUsername() == username){
                // Do nothing
            }
            else{
                outFile << lines[i] << "\n";
            }
        }

        // Close file
        outFile.close();
    }
}

// Get a list of owned games owned by a specific user
std::vector<Game> gameCollection(const std::vector<Game>& available_games, User owner){
    std::vector<Game> games = {};

    // Open file
    std::ifstream file("assets/game_collection.txt");

    // Read all lines from file
    if(file.is_open()){
        
        // Loop through all lines and save them to the list
        std::string line;
        while(std::getline(file, line)){
            
            // Construct game name and username
            std::string game_name = rtrim(line.substr(0,19));
            std::string username = rtrim(line.substr(20,15));

            // Check if the owner is the user
            if(owner.getUsername() != username){
                continue;
            }

            // Check if the game exists
            for(int i = 0; i < available_games.size(); i++){
                if(available_games[i].getGameName() == game_name){
                    // Add the game to the vector
                    games.push_back(available_games[i]);
                    break;
                }
            }
        }

        // Close file
        file.close();
    }

    return games;
}

// Create the daily transactions file at logout
void dailyTransactionsConstructor(){
    
}

//
void applyDailyTransactions(){

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

// Trim right
std::string rtrim(std::string& s){
    // Line taken from stack overflow https://stackoverflow.com/questions/216823/how-can-i-trim-a-stdstring
    s.erase(s.find_last_not_of(" \n\r\t") + 1);
    
    return s;
}