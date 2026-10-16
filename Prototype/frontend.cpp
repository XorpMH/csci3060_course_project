#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include "game.h"
#include "user.h"

// Boolean controlling the main loop when browsing
bool running = true;

// Whether the user is currently logged in or not
// TODO: replace with a check using the user class?
bool logged_in = false;

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
    //TODO: Show text and receive user input
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
        std::cout << "List accounts selected..." << std::endl;
                break;
                
        default:
        std::cout << "Invalid input, please select an option from 1 to 9 and try again..." << std::endl;
            break;
    }
}

// TODO: Implement logout function in a separate file.
void logout(){}
