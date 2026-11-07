#include <string>
#include <vector>
#include <iostream>
#include <limits>
#include <iomanip>
#include <sstream>

#include "game.h"
#include "user.h"
#include "functions.h"

// Boolean controlling the main loop
bool running = true;

// Whether the user is currently logged in
bool logged_in = false;

// Stores daily transaction records
std::vector<std::string> transactions = {};

// Temporary users
std::vector<User> users = {
    User("adminuser", "admin", 500.00),
    User("existinguser", "FS", 100.00)
};

// Temporary available games
std::vector<Game> games = {};

// Stores the seller for each game
std::vector<std::string> game_sellers = {};

std::string current_username;
std::string current_user_type;

// Function declarations
void loginMenu();
void browseMenu();
bool login(const std::string& username);

// Transaction formatting functions
std::string accountTransaction(
    const std::string& code,
    const std::string& username,
    const std::string& type,
    double credit
);

std::string sellTransaction(
    const std::string& game_name,
    const std::string& seller,
    double price
);

std::string buyTransaction(
    const std::string& game_name,
    const std::string& seller,
    const std::string& buyer,
    double price
);

std::string refundTransaction(
    const std::string& buyer,
    const std::string& seller,
    double credit
);


// Main browsing loop
int main() {
    while (running) {
        if (logged_in) {
            browseMenu();
        }
        else {
            loginMenu();
        }
    }
    return 0;
}


// Menu used when the user is not logged in
void loginMenu() {
    std::string command;
    std::cout << "Enter command: ";
    std::getline(std::cin, command);
    if (command == "login") {
        std::string username;
        std::cout << "Enter username: ";
        std::getline(std::cin, username);

        login(username);
    }
    else {
        std::cout
            << "Invalid command. Please try again."
            << std::endl;
    }
}


