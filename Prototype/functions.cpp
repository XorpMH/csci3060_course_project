#include "functions.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace {
    bool backend_logged_in = false;
    std::string backend_username;
    std::string backend_user_type;

    std::string daily_transactions_path = "assets/daily_transactions.txt";
    std::string user_accounts_path = "assets/user_accounts.txt";
    std::string available_games_path = "assets/available_games.txt";
    std::string game_collection_path = "assets/game_collection.txt";

    User* findUser(std::vector<User>& users, const std::string& username)
    {
        for (User& user : users) {
            if (user.getUsername() == username) {
                return &user;
            }
        }
        return nullptr;
    }
}

// Login function
bool login(
    const std::string& username,
    const std::vector<User>& users,
    bool& logged_in,
    std::string& current_username,
    std::string& current_user_type
)
{
    if (logged_in) {
        std::cout << "Login failed: user is already logged in\n";
        return false;
    }

    for (const User& user : users) {
        if (user.getUsername() == username) {
            logged_in = true;
            current_username = user.getUsername();
            current_user_type = user.getUserType();

            backend_logged_in = true;
            backend_username = current_username;
            backend_user_type = current_user_type;

            std::cout << "Login successful\n";
            return true;
        }
    }

    std::cout << "Login failed: user does not exist\n";
    return false;
}

// Logout function
void logout(
    bool& logged_in,
    std::string& current_username,
    std::string& current_user_type
)
{
    if (!logged_in) {
        std::cout << "Logout failed: no user is logged in\n";
        return;
    }

    logged_in = false;
    current_username.clear();
    current_user_type.clear();

    backend_logged_in = false;
    backend_username.clear();
    backend_user_type.clear();

    std::cout << "Logout successful\n";
}

// List accounts
void listAccounts(const std::vector<User>& users)
{
    if (users.empty()) {
        std::cout << "No accounts found." << std::endl;
        return;
    }

    for (const User& user : users) {
        std::cout << "Username: " << user.getUsername()
                  << ", Type: " << user.getUserType()
                  << ", Credit: $" << user.getCredit()
                  << std::endl;
    }
}

// Create a new user account
bool createUser(
    std::vector<User>& users,
    const std::string& username,
    const std::string& type
)
{
    if (!backend_logged_in || backend_user_type != "AA") {
        return false;
    }

    if (username.empty() || username.length() > 15) {
        return false;
    }

    if (!typeValid(type)) {
        return false;
    }

    for (const User& user : users) {
        if (user.getUsername() == username) {
            return false;
        }
    }

    users.push_back(User(username, type));
    return true;
}

// Delete an existing user account
bool deleteUser(
    std::vector<User>& users,
    const std::string& username
)
{
    if (!backend_logged_in || backend_user_type != "AA") {
        return false;
    }

    // Do not allow the currently logged-in user to delete themselves
    if (username == backend_username) {
        return false;
    }

    for (auto it = users.begin(); it != users.end(); ++it) {
        if (it->getUsername() == username) {
            users.erase(it);
            return true;
        }
    }

    return false;
}

// Put a game up for sale
bool sell(
    const std::string& gamename,
    double gameprice,
    const std::vector<Game>& available_games,
    std::vector<Game>& pending_games
)
{
    if (!backend_logged_in) {
        std::cout << "Error: User must be logged in to sell a game.\n";
        return false;
    }

    if (backend_user_type == "BS") {
        std::cout << "Error: Standard-buy accounts cannot sell games.\n";
        return false;
    }

    if (gamename.empty() || gamename.length() > 19) {
        std::cout << "Error: Game name must be between 1 and 19 characters.\n";
        return false;
    }

    if (!std::isfinite(gameprice) || gameprice < 0 || gameprice > 999.99) {
        std::cout << "Error: Game price must be between $0.00 and $999.99.\n";
        return false;
    }

    for (const Game& game : available_games) {
        if (game.getGameName() == gamename) {
            std::cout << "Error: Game name already exists.\n";
            return false;
        }
    }

    for (const Game& game : pending_games) {
        if (game.getGameName() == gamename) {
            std::cout << "Error: Game name already exists.\n";
            return false;
        }
    }

    pending_games.push_back(Game(gamename, backend_username, gameprice));
    return true;
}

// Search for a game by name
Game* searchGame(
    std::vector<Game>& games,
    const std::string& game_name
)
{
    for (Game& game : games) {
        if (game.getGameName() == game_name) {
            return &game;
        }
    }

    return nullptr;
}

