#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
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

#endif