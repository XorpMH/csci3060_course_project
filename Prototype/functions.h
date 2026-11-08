#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <vector>

#include "user.h"
#include "game.h"

/**
 * @file functions.h
 * @brief Declares utility functions for user authentication,
 * account management, game transactions, file handling,
 * and daily transaction processing.
 */

// User authentication
bool login(
    const std::string& username,
    const std::vector<User>& users,
    bool& logged_in,
    std::string& current_username,
    std::string& current_user_type
);

void logout(
    bool& logged_in,
    std::string& current_username,
    std::string& current_user_type
);

// Account management
void listAccounts(const std::vector<User>& users);

bool createUser(
    std::vector<User>& users,
    const std::string& username,
    const std::string& type
);

bool deleteUser(
    std::vector<User>& users,
    const std::string& username
);

// Game transactions
bool sell(
    const std::string& gamename,
    double gameprice,
    const std::vector<Game>& available_games,
    std::vector<Game>& pending_games
);

// Search for a game
Game* searchGame(
    std::vector<Game>& games,
    const std::string& game_name
);

bool buy(
    User& buyer,
    const Game& game
);

bool refund(
    User& buyer,
    User& seller,
    double amount
);

// Current User Accounts file
void currentUserAccountsConstructor(
    const std::vector<User>& users
);

std::vector<User> currentUserAccounts();

// Available Games file
void availableGamesConstructor(
    const std::vector<Game>& games
);

std::vector<Game> availableGames();

// Game Collection file
void appendGameCollection(
    const Game& game,
    const User& owner
);

void removeGameCollection(
    const Game& game,
    const User& owner
);

std::vector<Game> gameCollection(
    const std::vector<Game>& available_games,
    const User& owner
);

// Daily Transaction file
void dailyTransactionsConstructor(
    const std::vector<std::string>& transactions,
    const std::string& file_path
);

void applyDailyTransactions(
    const std::string& file_path
);

std::string applyTransaction(
    const std::string& transaction
);

// File path setters/getters
void setDailyTransactionsPath(const std::string& path);
void setUserAccountsPath(const std::string& path);
void setAvailableGamesPath(const std::string& path);
void setGameCollectionPath(const std::string& path);

std::string getDailyTransactionsPath();
std::string getUserAccountsPath();
std::string getAvailableGamesPath();
std::string getGameCollectionPath();

// File helpers
void appendToFile(
    const std::string& file_path,
    const std::string& line
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
bool passwordValid(const std::string& password);
bool emailValid(const std::string& email);
bool typeValid(const std::string& type);

// String helper
std::string rtrim(std::string s);

#endif
