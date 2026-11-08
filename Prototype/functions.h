#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <vector>

#include "user.h"
#include "game.h"


// Login
bool login(
    const std::string& username,
    const std::vector<User>& users,
    bool& logged_in,
    std::string& current_username,
    std::string& current_user_type
);


// Logout
void logout(
    bool& logged_in,
    std::string& current_username,
    std::string& current_user_type
);


// List accounts
void listAccounts(
    const std::vector<User>& users
);


// Create user - new version
bool createUser(
    User current_user,
    std::vector<User>& users,
    std::string username,
    std::string password,
    std::string email,
    std::string type
);


// Create user - old compatible version
bool createUser(
    User current_user,
    std::vector<User>* users,
    std::string username,
    std::string password,
    std::string email,
    std::string type
);


// Delete user - new version
bool deleteUser(
    User current_user,
    std::vector<User>& users,
    std::string username
);


// Delete user - old compatible version
bool deleteUser(
    User current_user,
    std::vector<User>* users,
    std::string username
);


// Sell - new version
void sell(
    bool logged_in,
    const std::string& current_user_type,
    const std::string& gamename,
    const std::string& gamedescription,
    double gameprice,
    const std::vector<Game>& available_games,
    std::vector<Game>& pending_games
);


// Sell - old compatible version
void sell();


// Search for a game
Game* searchGame(
    std::vector<Game>& games,
    const std::string& game_name
);


// Buy a game
bool buy(
    User& buyer,
    const Game& game
);


// Refund a purchase
bool refund(
    User& buyer,
    User& seller,
    double amount
);


// Current User Accounts - new
void currentUserAccountsConstructor(
    std::vector<User>* users,
    std::string file_path
);

std::vector<User> currentUserAccounts(
    std::string file_path
);


// Current User Accounts - old compatible versions
void currentUserAccountsConstructor();

void currentUserAccounts();


// Available Games - new
void availableGamesConstructor(
    std::vector<Game>* games,
    std::string file_path
);

std::vector<Game> availableGames(
    std::string file_path
);


// Available Games - old compatible versions
void availableGamesConstructor();

void availableGames();


// Game Collection - new
void appendGameCollection(
    Game game,
    User owner,
    std::string file_path
);

void removeGameCollection(
    Game game,
    User owner,
    std::string file_path
);

std::vector<Game> gameCollection(
    const std::vector<Game>& available_games,
    User owner,
    std::string file_path
);


// Game Collection - old compatible versions
void gameCollectionConstructor();

void gameCollection(
    User user
);


// Daily Transaction File
void dailyTransactionsConstructor(
    std::vector<std::string> transactions,
    std::string file_path
);

void applyDailyTransactions(
    std::string file_path
);

std::string applyTransaction(
    std::string file_path,
    std::string transaction
);


// File helpers
void appendToFile(
    std::string file_path,
    std::string line
);

void removeFromFile(
    const std::string& file_path,
    const std::string& line_to_remove
);

std::vector<std::string> readFile(
    const std::string& file_path
);

void writeFile(
    const std::string& file_path,
    const std::vector<std::string>& lines
);


// Validation helpers
bool passwordValid(
    std::string password
);

bool emailValid(
    std::string email
);

bool typeValid(
    std::string type
);


// String helper
std::string rtrim(
    std::string s
);

#endif