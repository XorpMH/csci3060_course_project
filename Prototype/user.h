#ifndef USER_H
#define USER_H
#include <string>
class User {
private:
    std::string username;
    std::string user_type;
    std::string password;
    std::string email;
    double credit;
public:
    User(std::string name, std::string type, std::string email, std::string password);
    User(std::string name, std::string type, double amount);
    User(std::string name, std::string type);
    std::string getUsername() const;
    std::string getUserType() const;
    std::string getEmail() const;
    double getCredit() const;
    void addCredit(double amount);
    bool deductCredit(double amount);
};

#endif