#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <tuple>
#include "game.h"
#include "user.h"
#include "functions.h"

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

void loginMenu(){
    std::string command;

    std::cout << "\nLogged in as: " << current_username << "\n";
    std::cout << "Enter command: ";
    std::getline(std::cin, command);

    if(command == "logout"){
        logout(logged_in, current_username, current_user_type);
    }
    else{
        std::cout << "Invalid command. Please try again.\n";
    }
}

void browseMenu(){
    //TODO: Show text and receive user input
    std::cout << "===MENU===" << std::endl;
    std::cout << "1. Logout" << std::endl;
    std::cout << "2. Create" << std::endl;
    std::cout << "3. Delete" << std::endl;
    std::cout << "4. Sell" << std::endl;
    std::cout << "5. Buy" << std::endl;
    std::cout << "6. Refund" << std::endl;
    std::cout << "7. Add credit" << std::endl;
    std::cout << "8. List transactions" << std::endl;
    std::cout << "9. List accounts" << std::endl;

    int input_variable;
    std::cout << "Please select an option: ";
    std::cin >> input_variable;

    //TODO: Comment out switch statement and set input variables inside, then distribute load into separate functions
    switch(input_variable){
        case 1:
        std::cout << "Logout selected..." << std::endl;
            // logout(); // TODO: Connect to logout function
            break;

        case 2:
        std::cout << "Create account selected..." << std::endl;
            break;

        case 3:
        std::cout << "Delete account selected..." << std::endl;
            break;      

        case 4:
        std::cout << "Sell game selected..." << std::endl;
            break;  

        case 5:
        std::cout << "Buy game selected..." << std::endl;   
            break;  

        case 6:
        std::cout << "Refund purchase(s) selected..." << std::endl;
            break;      

        case 7: 
         std::cout << "Add credit to account selected..." << std::endl;
                break;

        case 8: 
        std::cout << "List transactions selected..." << std::endl;
                break;

        case 9:
        listAccounts(users);
            break;

        default:
        std::cout << "Invalid input, please select an option from 1 to 9 and try again..." << std::endl;
            break;
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
