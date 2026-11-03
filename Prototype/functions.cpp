#include "user.h"
#include "game.h"
#include "functions.h"
#include <iostream>
#include <vector>

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