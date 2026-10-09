#include "game.h"

// Constructor for the Game class
Game::Game(std::string gamename, std::string gamedescription, double gameprice) {
  this->gamename = gamename;
  this->gamedescription = gamedescription;
  this->gameprice = gameprice;
}

// Getter for the game name
std::string Game::getGameName() const {
  return gamename;
}

// Getter for the game description
std::string Game::getGameDescription() const {
  return gamedescription;
} 

// Getter for the game price
double Game::getGamePrice() const {   
  return gameprice;
} 

// Setter for the game name
std::string Game::setGameName(std::string name) {
  gamename = name;
  return gamename;
}     

// Setter for the game description
std::string Game::setGameDescription(std::string description) {   
  gamedescription = description;
  return gamedescription;
} 

// Setter for the game price
double Game::setGamePrice(double price) {
  gameprice = price;
  return gameprice;
} 