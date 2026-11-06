#ifndef USER_H
#define USER_H
#include <string>

/**
 * @class User
 * @brief Represents a system user, managing authentication data and financial credit.
 */
class User {
private:
    std::string username;
    std::string user_type;
    std::string password;
    std::string email;
    double credit;

public:
    /// @name Constructors
    /// @{
    User(std::string name, std::string type, std::string email, std::string password);
    User(std::string name, std::string type, double amount = 0.0);
    User(std::string name, std::string type);
    /// @}

    /// @name Getters
    /// @{  
    std::string getUsername() const;
    std::string getUserType() const;
    std::string getEmail() const;
    double getCredit() const;
    /// @}

    /// @name Credit Wallet Management
    /// @{
    
    /** @brief Increases the user's available credit balance. */
    void addCredit(double amount);

    /** 
     * @brief Safely deducts funds from the user's balance.
     * @return true if successful, false if insufficient funds or invalid amount.
     * @param amount The value to deduct (must be positive).
     */
    bool deductCredit(double amount);
    /// @}

};

#endif