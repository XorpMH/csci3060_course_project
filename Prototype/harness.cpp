#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include <algorithm>
#include <cctype>

#include "functions.h"
#include "game.h"

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
    for (const auto& entry :
         fs::recursive_directory_iterator(directory))
    {
        if (entry.is_regular_file() &&
            entry.path().filename() == "input.txt")
        {
            test_files.push_back(
                entry.path().string()
            );
        }
    }

    // Keep test order consistent
    std::sort(
        test_files.begin(),
        test_files.end()
    );

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
    const std::string& file_path1,
    const std::string& file_path2
)
{
    // Check that both files exist
    if (!fs::exists(file_path1) ||
        !fs::exists(file_path2))
    {
        return false;
    }

    // Read both files
    std::vector<std::string> file1 =
        readFile(file_path1);

    std::vector<std::string> file2 =
        readFile(file_path2);

    // Check SUCCESS
    bool file1_success =
        containsStatus(file1, "SUCCESS");

    bool file2_success =
        containsStatus(file2, "SUCCESS");

    if (file1_success && file2_success)
    {
        return true;
    }

    // Check FAILURE
    bool file1_failure =
        containsStatus(file1, "FAILURE");

    bool file2_failure =
        containsStatus(file2, "FAILURE");

    if (file1_failure && file2_failure)
    {
        return true;
    }

    return false;
}


// Check whether a line looks like a transaction
bool isTransactionLine(const std::string& line)
{
    if (line.length() < 2)
    {
        return false;
    }

    return std::isdigit(
               static_cast<unsigned char>(line[0])
           ) &&
           std::isdigit(
               static_cast<unsigned char>(line[1])
           );
}


// Generate actual_output.txt from input.txt
bool generateActualOutput(
    const std::string& input_file,
    const std::string& actual_file
)
{
    std::vector<std::string> input_lines =
        readFile(input_file);

    std::vector<std::string> actual_results;

    if (input_lines.empty())
    {
        actual_results.push_back(
            "FAILURE: Empty input file"
        );

        writeFile(
            actual_file,
            actual_results
        );

        return true;
    }

    for (const std::string& line : input_lines)
    {
        // Ignore completely empty lines
        if (line.empty())
        {
            continue;
        }

        // Transaction files begin with 00-06
        if (isTransactionLine(line))
        {
            actual_results.push_back(
                applyTransaction(line)
            );
        }
        else
        {
            // This input is not in daily transaction format
            actual_results.push_back(
                "FAILURE: Unsupported input format"
            );
        }
    }

    // Make sure something is written
    if (actual_results.empty())
    {
        actual_results.push_back(
            "FAILURE: Empty input file"
        );
    }

    writeFile(
        actual_file,
        actual_results
    );

    return true;
}


// Test the search function
void testSearchFunction()
{
    std::cout
        << "=== SEARCH FUNCTION TESTS ==="
        << std::endl;

    std::vector<Game> games = {
        Game(
            "Minecraft",
            "TestSeller",
            25.00
        ),
        Game(
            "Terraria",
            "TestSeller",
            15.00
        )
    };

    // Test 1 - Existing game
    Game* found_game =
        searchGame(
            games,
            "Minecraft"
        );

    if (found_game != nullptr)
    {
        std::cout
            << "Search existing game: PASS"
            << std::endl;
    }
    else
    {
        std::cout
            << "Search existing game: FAIL"
            << std::endl;
    }


    // Test 2 - Missing game
    Game* missing_game =
        searchGame(
            games,
            "NotExistGame"
        );

    if (missing_game == nullptr)
    {
        std::cout
            << "Search missing game: PASS"
            << std::endl;
    }
    else
    {
        std::cout
            << "Search missing game: FAIL"
            << std::endl;
    }

    std::cout << std::endl;
}


int main()
{
    // Test search function first
    testSearchFunction();


    // Find all test input files
    std::vector<std::string> test_files =
        findTestFiles("test_cases");


    // Check whether tests exist
    if (test_files.empty())
    {
        std::cout
            << "No test cases found."
            << std::endl;

        return 0;
    }


    int passed_tests = 0;
    int failed_tests = 0;


    std::cout
        << "=== TEST RESULTS ==="
        << std::endl;

    std::cout << std::endl;


    // Go through every test case
    for (const std::string& input_file :
         test_files)
    {
        fs::path input_path(input_file);

        // Folder containing the test
        fs::path test_directory =
            input_path.parent_path();


        // Expected output
        fs::path expected_file =
            test_directory /
            "expected_output.txt";


        // Actual output
        fs::path actual_file =
            test_directory /
            "actual_output.txt";


        std::string test_name =
            test_directory.string();


        std::cout
            << "Test: "
            << test_name
            << std::endl;


        // Expected output must exist
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


        // Generate actual_output.txt
        bool generated =
            generateActualOutput(
                input_file,
                actual_file.string()
            );


        if (!generated)
        {
            std::cout
                << "Result: FAIL"
                << std::endl;

            std::cout
                << "Reason: Could not generate actual output"
                << std::endl;

            failed_tests++;

            std::cout << std::endl;

            continue;
        }


        // Compare actual and expected output
        bool passed =
            compareFile(
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

            // Show expected status
            std::vector<std::string> expected_lines =
                readFile(
                    expected_file.string()
                );

            std::vector<std::string> actual_lines =
                readFile(
                    actual_file.string()
                );

            std::cout
                << "Expected status: ";

            if (containsStatus(
                    expected_lines,
                    "SUCCESS"))
            {
                std::cout
                    << "SUCCESS"
                    << std::endl;
            }
            else if (containsStatus(
                         expected_lines,
                         "FAILURE"))
            {
                std::cout
                    << "FAILURE"
                    << std::endl;
            }
            else
            {
                std::cout
                    << "UNKNOWN"
                    << std::endl;
            }


            std::cout
                << "Actual status: ";

            if (containsStatus(
                    actual_lines,
                    "SUCCESS"))
            {
                std::cout
                    << "SUCCESS"
                    << std::endl;
            }
            else if (containsStatus(
                         actual_lines,
                         "FAILURE"))
            {
                std::cout
                    << "FAILURE"
                    << std::endl;
            }
            else
            {
                std::cout
                    << "UNKNOWN"
                    << std::endl;
            }


            failed_tests++;
        }

        std::cout << std::endl;
    }


    // Final summary
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


    // Success only if every test passed
    if (failed_tests == 0)
    {
        return 0;
    }

    return 1;
}