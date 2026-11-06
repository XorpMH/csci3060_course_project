#ifndef GAME_H
#define GAME_H
#include <string>

/**
 * @class Game
 * @brief Represents a game in the store catalog, tracking its metadata and pricing.
 */
class Game {
private:
    std::string game_name;
    std::string seller_name;
    std::string game_description;
    double game_price;
    
public:
    /// @name Constructor
    /// @{
    Game(std::string game_name, std::string seller_name, std::string game_description, double game_price);
    /// @}

    /// @name Getters
    /// @{
    std::string getGameName() const;
    std::string getSellerName() const;
    std::string getGameDescription() const;
    double getGamePrice() const;
    /// @}

    /// @name Setters
    /// @{
    std::string setGameName(std::string name);
    std::string setSellerName(std::string name);
    std::string setGameDescription(std::string description);
    double setGamePrice(double price);
    /// @}
};

#endif