// Menu used after the user has logged in
void browseMenu() {
    std::cout << std::endl;
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

    // Remove the newline left by std::cin
    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    switch (input_variable) {

        // Logout
        case 1: {

            // End session transaction
            transactions.push_back("00");

            logout(
                logged_in,
                current_username,
                current_user_type
            );

            break;
        }


        // Create account
        case 2: {

            std::string username;
            std::string password;
            std::string email;
            std::string type;

            std::cout << "Enter username: ";
            std::cin >> username;

            std::cout << "Enter password: ";
            std::cin >> password;

            std::cout << "Enter email: ";
            std::cin >> email;

            std::cout << "Enter user type: ";
            std::cin >> type;

            bool current_user_found = false;

            for (User& user : users) {

                if (user.getUsername() == current_username) {

                    current_user_found = true;

                    if (createUser(
                            user,
                            &users,
                            username,
                            password,
                            email,
                            type
                        )) {

                        std::cout
                            << "Account created successfully."
                            << std::endl;

                        // 01 = Create
                        transactions.push_back(
                            accountTransaction(
                                "01",
                                username,
                                type,
                                0.00
                            )
                        );
                    }
                    else {

                        std::cout
                            << "Account creation failed."
                            << std::endl;
                    }

                    break;
                }
            }

            if (!current_user_found) {

                std::cout
                    << "Account creation failed: current user not found."
                    << std::endl;
            }

            break;
        }


        // Delete account
        case 3: {
            std::string username;
            std::cout << "Enter username to delete: ";
            std::getline(std::cin, username);
            bool current_user_found = false;
            std::string deleted_type = "";
            double deleted_credit = 0.00;

            // Find the account before deleting it
            for (const User& user : users) {
                if (user.getUsername() == username) {
                    deleted_type = user.getUserType();
                    deleted_credit = user.getCredit();
                    break;
                }
            }
            for (User& user : users) {
                if (user.getUsername() == current_username) {
                    current_user_found = true;
                    if (deleteUser(
                            user,
                            &users,
                            username
                        )) {

                        std::cout
                            << "Account deleted successfully."
                            << std::endl;

                        // 02 = Delete
                        transactions.push_back(
                            accountTransaction(
                                "02",
                                username,
                                deleted_type,
                                deleted_credit
                            )
                        );
                    }
                    else {

                        std::cout
                            << "Account deletion failed."
                            << std::endl;
                    }

                    break;
                }
            }

            if (!current_user_found) {

                std::cout
                    << "Account deletion failed: current user not found."
                    << std::endl;
            }

            break;
        }


        // Sell game
        case 4: {

            std::string game_name;
            std::string game_description;
            double game_price;

            std::cout << "Enter game name: ";
            std::getline(std::cin, game_name);

            std::cout << "Enter game description: ";
            std::getline(std::cin, game_description);

            std::cout << "Enter game price: ";
            std::cin >> game_price;

            if (game_price < 0) {

                std::cout
                    << "Sell failed: invalid price."
                    << std::endl;

                break;
            }

            games.push_back(
                Game(
                    game_name,
                    game_description,
                    game_price
                )
            );

            // Store the seller beside the game
            game_sellers.push_back(current_username);

            std::cout
                << "Game listed for sale successfully."
                << std::endl;

            // 03 = Sell
            transactions.push_back(
                sellTransaction(
                    game_name,
                    current_username,
                    game_price
                )
            );

            break;
        }


        // Buy game
        case 5: {

            std::string game_name;
            bool game_found = false;
            bool user_found = false;

            std::cout << "Enter game name: ";
            std::getline(std::cin, game_name);

            for (size_t i = 0; i < games.size(); i++) {

                Game& game = games[i];

                if (game.getGameName() == game_name) {

                    game_found = true;

                    std::string seller_username;

                    if (i < game_sellers.size()) {
                        seller_username = game_sellers[i];
                    }

                    for (User& user : users) {

                        if (user.getUsername() == current_username) {

                            user_found = true;

                            if (buy(user, game)) {

                                std::cout
                                    << "Buy successful."
                                    << std::endl;

                                // 04 = Buy
                                transactions.push_back(
                                    buyTransaction(
                                        game_name,
                                        seller_username,
                                        current_username,
                                        game.getGamePrice()
                                    )
                                );
                            }
                            else {

                                std::cout
                                    << "Buy failed: insufficient credit."
                                    << std::endl;
                            }

                            break;
                        }
                    }

                    break;
                }
            }

            if (!game_found) {

                std::cout
                    << "Buy failed: game not found."
                    << std::endl;
            }
            else if (!user_found) {

                std::cout
                    << "Buy failed: current user not found."
                    << std::endl;
            }

            break;
        }


        // Refund
        case 6: {

            std::string buyer_username;
            std::string seller_username;
            double amount;

            std::cout << "Enter buyer username: ";
            std::cin >> buyer_username;

            std::cout << "Enter seller username: ";
            std::cin >> seller_username;

            std::cout << "Enter refund amount: ";
            std::cin >> amount;

            User* buyer = nullptr;
            User* seller = nullptr;

            for (User& user : users) {

                if (user.getUsername() == buyer_username) {
                    buyer = &user;
                }

                if (user.getUsername() == seller_username) {
                    seller = &user;
                }
            }

            if (buyer == nullptr || seller == nullptr) {

                std::cout
                    << "Refund failed: user not found."
                    << std::endl;

                break;
            }

            if (refund(
                    *buyer,
                    *seller,
                    amount
                )) {

                std::cout
                    << "Refund successful."
                    << std::endl;

                // 05 = Refund
                transactions.push_back(
                    refundTransaction(
                        buyer_username,
                        seller_username,
                        amount
                    )
                );
            }
            else {

                std::cout
                    << "Refund failed."
                    << std::endl;
            }

            break;
        }


        // Add credit
        case 7: {

            std::string username;
            double amount;

            std::cout << "Enter username: ";
            std::cin >> username;

            std::cout << "Enter credit amount: ";
            std::cin >> amount;

            bool user_found = false;

            for (User& user : users) {

                if (user.getUsername() == username) {

                    user.addCredit(amount);

                    user_found = true;

                    std::cout
                        << "Credit added successfully."
                        << std::endl;

                    // 06 = Add credit
                    transactions.push_back(
                        accountTransaction(
                            "06",
                            username,
                            user.getUserType(),
                            amount
                        )
                    );

                    break;
                }
            }

            if (!user_found) {

                std::cout
                    << "Add credit failed: user not found."
                    << std::endl;
            }

            break;
        }


        // List transactions
        case 8: {

            std::cout << std::endl;
            std::cout << "===TRANSACTIONS===" << std::endl;

            if (transactions.empty()) {

                std::cout
                    << "No transactions found."
                    << std::endl;
            }
            else {

                for (const std::string& transaction : transactions) {

                    std::cout
                        << transaction
                        << std::endl;
                }

                std::cout
                    << "Total transactions: "
                    << transactions.size()
                    << std::endl;
            }

            break;
        }


        // List accounts
        case 9: {

            listAccounts(users);

            break;
        }


        default: {

            std::cout
                << "Invalid input, please select an option from 1 to 9 and try again..."
                << std::endl;

            break;
        }
    }
}


