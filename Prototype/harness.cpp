#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <filesystem>
#include "functions.h"

// Shorten std::filesystem to fs
namespace fs = std::filesystem;

// Recursively find all test input files
std::vector<std::string> findTestFiles(const std::string& directory)
{
    std::vector<std::string> test_files;

    // Check if the directory exists
    if (!fs::exists(directory)) {
        return test_files;
    }

    // Go through all files and subfolders
    for (const auto& entry : fs::recursive_directory_iterator(directory))
    {
        // Check if the file is named input.txt
        if (entry.is_regular_file() &&
            entry.path().filename() == "input.txt")
        {
            // Store the file path for later testing
            test_files.push_back(entry.path().string());
        }
    }

    return test_files;
}

int main()
{
    // Find all test input files
    std::vector<std::string> test_files = findTestFiles("test_cases");

    // TODO: Compare actual output with expected_out.txt

    // TODO: Display PASS/FAIL results

    return 0;
}