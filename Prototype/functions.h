#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include "user.h"

bool login(const std::string& username,
           const std::vector<User>& users,
           bool& logged_in,
           std::string& current_username,
           std::string& current_user_type);


void logout(bool& logged_in,
            std::string& current_username,
            std::string& current_user_type);

void listAccounts(const std::vector<User>& users);


#endif