// Log in to an existing user account
bool login(const std::string& username) {

    if (logged_in) {

        std::cout
            << "Login failed: user is already logged in"
            << std::endl;

        return false;
    }

    for (const User& user : users) {

        if (user.getUsername() == username) {

            logged_in = true;

            current_username = user.getUsername();

            current_user_type = user.getUserType();

            std::cout
                << "Login successful"
                << std::endl;

            return true;
        }
    }

    std::cout
        << "Login failed: user does not exist"
        << std::endl;

    return false;
}


// Format 01 Create, 02 Delete and 06 Add Credit
//
// Format:
// XX UUUUUUUUUUUUUUU TT CCCCCCCCC
std::string accountTransaction(
    const std::string& code,
    const std::string& username,
    const std::string& type,
    double credit
) {

    std::ostringstream output;

    // Limit username to 15 characters
    std::string formatted_username =
        username.substr(0, 15);

    // Limit user type to 2 characters
    std::string formatted_type =
        type.substr(0, 2);

    output
        << code
        << " "

        << std::left
        << std::setfill(' ')
        << std::setw(15)
        << formatted_username

        << " "

        << std::left
        << std::setw(2)
        << formatted_type

        << " "

        << std::right
        << std::setfill('0')
        << std::setw(9)
        << std::fixed
        << std::setprecision(2)
        << credit;

    return output.str();
}


// Format 03 Sell
//
// Format:
// 03 IIIIIIIIIIIIIIIIIIIIIIIII SSSSSSSSSSSSSSS PPPPPP
std::string sellTransaction(
    const std::string& game_name,
    const std::string& seller,
    double price
) {

    std::ostringstream output;

    std::string formatted_game =
        game_name.substr(0, 25);

    std::string formatted_seller =
        seller.substr(0, 15);

    output
        << "03"
        << " "

        << std::left
        << std::setfill(' ')
        << std::setw(25)
        << formatted_game

        << " "

        << std::left
        << std::setw(15)
        << formatted_seller

        << " "

        << std::right
        << std::setfill('0')
        << std::setw(6)
        << std::fixed
        << std::setprecision(2)
        << price;

    return output.str();
}


// Format 04 Buy
//
// Format:
// 04 IIIIIIIIIIIIIIIIIIIIIIIII SSSSSSSSSSSSSSS UUUUUUUUUUUUUUU PPPPPP
std::string buyTransaction(
    const std::string& game_name,
    const std::string& seller,
    const std::string& buyer,
    double price
) {

    std::ostringstream output;

    std::string formatted_game =
        game_name.substr(0, 25);

    std::string formatted_seller =
        seller.substr(0, 15);

    std::string formatted_buyer =
        buyer.substr(0, 15);

    output
        << "04"
        << " "

        << std::left
        << std::setfill(' ')
        << std::setw(25)
        << formatted_game

        << " "

        << std::left
        << std::setw(15)
        << formatted_seller

        << " "

        << std::left
        << std::setw(15)
        << formatted_buyer

        << " "

        << std::right
        << std::setfill('0')
        << std::setw(6)
        << std::fixed
        << std::setprecision(2)
        << price;

    return output.str();
}


// Format 05 Refund
//
// Format:
// 05 UUUUUUUUUUUUUUU SSSSSSSSSSSSSSS CCCCCCCCC
std::string refundTransaction(
    const std::string& buyer,
    const std::string& seller,
    double credit
) {

    std::ostringstream output;

    std::string formatted_buyer =
        buyer.substr(0, 15);

    std::string formatted_seller =
        seller.substr(0, 15);

    output
        << "05"
        << " "

        << std::left
        << std::setfill(' ')
        << std::setw(15)
        << formatted_buyer

        << " "

        << std::left
        << std::setw(15)
        << formatted_seller

        << " "

        << std::right
        << std::setfill('0')
        << std::setw(9)
        << std::fixed
        << std::setprecision(2)
        << credit;

    return output.str();
}