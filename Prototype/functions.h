#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include "user.h"

void logout(bool& logged_in,
            std::string& current_username,
            std::string& current_user_type);

void listAccounts(const std::vector<User>& users);

#endif