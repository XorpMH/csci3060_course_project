#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include "frontend.cpp"

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

// Write the status of the output
void writeFile(std::string file_path, std::string status){
    
    // Open file
    std::ofstream file(file_path);

    // Write to file if open
    if(file.is_open()){
        file << "STATUS: " << status;

        file.close();
    }
}

// Overload: Write the status of the output
void writeFile(std::string file_path, std::string status, std::unordered_map<std::string, std::string> outputs){
    // Open file
    std::ofstream file(file_path);

    // Write to file if open
    if(file.is_open()){

        for(const auto& [key, value] : outputs){
            file << key << ": " << outputs[key];
        }

        file << "STATUS: " << status;

        file.close();
    }
}

void compareFiles(){
    //TODO: Compare 2 output files to see if they have the same outputs
    // Main metrics: fail if any output is different, warning if an output does not exist(if status does not exist, fail)
}

void testFunction(){
    //TODO: Take in a function call/template and read in the input+write the output
}