#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

#include "functions.h"
#include "game.h"
#include "user.h"

bool running = true;
bool logged_in = false;

std::vector<std::string> transactions = {};

std::vector<User> users = {
    User("adminuser", "AA", 500.00),
    User("existinguser", "FS", 100.00)
};

std::vector<Game> games = {};
std::vector<Game> pending_games = {};

std::string current_username;
std::string current_user_type;

void loginMenu();
void browseMenu();

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

User* findFrontendUser(const std::string& username)
{
    for (User& user : users) {
        if (user.getUsername() == username) {
            return &user;
        }
    }

    return nullptr;
}

int main(int argc, char* argv[])
{
    // File mode: evaluate every transaction in the supplied file
    if (argc > 1) {
        std::vector<std::string> file_transactions = readFile(argv[1]);

        if (file_transactions.empty()) {
            std::cout << "No transactions found." << std::endl;
            return 1;
        }

        for (const std::string& transaction : file_transactions) {
            std::cout << applyTransaction(transaction) << std::endl;
        }

        return 0;
    }

    // Normal interactive mode
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

void loginMenu()
{
    std::string command;

    std::cout << "Enter command: ";
    std::getline(std::cin, command);

    if (command == "login") {
        std::string username;

        std::cout << "Enter username: ";
        std::getline(std::cin, username);

        login(
            username,
            users,
            logged_in,
            current_username,
            current_user_type
        );
    }
    else {
        std::cout
            << "Invalid command. Please try again."
            << std::endl;
    }
}

void browseMenu()
{
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

    if (!(std::cin >> input_variable)) {
        std::cin.clear();
        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cout << "Invalid input." << std::endl;
        return;
    }

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    switch (input_variable) {
        case 1: {
            User* current_user = findFrontendUser(current_username);

            if (current_user != nullptr) {
                transactions.push_back(
                    accountTransaction(
                        "00",
                        current_user->getUsername(),
                        current_user->getUserType(),
                        current_user->getCredit()
                    )
                );
            }

            dailyTransactionsConstructor(
                transactions,
                getDailyTransactionsPath()
            );

            logout(
                logged_in,
                current_username,
                current_user_type
            );

            break;
        }

        case 2: {
            std::string username;
            std::string type;

            std::cout << "Enter username: ";
            std::getline(std::cin, username);

            std::cout << "Enter user type (AA/FS/BS/SS): ";
            std::getline(std::cin, type);

            if (createUser(users, username, type)) {
                std::cout
                    << "Account created successfully."
                    << std::endl;

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

        case 3: {
            std::string username;

            std::cout << "Enter username to delete: ";
            std::getline(std::cin, username);

            User* user_to_delete = findFrontendUser(username);

            std::string deleted_type;
            double deleted_credit = 0.00;

            if (user_to_delete != nullptr) {
                deleted_type = user_to_delete->getUserType();
                deleted_credit = user_to_delete->getCredit();
            }

            if (deleteUser(users, username)) {
                std::cout
                    << "Account deleted successfully."
                    << std::endl;

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

        case 4: {
            std::string game_name;
            double game_price;

            std::cout << "Enter game name: ";
            std::getline(std::cin, game_name);

            std::cout << "Enter game price: ";
            std::cin >> game_price;

            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );

            if (sell(
                    game_name,
                    game_price,
                    games,
                    pending_games
                )) {

                std::cout
                    << "Game listed for sale successfully."
                    << std::endl;

                transactions.push_back(
                    sellTransaction(
                        game_name,
                        current_username,
                        game_price
                    )
                );
            }
            else {
                std::cout
                    << "Sell failed."
                    << std::endl;
            }

            break;
        }

        case 5: {
            std::string game_name;

            std::cout << "Enter game name: ";
            std::getline(std::cin, game_name);

            Game* game = searchGame(games, game_name);
            User* buyer = findFrontendUser(current_username);

            if (game == nullptr) {
                std::cout
                    << "Buy failed: game not found."
                    << std::endl;

                break;
            }

            if (buyer == nullptr) {
                std::cout
                    << "Buy failed: current user not found."
                    << std::endl;

                break;
            }

            if (buy(*buyer, *game)) {
                std::cout << "Buy successful." << std::endl;

                transactions.push_back(
                    buyTransaction(
                        game->getGameName(),
                        game->getSellerName(),
                        buyer->getUsername(),
                        game->getGamePrice()
                    )
                );
            }
            else {
                std::cout
                    << "Buy failed."
                    << std::endl;
            }

            break;
        }

        case 6: {
            std::string buyer_username;
            std::string seller_username;
            double amount;

            std::cout << "Enter buyer username: ";
            std::getline(std::cin, buyer_username);

            std::cout << "Enter seller username: ";
            std::getline(std::cin, seller_username);

            std::cout << "Enter refund amount: ";
            std::cin >> amount;

            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );

            User* buyer = findFrontendUser(buyer_username);
            User* seller = findFrontendUser(seller_username);

            if (buyer == nullptr || seller == nullptr) {
                std::cout
                    << "Refund failed: user not found."
                    << std::endl;

                break;
            }

            if (refund(*buyer, *seller, amount)) {
                std::cout
                    << "Refund successful."
                    << std::endl;

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

        case 7: {
            if (current_user_type != "AA") {
                std::cout
                    << "Add credit failed: admin access required."
                    << std::endl;

                break;
            }

            std::string username;
            double amount;

            std::cout << "Enter username: ";
            std::getline(std::cin, username);

            std::cout << "Enter credit amount: ";
            std::cin >> amount;

            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );

            if (amount <= 0 || amount > 1000.00) {
                std::cout
                    << "Add credit failed: amount must be between $0.01 and $1000.00."
                    << std::endl;

                break;
            }

            User* user = findFrontendUser(username);

            if (user == nullptr) {
                std::cout
                    << "Add credit failed: user not found."
                    << std::endl;

                break;
            }

            user->addCredit(amount);

            std::cout
                << "Credit added successfully."
                << std::endl;

            transactions.push_back(
                accountTransaction(
                    "06",
                    username,
                    user->getUserType(),
                    amount
                )
            );

            break;
        }

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

// Format 00, 01, 02 and 06
std::string accountTransaction(
    const std::string& code,
    const std::string& username,
    const std::string& type,
    double credit
)
{
    std::ostringstream output;

    output << code << " "
           << std::left
           << std::setfill(' ')
           << std::setw(15)
           << username.substr(0, 15)
           << " "
           << std::setw(2)
           << type.substr(0, 2)
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
std::string sellTransaction(
    const std::string& game_name,
    const std::string& seller,
    double price
)
{
    std::ostringstream output;

    output << "03 "
           << std::left
           << std::setfill(' ')
           << std::setw(19)
           << game_name.substr(0, 19)
           << " "
           << std::setw(15)
           << seller.substr(0, 15)
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
std::string buyTransaction(
    const std::string& game_name,
    const std::string& seller,
    const std::string& buyer,
    double price
)
{
    std::ostringstream output;

    output << "04 "
           << std::left
           << std::setfill(' ')
           << std::setw(19)
           << game_name.substr(0, 19)
           << " "
           << std::setw(15)
           << seller.substr(0, 15)
           << " "
           << std::setw(15)
           << buyer.substr(0, 15)
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
std::string refundTransaction(
    const std::string& buyer,
    const std::string& seller,
    double credit
)
{
    std::ostringstream output;

    output << "05 "
           << std::left
           << std::setfill(' ')
           << std::setw(15)
           << buyer.substr(0, 15)
           << " "
           << std::setw(15)
           << seller.substr(0, 15)
           << " "
           << std::right
           << std::setfill('0')
           << std::setw(9)
           << std::fixed
           << std::setprecision(2)
           << credit;

    return output.str();
}
