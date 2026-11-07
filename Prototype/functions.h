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

// Case 2 - Create user
bool createUser(
    User current_user,
    std::vector<User>* users,
    std::string username,
    std::string password,
    std::string email,
    std::string type
);

// Case 3 - Delete user
bool deleteUser(
    User current_user,
    std::vector<User>* users,
    std::string username
);

// Case 4 - Sell
void sell();

// Case 5 - Buy
bool buy(
    User& buyer,
    const Game& game
);

// Case 6 - Refund
bool refund(
    User& buyer,
    User& seller,
    double amount
);

// Case 9 - List accounts
void listAccounts(
    const std::vector<User>& users
);

// Current User Accounts file
void currentUserAccountsConstructor();
void currentUserAccounts();

// Available Games file
void availableGamesConstructor();
void availableGames();

// Game Collection file
void gameCollectionConstructor();
void gameCollection(User user);

#endif