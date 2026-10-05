#ifndef USER_H
#define USER_H
#include <string>
class User {
private:
    std::string username;
    std::string userType;
    double credit;
public:
    User(std::string name, std::string type, double amount);
    std::string getUsername() const;
    std::string getUserType() const;
    double getCredit() const;
    void addCredit(double amount);
    bool deductCredit(double amount);
};

#endif
