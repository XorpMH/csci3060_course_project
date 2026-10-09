#include <string>;
#include <vector>;
#include <unordered_map>;
#include <iostream>;
#include <fstream>;
#include "game.h";
#include "user.h";

int main(){
    
}

// Using filepath, returns a tuple containing the variables as key-value pairings, and the desired command as a string
std::tuple<std::unordered_map<std::string, std::string>, std::string> readFile(std::string file_path){
    
    //return variables
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
            
        }

        // Close file
        file.close();
    }

    return {value_map, command}
}

void writeFile(){

}