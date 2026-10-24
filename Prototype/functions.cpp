#include "user.h"
#include "game.h"
#include "functions.h"
#include <iostream>
#include <vector>

// Logout function
void logout(bool& logged_in,
            std::string& current_username,
            std::string& current_user_type) {
    logged_in = false;
    current_username = "";
    current_user_type = "";

    std::cout << "Logout successful\n";
}

// listAccounts function
void listAccounts(const std::vector<User>& users) {
  for (const User& user : users) {
    std::cout << "Username: " << user.getUsername() 
    << ", Type: " << user.getUserType() 
    << ", Credit: $" << user.getCredit() 
    << std::endl;
  }
}