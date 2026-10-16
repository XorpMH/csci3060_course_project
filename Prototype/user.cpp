#include "user.h"
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
    if (credit >= amount) {
        credit -= amount;
        return true;
    }
    return false;
}

