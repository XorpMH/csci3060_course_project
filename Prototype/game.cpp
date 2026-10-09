#include "game.h"

// Constructor for the Game class
Game::Game(std::string game_name, std::string game_description, double game_price) {
  this->game_name = game_name;
  this->game_description = game_description;
  this->game_price = game_price;
}

// Getter for the game name
std::string Game::getGameName() const {
  return game_name;
}

// Getter for the game description
std::string Game::getGameDescription() const {
  return game_description;
} 

// Getter for the game price
double Game::getGamePrice() const {   
  return game_price;
} 

// Setter for the game name
std::string Game::setGameName(std::string name) {
  game_name = name;
  return game_name;
}     

// Setter for the game description
std::string Game::setGameDescription(std::string description) {   
  game_description = description;
  return game_description;
} 

// Setter for the game price
double Game::setGamePrice(double price) {
  game_price = price;
  return game_price;
} 