// Purchase a game
bool buy(User& buyer, const Game& game)
{
    if (buyer.getUserType() == "SS") {
        return false;
    }

    if (buyer.getCredit() < game.getGamePrice()) {
        return false;
    }

    return buyer.deductCredit(game.getGamePrice());
}

// Refund a purchase
bool refund(User& buyer, User& seller, double amount)
{
    if (amount <= 0) {
        return false;
    }

    if (!seller.deductCredit(amount)) {
        return false;
    }

    buyer.addCredit(amount);
    return true;
}

// Build the Current User Accounts file
void currentUserAccountsConstructor(const std::vector<User>& users)
{
    std::ofstream file(user_accounts_path);

    if (!file.is_open()) {
        return;
    }

    for (const User& user : users) {
        file << std::left
             << std::setfill(' ')
             << std::setw(15)
             << user.getUsername().substr(0, 15)
             << " "
             << std::setw(2)
             << user.getUserType().substr(0, 2)
             << " "
             << std::right
             << std::setfill('0')
             << std::setw(9)
             << std::fixed
             << std::setprecision(2)
             << user.getCredit()
             << "\n";
    }
}

// Read the Current User Accounts file
std::vector<User> currentUserAccounts()
{
    std::vector<User> users;
    std::ifstream file(user_accounts_path);

    if (!file.is_open()) {
        return users;
    }

    std::string line;

    while (std::getline(file, line)) {
        if (line.size() < 28) {
            continue;
        }

        try {
            std::string username = rtrim(line.substr(0, 15));
            std::string user_type = rtrim(line.substr(16, 2));
            double credit = std::stod(line.substr(19, 9));

            if (!username.empty() && username != "END") {
                users.push_back(User(username, user_type, credit));
            }
        }
        catch (...) {
            // Ignore malformed lines
        }
    }

    return users;
}

// Build the Available Games file
void availableGamesConstructor(const std::vector<Game>& games)
{
    std::ofstream file(available_games_path);

    if (!file.is_open()) {
        return;
    }

    for (const Game& game : games) {
        file << std::left
             << std::setfill(' ')
             << std::setw(19)
             << game.getGameName().substr(0, 19)
             << " "
             << std::setw(15)
             << game.getSellerName().substr(0, 15)
             << " "
             << std::right
             << std::setfill('0')
             << std::setw(6)
             << std::fixed
             << std::setprecision(2)
             << game.getGamePrice()
             << "\n";
    }
}

// Read the Available Games file
std::vector<Game> availableGames()
{
    std::vector<Game> games;
    std::ifstream file(available_games_path);

    if (!file.is_open()) {
        return games;
    }

    std::string line;

    while (std::getline(file, line)) {
        if (line.size() < 42) {
            continue;
        }

        try {
            std::string game_name = rtrim(line.substr(0, 19));
            std::string seller_name = rtrim(line.substr(20, 15));
            double price = std::stod(line.substr(36, 6));

            if (!game_name.empty() && game_name != "END") {
                games.push_back(Game(game_name, seller_name, price));
            }
        }
        catch (...) {
            // Ignore malformed lines
        }
    }

    return games;
}

// Add ownership to the Game Collection file
void appendGameCollection(const Game& game, const User& owner)
{
    std::ofstream file(game_collection_path, std::ios::app);

    if (!file.is_open()) {
        return;
    }

    file << std::left
         << std::setfill(' ')
         << std::setw(19)
         << game.getGameName().substr(0, 19)
         << " "
         << std::setw(15)
         << owner.getUsername().substr(0, 15)
         << "\n";
}

// Remove ownership from the Game Collection file
void removeGameCollection(const Game& game, const User& owner)
{
    std::vector<std::string> lines = readFile(game_collection_path);
    std::ofstream file(game_collection_path);

    if (!file.is_open()) {
        return;
    }

    for (const std::string& line : lines) {
        if (line.size() < 35) {
            continue;
        }

        std::string game_name = rtrim(line.substr(0, 19));
        std::string username = rtrim(line.substr(20, 15));

        if (game.getGameName() == game_name &&
            owner.getUsername() == username) {
            continue;
        }

        file << line << "\n";
    }
}

