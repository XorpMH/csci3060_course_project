#include <string>;
#include <vector>;
#include <unordered_map>;
#include <iostream>;
#include <fstream>;
#include "game.h";
#include "user.h";

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

void writeFile(std::string file_path){
    //TODO: Write to the file output location in the format of the test cases
    
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
