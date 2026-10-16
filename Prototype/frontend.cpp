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
    
    //TODO: Comment out switch statement and set input variables inside, then distribute load into separate functions
    /*
    switch(input_variable){
        case EXAMPLE_CONSTANT:
            break;
    }
    */
}