// Get games owned by a user
std::vector<Game> gameCollection(
    const std::vector<Game>& available_games,
    const User& owner
)
{
    std::vector<Game> games;
    std::ifstream file(game_collection_path);

    if (!file.is_open()) {
        return games;
    }

    std::string line;

    while (std::getline(file, line)) {
        if (line.size() < 35) {
            continue;
        }

        std::string game_name = rtrim(line.substr(0, 19));
        std::string username = rtrim(line.substr(20, 15));

        if (owner.getUsername() != username) {
            continue;
        }

        for (const Game& game : available_games) {
            if (game.getGameName() == game_name) {
                games.push_back(game);
                break;
            }
        }
    }

    return games;
}

// Create the Daily Transactions file
void dailyTransactionsConstructor(
    const std::vector<std::string>& transactions,
    const std::string& file_path
)
{
    writeFile(file_path, transactions);
}

// Apply every transaction from a file
void applyDailyTransactions(const std::string& file_path)
{
    std::vector<std::string> transactions = readFile(file_path);

    for (const std::string& transaction : transactions) {
        std::cout << applyTransaction(transaction) << std::endl;
    }
}

// Apply one transaction
std::string applyTransaction(const std::string& transaction)
{
    if (transaction.size() < 2) {
        return "FAILURE: Improper format";
    }

    try {
        int code = std::stoi(transaction.substr(0, 2));

        switch (code) {
            case 0: {
                if (transaction.length() != 31) {
                    return "FAILURE: Improper format";
                }

                if (!backend_logged_in) {
                    return "FAILURE: User is not logged in";
                }

                std::string username = rtrim(transaction.substr(3, 15));
                std::string user_type = rtrim(transaction.substr(19, 2));

                bool logged_in = true;
                logout(logged_in, username, user_type);

                return "SUCCESS: Transaction completed";
            }

            case 1: {
                if (transaction.length() != 31) {
                    return "FAILURE: Improper format";
                }

                std::string username = rtrim(transaction.substr(3, 15));
                std::string user_type = rtrim(transaction.substr(19, 2));
                double credit = std::stod(transaction.substr(22, 9));

                std::vector<User> users = currentUserAccounts();

                for (const User& user : users) {
                    if (user.getUsername() == username) {
                        return "FAILURE: Transaction denied";
                    }
                }

                if (!typeValid(user_type) || username.empty() ||
                    username.length() > 15 || credit != 0.0) {
                    return "FAILURE: Transaction denied";
                }

                users.push_back(User(username, user_type, credit));
                currentUserAccountsConstructor(users);

                return "SUCCESS: Transaction completed";
            }

            case 2: {
                if (transaction.length() != 31) {
                    return "FAILURE: Improper format";
                }

                std::string username = rtrim(transaction.substr(3, 15));
                std::vector<User> users = currentUserAccounts();

                bool removed = false;

                for (auto it = users.begin(); it != users.end(); ++it) {
                    if (it->getUsername() == username) {
                        users.erase(it);
                        removed = true;
                        break;
                    }
                }

                if (!removed) {
                    return "FAILURE: Transaction denied";
                }

                currentUserAccountsConstructor(users);
                return "SUCCESS: Transaction completed";
            }

            case 3: {
                if (transaction.length() != 45) {
                    return "FAILURE: Improper format";
                }

                std::string game_name = rtrim(transaction.substr(3, 19));
                std::string seller_name = rtrim(transaction.substr(23, 15));
                double price = std::stod(transaction.substr(39, 6));

                if (game_name.empty() || game_name.length() > 19 ||
                    seller_name.empty() ||
                    price < 0 || price > 999.99) {
                    return "FAILURE: Transaction denied";
                }

                std::vector<Game> games = availableGames();

                for (const Game& game : games) {
                    if (game.getGameName() == game_name) {
                        return "FAILURE: Transaction denied";
                    }
                }

                games.push_back(Game(game_name, seller_name, price));
                availableGamesConstructor(games);

                return "SUCCESS: Transaction completed";
            }

            case 4: {
                if (transaction.length() != 61) {
                    return "FAILURE: Improper format";
                }

                std::string game_name = rtrim(transaction.substr(3, 19));
                std::string seller_name = rtrim(transaction.substr(23, 15));
                std::string buyer_name = rtrim(transaction.substr(39, 15));
                double price = std::stod(transaction.substr(55, 6));

                std::vector<Game> games = availableGames();
                std::vector<User> users = currentUserAccounts();

                Game* game = searchGame(games, game_name);
                User* buyer = findUser(users, buyer_name);
                User* seller = findUser(users, seller_name);

                if (game == nullptr || buyer == nullptr || seller == nullptr) {
                    return "FAILURE: Transaction denied";
                }

                if (buyer->getUserType() == "SS" ||
                    buyer->getCredit() < price) {
                    return "FAILURE: Transaction denied";
                }

                if (!buyer->deductCredit(price)) {
                    return "FAILURE: Transaction denied";
                }

                seller->addCredit(price);

                currentUserAccountsConstructor(users);
                appendGameCollection(*game, *buyer);

                return "SUCCESS: Transaction completed";
            }

            case 5: {
                if (transaction.length() != 44) {
                    return "FAILURE: Improper format";
                }

                std::string buyer_name = rtrim(transaction.substr(3, 15));
                std::string seller_name = rtrim(transaction.substr(19, 15));
                double refund_credit = std::stod(transaction.substr(35, 9));

                std::vector<User> users = currentUserAccounts();

                User* buyer = findUser(users, buyer_name);
                User* seller = findUser(users, seller_name);

                if (buyer == nullptr || seller == nullptr ||
                    refund_credit <= 0) {
                    return "FAILURE: Transaction denied";
                }

                if (!refund(*buyer, *seller, refund_credit)) {
                    return "FAILURE: Transaction denied";
                }

                currentUserAccountsConstructor(users);
                return "SUCCESS: Transaction completed";
            }

            case 6: {
                if (transaction.length() != 31) {
                    return "FAILURE: Improper format";
                }

                std::string username = rtrim(transaction.substr(3, 15));
                double credit = std::stod(transaction.substr(22, 9));

                if (credit <= 0 || credit > 1000.00) {
                    return "FAILURE: Transaction denied";
                }

                std::vector<User> users = currentUserAccounts();
                User* user = findUser(users, username);

                if (user == nullptr) {
                    return "FAILURE: Transaction denied";
                }

                user->addCredit(credit);
                currentUserAccountsConstructor(users);

                return "SUCCESS: Transaction completed";
            }

            default:
                return "FAILURE: No corresponding transaction";
        }
    }
    catch (...) {
        return "FAILURE: Improper format";
    }
}

