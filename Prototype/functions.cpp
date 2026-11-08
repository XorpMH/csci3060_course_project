#include "user.h"
#include "game.h"
#include "functions.h"
#include <iostream>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>

// Functionality variables
namespace {
    // The user currently logged in
    User* cur_user = nullptr;
    
    // Default file paths
    std::string daily_transactions_path = "assets/daily_transactions.txt";
    std::string user_accounts_path = "assets/user_accounts.txt";
    std::string available_games_path = "assets/available_games.txt";
    std::string game_collection_path = "assets/game_collection.txt";
}

// Login function
bool login(const std::string &username,
           std::vector<User> &users,
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
    for (User &user : users)
    {
        if (user.getUsername() == username)
        {
            logged_in = true;
            current_username = user.getUsername();
            current_user_type = user.getUserType();
            
            // Set the user as logged in for the backend
            cur_user = &user;

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
    if(logged_in){
        logged_in = false;
        current_username = "";
        current_user_type = "";

        // Forget the logged in user
        cur_user = nullptr;
    }
}

// listAccounts function
void listAccounts(const std::vector<User>& users) {
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
bool createUser(std::vector<User>& users, // Existing user list, passed by reference
        std::string username, 
        std::string type){
    
    // Confirm logged in
    if (cur_user == nullptr){
        return false;
    }

    // Confirm that the current user is an admin
    if(!(cur_user->getUserType()=="AA")){
        return false;
    }
    
    // Check that the user being created has a unique username
    for(auto it = users.begin(); it != users.end(); it++){
        if(it->getUsername()==username){
            return false;
        }
    }

    // Check user type format
    if(!typeValid(type)){
        return false;
    }

    // Create the user
    users.push_back(User(username, type));
    // TODO: replace push_back with a constructing a transaction and adding it to the transactions list

    return true;
}

// Remove an existing user
bool deleteUser(std::vector<User>& users,  // Existing user list, passed by reference
        std::string username // The username of the user to delete
    ){

    // Confirm logged in
    if (cur_user == nullptr){
        return false;
    }

    // Confirm that the current user is an admin
    if(!(cur_user->getUserType()=="AA")){
        return false;
    }
    
    // Find and remove the user from the list
    for(auto it = users.begin(); it != users.end(); it++){
        if(it->getUsername()==username){
            users.erase(it);
            return true;
        }
    }
    // TODO: replace erase with a constructing a transaction and adding it to the transactions list

    return false;
}

// Put a game up for sale
bool sell(const std::string& gamename,
          const std::string& seller_name,
          double gameprice,
          std::vector<Game>& available_games)
{

    // Check if the user is logged in
    if (cur_user == nullptr) {
        std::cout << "Error: User must be logged in to sell a game.\n";
        return false;
    }

    // Standard-buy accounts cannot sell games
    if (cur_user->getUserType() == "BS") {
        std::cout << "Error: Standard-buy accounts cannot sell games.\n";
        return false;
    }

    // Check if the game name is valid
    if (gamename.empty() || gamename.length() > 25) {
        std::cout << "Error: Game name must be between 1 and 25 characters.\n";
        return false;
    }

    // Check if the price is valid
    if (!std::isfinite(gameprice) ||
        gameprice < 0 || gameprice > 999.99) {
        std::cout << "Error: Game price must be between $0.00 and $999.99.\n";
        return false;
    }

    // Check if the game name already exists
    for (const Game& game : available_games) {
        if (game.getGameName() == gamename) {
            std::cout << "Error: Game name already exists.\n";
            return false;
        }
    }

    // Create a new game and add it to the available games vector
    Game new_game(gamename, seller_name, gameprice);
    available_games.push_back(new_game);
    return true;
}


// Purchase a game that is up for sale
bool buy(User& buyer, const Game& game)
{
    // Check if the buyer has enough credit
    if (buyer.getCredit() < game.getGamePrice())
    {
        return false;
    }

    // Deduct the game price from the buyer
    return buyer.deductCredit(game.getGamePrice());
}


// Search for a game by name
Game* searchGame(
    std::vector<Game>& games,
    const std::string& game_name
)
{
    for (Game& game : games)
    {
        if (game.getGameName() == game_name)
        {
            return &game;
        }
    }

    return nullptr;
}


// Refund a user's purchase
bool refund(
    User& buyer,
    User& seller,
    double amount
)
{
    // Refund amount must be valid
    if (amount <= 0)
    {
        return false;
    }

    // Check if the seller has enough credit
    if (!seller.deductCredit(amount))
    {
        return false;
    }

    // Return the credit to the buyer
    buyer.addCredit(amount);

    return true;
}

//
// FILE FUNCTIONS
//

// Build the file from a list of user accounts
// UUUUUUUUUUUUUUU_TT_CCCCCCCCC
void currentUserAccountsConstructor(std::vector<User>& users){
    // Open file
    std::ofstream file(user_accounts_path);

    // Write to file if open
    if(file.is_open()){

        // Write a line for each user
        for(auto it = users.begin(); it != users.end(); it++){
            
            // Write the username, and pad anything shorter than 15 characters
            file << std::left << std::setw(15) << it->getUsername();
            
            file << " ";

            // Write user type as a 2 letter capitalized string
            file << it->getUserType();

            file << " ";

            // Write the user's credit, append .00 if it is an integer and write only 2 decimals
            // TODO
            if(it->getCredit() == static_cast<int>(it->getCredit())){
                file << it->getCredit() << ".00";
            }
            else{
                file << std::setprecision(2) << it->getCredit();
            }

            file << "\n";
        }

        // Close file
        file.close();
    }
}

// Get a list of user accounts from the file
std::vector<User> currentUserAccounts(){
    std::vector<User> users = {};

    try{
        // Open file
        std::ifstream file(user_accounts_path);

        // Confirm that filepath exists/file is open
        if(file.is_open()){
            
            // Loop through all lines and add users to the list
            std::string line;
            while(std::getline(file, line)){
                
                // Get user information from formatted line
                std::string username = line.substr(0,15);
                std::string user_type = line.substr(16,2);
                double credits = std::stod(line.substr(19,9));

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
void availableGamesConstructor(std::vector<Game> games){
    // Open file
    std::ofstream file(available_games_path);

    // Write to file if open
    if(file.is_open()){

        // Write a line for each game
        for(auto it = games.begin(); it != games.end(); it++){
            
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
    std::vector<Game> games = {};

    try{
        // Open file
        std::ifstream file(available_games_path);

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
    std::ofstream file(game_collection_path, std::ios::app);

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
    // Open original file
    std::ifstream inFile(game_collection_path);
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
    std::ofstream outFile(game_collection_path);

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
std::vector<Game> gameCollection(const std::vector<Game>& available_games, User owner, std::string file_path){
    std::vector<Game> games = {};

    // Open file
    std::ifstream file(file_path);

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
void dailyTransactionsConstructor(std::vector<std::string> transactions, std::string file_path){
    // Write all transactions line-by-line into the file
    writeFile(file_path, transactions);
}

// Apply individual transactions from the daily transactions file
void applyDailyTransactions(std::string file_path){
    // Get all transactions as strings
    std::vector<std::string> transactions = readFile(file_path);

    // Apply each transaction to the corresponding file
    for(std::string transaction: transactions){
        applyTransaction(transaction);
    }
}

// Apply a single transaction
// XX UUUUUUUUUUUUUUU TT CCCCCCCCC (01, 02, 06, 00)
// XX UUUUUUUUUUUUUUU SSSSSSSSSSSSSSS CCCCCCCCC (05)
// XX IIIIIIIIIIIIIIIIIII SSSSSSSSSSSSSSS PPPPPP (03)
// XX IIIIIIIIIIIIIIIIIII SSSSSSSSSSSSSSS UUUUUUUUUUUUUUU PPPPPP (04)
std::string applyTransaction(std::string transaction){
    // Grab the transaction type
    std::string code = transaction.substr(0,2);
    
    switch(stoi(code)){
        // Log out / end of session
        case 0: {
            // Confirm correct length
            if(transaction.length() != 31){
                return "FAILURE: Improper format";
            }

            // TODO: add any input checking here

            // Parse input values
            std::string username = rtrim(transaction.substr(3,15));
            std::string user_type = rtrim(transaction.substr(19,2));
            double credit = stod(transaction.substr(22,9));

            // Attempt to log out
            bool logged_in = cur_user != nullptr;
            bool temp = logged_in;
            logout(logged_in, username, user_type);
            
            if (logged_in != temp){
                return "FAILURE: Already logged in";
            }
            return "SUCCESS: Transaction accepted";
        }

        // Create account
        case 1: {
            // Confirm correct length
            if(transaction.length() != 31){
                return "FAILURE: Improper format";
            }

            // TODO: add any input checking here

            // Parse input values
            std::string username = rtrim(transaction.substr(3,15));
            std::string user_type = rtrim(transaction.substr(19,2));
            double credit = stod(transaction.substr(22,9));

            // Delete a user from the user_accounts file
            std::vector<User> users = currentUserAccounts();
            if(createUser(users, username, user_type)){
                // Rebuild current_user_accounts file
                currentUserAccountsConstructor(users);
                return "SUCCESS: Transaction accepted";
            }
            else{
                return "FAILURE: Transaction denied";
            }
        }

        // Delete account
        case 2: {
            // Confirm correct length
            if(transaction.length() != 31){
                return "FAILURE: Improper format";
            }

            // TODO: add any input checking here

            // Parse input values
            std::string username = rtrim(transaction.substr(3,15));
            std::string user_type = rtrim(transaction.substr(19,2));
            double credit = stod(transaction.substr(22,9));

            // Delete a user from the user_accounts file
            std::vector<User> users = currentUserAccounts();
            if(deleteUser(users, username)){
                // Rebuild current_user_accounts file
                currentUserAccountsConstructor(users);
                return "SUCCESS: Transaction accepted";
            }
            else{
                return "FAILURE: Transaction denied";
            }
        }

        // Put a game up for sale
        case 3: {
            // Confirm correct length
            if(transaction.length() != 45){
                return "FAILURE: Improper format";
            }

            // TODO: add any input checking here

            // Parse input values
            std::string game_name = rtrim(transaction.substr(3,19));
            std::string seller_name = rtrim(transaction.substr(23,15));
            double price = stod(transaction.substr(39,6));

            // Add a game to the available_games file
            std::vector<Game> games = availableGames();
            if(sell(game_name, seller_name, price, games)){
                // Rebuild available_games file
                availableGamesConstructor(games);
                return "SUCCESS: Transaction accepted";
            }
            return "FAILURE: Transaction denied";
        }

        // Buy a game
        case 4: {
            // Confirm correct length
            if(transaction.length() != 61){
                return "FAILURE: Improper format";
            }

            // TODO: add any input checking here

            // Parse input values
            std::string game_name = rtrim(transaction.substr(3,19));
            std::string seller_name = rtrim(transaction.substr(23,15));
            std::string buyer_name = rtrim(transaction.substr(39,15));
            double price = stod(transaction.substr(55,6));

            // Get users and games from files
            std::vector<User> users = currentUserAccounts();
            std::vector<Game> games = availableGames();

            // Pointers to find the desired user and game
            User* user_to_buy = nullptr;
            Game* game_to_buy = nullptr;

            // Find the user if it exists
            for(User& user : users){
                if(user.getUsername()==buyer_name){
                    user_to_buy = &user;
                    continue;
                }
            }
            // Find the game if it exists
            for(Game game : games){
                if(game.getGameName()==game_name && game.getSellerName()==seller_name){
                    game_to_buy = &game;
                    continue;
                }
            }

            // Only run buy if both the user and game exist
            if(user_to_buy == nullptr){
                return "FAILURE: No matching user";
            }
            else if(game_to_buy == nullptr){
                return "FAILURE: No matching game";
            }
            else{
                // Run the buy function
                if(buy(*user_to_buy, *game_to_buy)){
                    // Rebuild current_user_accounts file
                    currentUserAccountsConstructor(users);
                    return "SUCCESS: Transaction accepted";
                }
                return "FAILURE: Transaction denied";
            }
        }

        // Refund a game
        case 5: {
            // Confirm correct length
            if(transaction.length() != 44){
                return "FAILURE: Improper format";
            }

            // TODO: add any input checking here

            // Parse input values
            std::string buyer_name = rtrim(transaction.substr(3,15));
            std::string seller_name = rtrim(transaction.substr(19,15));
            double refund_credit = stod(transaction.substr(35,9));

            // Get users and games from files
            std::vector<User> users = currentUserAccounts();
            std::vector<Game> games = availableGames();

            // Pointers to find the desired users
            User* buyer_to_refund = nullptr;
            User* seller_to_charge = nullptr;

            // Find the users if they exist
            for(User& user : users){
                if(user.getUsername()==buyer_name){
                    buyer_to_refund = &user;
                    continue;
                }
                else if(user.getUsername()==seller_name){
                    seller_to_charge = &user;
                    continue;
                }
            }

            // Only run buy if both the user and game exist
            if(buyer_to_refund == nullptr){
                return "FAILURE: No matching buyer";
            }
            if(seller_to_charge == nullptr){
                return "FAILURE: No matching seller";
            }
            
            // Run the buy function
            if(refund(*buyer_to_refund, *seller_to_charge, refund_credit)){
                return "SUCCESS: Transaction accepted";
            }
            return "FAILURE: Transaction denied";
        }

        // Add credit to a user
        case 6: {
            // Confirm correct length
            if(transaction.length() != 31){
                return "FAILURE: Improper format";
            }

            // TODO: add any input checking here

            // Parse input values
            std::string username = rtrim(transaction.substr(3,15));
            std::string user_type = rtrim(transaction.substr(19,2));
            double credit = stod(transaction.substr(22,9));

            // Get users from files
            std::vector<User> users = currentUserAccounts();

            // Pointer to find the desired user
            User* user_to_pay = nullptr;

            // Find the users if they exist
            for(User& user : users){
                if(user.getUsername()==username){
                    user_to_pay = &user;
                    continue;
                }
            }

            // Confirm that the user exists
            if(user_to_pay == nullptr){
                return "FAILURE: No matching user";
            }
            
            // Add credit to the user
            user_to_pay->addCredit(credit);
            return "SUCCESS: Transaction accepted";
        }
    }

    return "FAILURE: No corresponding transaction";
}

//
// SETTERS AND GETTERS FOR FILE PATHS
//

void setDailyTransactionsPath(std::string path) {daily_transactions_path = path;}
void setUserAccountsPath(std::string path) {user_accounts_path = path;}
void setAvailableGamesPath(std::string path) {available_games_path = path;}
void setGameCollectionPath(std::string path) {game_collection_path = path;}

std::string getDailyTransactionsPath(){return daily_transactions_path;}
std::string getUserAccountsPath(){return user_accounts_path;}
std::string getAvailableGamesPath(){return available_games_path;}
std::string getGameCollectionPath(){return game_collection_path;}


//
// HELPER FUNCTIONS
//

// Append a line to a file helper
void appendToFile(std::string file_path, std::string line){
    // Open file to append to it
    std::ofstream file(file_path, std::ios::app);

    // Write to file if open
    if(file.is_open()){
        
        // Add line to end of file
        file << line << "\n";

        // Close file
        file.close();
    }
}

// Remove a line from a file helper
void removeFromFile(const std::string& file_path, const std::string& line_to_remove){
    // Open original file
    std::vector<std::string> lines = readFile(file_path);

    // Open overwrite file
    std::ofstream outFile(file_path);

    // Write to file if open
    if(outFile.is_open()){
        
        // Find the line to remove
        for(int i = 0; i < lines.size(); i++){
            
            // Write everything except the line to remove
            if(lines[i] == line_to_remove){
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

// Read all lines from a file helper
std::vector<std::string> readFile(const std::string& file_path){
    std::vector<std::string> lines = {};

    // Open file
    std::ifstream inFile(file_path);

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

    return lines;
}

// Write all lines to a file helper
void writeFile(const std::string& file_path, const std::vector<std::string>& lines){
    // Open file
    std::ofstream file(file_path);

    // Write to file if open
    if(file.is_open()){

        // Write each line
        for(std::string line : lines){
            
            // Write line to the file
            file << line << "\n";
        }

        // Close file
        file.close();
    }
}

// Requirements for a valid user type
bool typeValid(std::string type){
    return (type == "AA" || type == "FS" || type == "BS" || type == "SS");
}

// Trim right
std::string rtrim(std::string s){
    // Line taken from stack overflow https://stackoverflow.com/questions/216823/how-can-i-trim-a-stdstring
    s.erase(s.find_last_not_of(" \n\r\t") + 1);
    
    return s;
}