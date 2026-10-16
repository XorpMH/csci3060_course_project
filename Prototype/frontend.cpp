#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <tuple>
#include "game.h"
#include "user.h"

// Boolean controlling the main loop when browsing
bool running = true;

// Whether the user is currently logged in or not
// TODO: replace with a check using the user class?
bool logged_in = false;

std::vector<User> users = {
    User("adminuser", "AA", 500.00),
    User("existinguser", "FS", 100.00)
};

std::string current_username;
std::string current_user_type;

void loginMenu();
void browseMenu();
bool login(const std::string& username);

// Main browsing loop
int main(){
    while(running){
        if(logged_in){
            loginMenu();
        }
        else{
            browseMenu();
        }
    }
}

// Input harness
// Using filepath, returns a tuple containing the variables as key-value pairings, and the desired command as a string
std::tuple<std::unordered_map<std::string, std::string>, std::string> readFile(std::string file_path){
    
    // Return variables, empty if file isn't accessed
    std::unordered_map<std::string, std::string> value_map;
    std::string command;

    // Open file
    std::ifstream file(file_path);

    // Confirm that filepath exists/file is open
    if(file.is_open()){
        // Loop through all lines but the last, assigning key-value pairs based on the input file
        // Input values are formatted as:
        // Variable: value
        std::string line;
        while(std::getline(file, line)){
            if(line.find(':') != std::string::npos){
                // Key is everything before ":",  value is everything after the first space
                value_map[line.erase(line.find(':'))] = line.erase(0, line.find(' ') + 1);
            }
            else{
                // Any line not containing ":" is interpreted as a command
                command = line;
            }
        }

        // Close file
        file.close();
    }

    return {value_map, command};
}

void writeFile(){
    //TODO: Write to the file output location in the format of the test cases
}

void loginMenu(){
    std::string command;

    std::cout << "\nLogged in as: " << current_username << "\n";
    std::cout << "Enter command: ";
    std::getline(std::cin, command);

    if(command == "logout"){
        logged_in = false;
        current_username = "";
        current_user_type = "";

        std::cout << "Logout successful\n";
    }
    else{
        std::cout << "Invalid command\n";
    }
}

void browseMenu(){
    std::string command;

    std::cout << "Enter command: ";
    std::getline(std::cin, command);

    if(command == "login"){
        std::string username;

        std::cout << "Enter username: ";
        std::getline(std::cin, username);

        login(username);
    }
    else{
        std::cout << "Invalid command\n";
    }
}

bool login(const std::string& username){
    if(logged_in){
        std::cout << "Login failed: user is already logged in\n";
        return false;
    }

    for(const User& user : users){
        if(user.getUsername() == username){
            logged_in = true;
            current_username = user.getUsername();
            current_user_type = user.getUserType();

            std::cout << "Login successful\n";
            return true;
        }
    }

    std::cout << "Login failed: user does not exist\n";
    return false;
}