// File path setters/getters
void setDailyTransactionsPath(const std::string& path)
{
    daily_transactions_path = path;
}

void setUserAccountsPath(const std::string& path)
{
    user_accounts_path = path;
}

void setAvailableGamesPath(const std::string& path)
{
    available_games_path = path;
}

void setGameCollectionPath(const std::string& path)
{
    game_collection_path = path;
}

std::string getDailyTransactionsPath()
{
    return daily_transactions_path;
}

std::string getUserAccountsPath()
{
    return user_accounts_path;
}

std::string getAvailableGamesPath()
{
    return available_games_path;
}

std::string getGameCollectionPath()
{
    return game_collection_path;
}

// Append one line to a file
void appendToFile(
    const std::string& file_path,
    const std::string& line
)
{
    std::ofstream file(file_path, std::ios::app);

    if (file.is_open()) {
        file << line << "\n";
    }
}

// Remove one exact line from a file
void removeFromFile(
    const std::string& file_path,
    const std::string& line_to_remove
)
{
    std::vector<std::string> lines = readFile(file_path);
    std::ofstream file(file_path);

    if (!file.is_open()) {
        return;
    }

    for (const std::string& line : lines) {
        if (line != line_to_remove) {
            file << line << "\n";
        }
    }
}

// Read all lines from a file
std::vector<std::string> readFile(const std::string& file_path)
{
    std::vector<std::string> lines;
    std::ifstream file(file_path);

    if (!file.is_open()) {
        return lines;
    }

    std::string line;

    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    return lines;
}

// Write all lines to a file
void writeFile(
    const std::string& file_path,
    const std::vector<std::string>& lines
)
{
    std::ofstream file(file_path);

    if (!file.is_open()) {
        return;
    }

    for (const std::string& line : lines) {
        file << line << "\n";
    }
}

// Validation helpers
bool passwordValid(const std::string& password)
{
    return !password.empty();
}

bool emailValid(const std::string& email)
{
    return !email.empty() && email.find('@') != std::string::npos;
}

bool typeValid(const std::string& type)
{
    return type == "AA" ||
           type == "FS" ||
           type == "BS" ||
           type == "SS";
}

// Trim whitespace from the right side
std::string rtrim(std::string s)
{
    const std::size_t end = s.find_last_not_of(" \n\r\t");

    if (end == std::string::npos) {
        return "";
    }

    s.erase(end + 1);
    return s;
}
