#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <vector>

#include "user.h"
#include "game.h"

/**
 * @file functions.h
 * @brief Declares utility functions for user authentication,
 * account management, and game transactions.
 */

/// @name User Authentication
/// @{

/**
 * @brief Logs a user into the system.
 * @param username Username to authenticate.
 * @param users List of registered users.
 * @param logged_in Current login status.
 * @param current_username Username of the logged-in user.
 * @param current_user_type Account type of the logged-in user.
 * @return true if login succeeds, false otherwise.
 */
bool login(const std::string& username,
           const std::vector<User>& users,
           bool& logged_in,
           std::string& current_username,
           std::string& current_user_type);

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
/**
 * @brief Logs out the current user and clears session information.
 * @param logged_in Current login status.
 * @param current_username Username of the logged-in user.
 * @param current_user_type Account type of the logged-in user.
 */
void logout(bool& logged_in,
            std::string& current_username,
            std::string& current_user_type);
/// @}

/// @name Account Management
/// @{

/**
 * @brief Displays a list of registered user accounts.
 * @param users List of registered users.
 */
void listAccounts(const std::vector<User>& users);
/// @}

/// @name Game Transactions
/// @{

/**
 * @brief Validates a game listing for sale.
 * @param logged_in Current login status.
 * @param current_user_type Account type of the current user.
 * @param gamename Name of the game being sold.
 * @param gamedescription Description of the game.
 * @param gameprice Selling price of the game.
 * @param available_games List of currently available games.
 * @param pending_games List of games pending availability.
 */
void sell(bool logged_in,
          const std::string& current_user_type,
          const std::string& gamename,
          const std::string& gamedescription,
          double gameprice,
          const std::vector<Game>& available_games,
          std::vector<Game>& pending_games);
/// @}


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
    std::vector<User>* users
);

std::vector<User> currentUserAccounts();


// Available Games - new
void availableGamesConstructor(
    std::vector<Game>* games
);

std::vector<Game> availableGames();


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

#endif