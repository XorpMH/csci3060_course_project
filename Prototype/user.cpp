#include "user.h"

/**
 * @file user.cpp
 * @brief Implements the User class tracking account credentials, roles, and balances.
 */

 // --- CONSTRUCTORS ---

// Full constructor for a new user account registration
User::User(std::string name, std::string type, std::string email, std::string password) {
    this->username = name;
    this->user_type = type;
    this->password = password;
    this->email = email;
    this->credit = 0.0;
}

// Constructor for loading an existing user with a pre-existing balance
User::User(std::string name, std::string type, double amount) {
    username = name;
    user_type = type;
    credit = amount;
}

// Minimal constructor for quick/guest user creation
User::User(std::string name, std::string type) {
    username = name;
    user_type = type;
    credit = 0.0;
}

// --- GETTERS ---

std::string User::getUsername() const {
    return username;
}

std::string User::getUserType() const {
    return user_type;
}

double User::getCredit() const {
    return credit;
}

// --- CREDIT MANAGEMENT ---

void User::addCredit(double amount) {
    credit += amount;
}

// Prevents overdrafts and negative deductions
bool User::deductCredit(double amount) {
    if (credit >= amount && amount >= 0) {
        credit -= amount;
        return true;
    }
    return false;
}

