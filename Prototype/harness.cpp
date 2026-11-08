#include <string>
#include <vector>
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
    if (!fs::exists(directory))
    {
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


// Check if a file contains a specific status
bool containsStatus(
    const std::vector<std::string>& file_lines,
    const std::string& status
)
{
    for (const std::string& line : file_lines)
    {
        if (line.find(status) != std::string::npos)
        {
            return true;
        }
    }

    return false;
}


// Compare expected output and actual output
bool compareFile(
    std::string file_path1,
    std::string file_path2
)
{
    // Check that both files exist
    if (!fs::exists(file_path1) || !fs::exists(file_path2))
    {
        return false;
    }

    // Use readFile from functions.cpp
    std::vector<std::string> file1 = readFile(file_path1);
    std::vector<std::string> file2 = readFile(file_path2);

    // Check whether both files contain SUCCESS
    bool file1_success = containsStatus(file1, "SUCCESS");
    bool file2_success = containsStatus(file2, "SUCCESS");

    if (file1_success && file2_success)
    {
        return true;
    }

    // Check whether both files contain FAILURE
    bool file1_failure = containsStatus(file1, "FAILURE");
    bool file2_failure = containsStatus(file2, "FAILURE");

    if (file1_failure && file2_failure)
    {
        return true;
    }

    // Statuses do not match
    return false;
}

void testSearchFunction()
{
    std::vector<Game> games = {
        Game("Minecraft", "Test game", 25.00),
        Game("Terraria", "Test game", 15.00)
    };

    // Test 1: Game exists
    Game* found_game = searchGame(games, "Minecraft");

    if (found_game != nullptr)
    {
        std::cout << "Search existing game: PASS" << std::endl;
    }
    else
    {
        std::cout << "Search existing game: FAIL" << std::endl;
    }

    // Test 2: Game does not exist
    Game* missing_game = searchGame(games, "NotExistGame");

    if (missing_game == nullptr)
    {
        std::cout << "Search missing game: PASS" << std::endl;
    }
    else
    {
        std::cout << "Search missing game: FAIL" << std::endl;
    }
}

int main()
{
    testSearchFunction();
    // Find all test input files
    std::vector<std::string> test_files =
        findTestFiles("test_cases");

    // Check if any tests were found
    if (test_files.empty())
    {
        std::cout
            << "No test cases found."
            << std::endl;

        return 0;
    }

    int passed_tests = 0;
    int failed_tests = 0;

    std::cout << "=== TEST RESULTS ===" << std::endl;
    std::cout << std::endl;

    // Go through every test case
    for (const std::string& input_file : test_files)
    {
        fs::path input_path(input_file);

        // Get the folder containing input.txt
        fs::path test_directory =
            input_path.parent_path();

        // Expected output file
        fs::path expected_file =
            test_directory / "expected_output.txt";

        // Actual output file
        fs::path actual_file =
            test_directory / "actual_output.txt";

        // Get a readable test name
        std::string test_name =
            test_directory.string();

        std::cout
            << "Test: "
            << test_name
            << std::endl;

        // Check expected output exists
        if (!fs::exists(expected_file))
        {
            std::cout
                << "Result: FAIL"
                << std::endl;

            std::cout
                << "Reason: expected_output.txt not found"
                << std::endl;

            failed_tests++;

            std::cout << std::endl;

            continue;
        }

        // Check actual output exists
        if (!fs::exists(actual_file))
        {
            std::cout
                << "Result: FAIL"
                << std::endl;

            std::cout
                << "Reason: actual_output.txt not found"
                << std::endl;

            failed_tests++;

            std::cout << std::endl;

            continue;
        }

        // Compare expected and actual output
        bool passed = compareFile(
            expected_file.string(),
            actual_file.string()
        );

        if (passed)
        {
            std::cout
                << "Result: PASS"
                << std::endl;

            passed_tests++;
        }
        else
        {
            std::cout
                << "Result: FAIL"
                << std::endl;

            std::cout
                << "Reason: Expected and actual status do not match"
                << std::endl;

            failed_tests++;
        }

        std::cout << std::endl;
    }


    // Display final results
    std::cout
        << "=== TEST SUMMARY ==="
        << std::endl;

    std::cout
        << "Passed: "
        << passed_tests
        << std::endl;

    std::cout
        << "Failed: "
        << failed_tests
        << std::endl;

    std::cout
        << "Total: "
        << test_files.size()
        << std::endl;


    // Return 0 if all tests passed
    if (failed_tests == 0)
    {
        return 0;
    }

    // Return 1 if at least one test failed
    return 1;
}