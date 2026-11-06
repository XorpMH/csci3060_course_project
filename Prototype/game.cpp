#include "game.h"

/**
 * @file game.cpp
 * @brief Implements the Game class for managing store catalog entries.
 */

// --- CONSTRUCTOR ---

Game::Game(std::string game_name, std::string game_description, double game_price) {
  this->game_name = game_name;
  this->game_description = game_description;
  this->game_price = game_price;
}

// --- GETTERS ---

std::string Game::getGameName() const {
  return game_name;
}

std::string Game::getGameDescription() const {
  return game_description;
} 

double Game::getGamePrice() const {   
  return game_price;
} 

// --- SETTERS ---

std::string Game::setGameName(std::string name) {
  game_name = name;
  return game_name;
}     

std::string Game::setGameDescription(std::string description) {   
  game_description = description;
  return game_description;
} 

double Game::setGamePrice(double price) {
  game_price = price;
  return game_price;
}