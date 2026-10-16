#ifndef GAME_H
#define GAME_H
#include <string>
class Game {
private:
    std::string game_name;
    std::string game_description;
    double game_price;
public:
    Game(std::string gamename, std::string gamedescription, double gameprice);
    std::string getGameName() const;
    std::string getGameDescription() const;
    double getGamePrice() const;
    std::string setGameName(std::string name);
    std::string setGameDescription(std::string description);
    double setGamePrice(double price);
};

#endif