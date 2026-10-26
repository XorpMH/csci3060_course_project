#include "user.h"

User::User(std::string name, std::string type, std::string email, std::string password) {
    this->username = name;
    this->user_type = type;
    this->password = password;
    this->email = email;
    this->credit = 0.0;
}
User::User(std::string name, std::string type, double amount) {
    username = name;
    user_type = type;
    credit = amount;
}
User::User(std::string name, std::string type) {
    username = name;
    user_type = type;
    credit = 0.0;
}
std::string User::getUsername() const {
    return username;
}
std::string User::getUserType() const {
    return user_type;
}
double User::getCredit() const {
    return credit;
}
void User::addCredit(double amount) {
    credit += amount;
}
bool User::deductCredit(double amount) {
    if (credit >= amount && amount >= 0) {
        credit -= amount;
        return true;
    }
    return false;
}

