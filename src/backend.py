"""
Backend handler for process the daily_transaction.txt file.

Access with applyTransactions(), optionally adding custom file paths. 

This program parses all the data files into lists of Games, Users, and game collections (matched user and game names)
It then applies transactions to the parsed data, and re-encodes it back into those files.
All transactions and their output (results) are recorded in the log file.
"""

from src.user import User
from src.game import Game
from src import file_handler
from pathlib import Path

# Default path (data folder outside src)
DEFAULT_PATH = Path(__file__).resolve().parent.parent / "data"

# Helper functions

def findUser(users, username):
    """
    Finds a user by username.

    Returns the User object if found.
    Returns None if the user does not exist.
    """

    # Check list of users for user
    for user in users:
        if user.get_username() == username:
            return user

    return None


def findGame(games, game_name):
    """
    Finds a game by game name.

    Returns the Game object if found.
    Returns None if the game does not exist.
    """

    # Check list of games for game
    for game in games:
        if game.get_name() == game_name:
            return game

    return None


def validUserType(user_type):
    """
    Checks whether a user type is valid.
    """

    # Definition of every user type
    valid_types = [
        "AA",
        "FS",
        "BS",
        "SS",
        "AM"
    ]

    # Compare user type
    return user_type in valid_types

# String parsers

def parseTransaction(line):
    """
    Parses one fixed-width transaction line produced by FrontEndC.

    Transaction types:

    00 - Logout
    01 - Create
    02 - Delete
    03 - Sell
    04 - Buy
    05 - Refund
    06 - Add credit

    Returns a dictionary containing the parsed transaction.
    Returns None if the transaction is invalid.
    """

    # Empty check
    if line is None or len(line) < 2:
        return None

    # Parse transaction type
    code = line[0:2]

    try:
        match code:
            # Logoout, Create, Delete, Add credit
            case "00" | "01" | "02" | "06":
                # Length check
                if len(line) < 31:
                    return None

                # Parse line
                username = line[3:18].rstrip()
                user_type = line[19:21].rstrip()
                credit = float(line[22:31])

                # Return parsed values as a dictionary
                return {
                    "code": code,
                    "username": username,
                    "user_type": user_type,
                    "credit": credit
                }

            # Sell
            case "03":
                # Length check
                if len(line) < 52:
                    return None

                # Parse line
                game_name = line[3:29].rstrip()
                seller = line[30:45].rstrip()
                price = float(line[46:52])

                # Return parsed values as a dictionary
                return {
                    "code": code,
                    "game": game_name,
                    "seller": seller,
                    "price": price
                }

            # Buy
            case "04":
                # Length check
                if len(line) < 68:
                    return None

                # Parse line
                game_name = line[3:29].rstrip()
                seller = line[30:45].rstrip()
                buyer = line[46:61].rstrip()
                price = float(line[62:68])

                # Return parsed values as a dictionary
                return {
                    "code": code,
                    "game": game_name,
                    "seller": seller,
                    "buyer": buyer,
                    "price": price
                }

            # Refund
            case "05":
                # Length check
                if len(line) < 44:
                    return None
                
                # Parse line
                buyer = line[3:18].rstrip()
                seller = line[19:34].rstrip()
                credit = float(line[35:44])

                # Return parsed values as a dictionary
                return {
                    "code": code,
                    "buyer": buyer,
                    "seller": seller,
                    "credit": credit
                }

            # Unknown transaction code
            case _:
                return None

    except ValueError:
        return None

def parseUser(line):
    """
    Parse a user string into a User object.

    From UUUUUUUUUUUUUUU_TT_CCCCCCCCC

    Returns the constructed user or None if invalid.
    """
    # Format check
    if line is None or len(line) < 28:
        return None

    # Parse line
    username = line[0:15].rstrip()
    user_type = line[16:18].rstrip()
    credit = float(line[19:28])

    return User(username, user_type, credit)

def parseGame(line):
    """
    Parse a game string into a Game object from the available_games file.
    
    From IIIIIIIIIIIIIIIIIIIIIIIIII_SSSSSSSSSSSSSSS_PPPPPP

    Returns the constructed game or None if invalid.
    """
    # Format check
    if line is None or len(line) < 49:
        return None

    # Parse line
    game_name = line[0:26].rstrip()
    seller_name = line[27:42].rstrip()
    price = float(line[43:49])

    return Game(game_name, seller_name, price)

def parseCollectedGame(line):
    """
    Parse a game string into a Game object from the games_collection file.

    From IIIIIIIIIIIIIIIIIIIIIIIIII_SSSSSSSSSSSSSSS

    Returns the game name and associated username or None if invalid.
    """
    # Format check
    if line is None or len(line) < 42:
        return None, None

    # Parse line
    game_name = line[0:26].rstrip()
    owner_name = line[27:42].rstrip()

    return game_name, owner_name

# String encoders

def encodeUser(user):
    """
    Encode a User object into string format for the current_users file.

    Into UUUUUUUUUUUUUUU_TT_CCCCCCCCC

    Returns the formatted string.
    """
    # Pad values
    padded_username = str(user.get_username()).ljust(15)[:15]
    padded_user_type = str(user.get_user_type()).ljust(2)[:2]
    padded_credit = f"{user.get_credit():09.2f}"[:9] #:09 pads zeroes on the right and .2f locks to 2 decimal places

    return padded_username + "_" + padded_user_type + "_" + padded_credit

def encodeGame(game):
    """
    Encode a Game object into string format for the available_games file.

    Into IIIIIIIIIIIIIIIIIIIIIIIIII_SSSSSSSSSSSSSSS_PPPPPP
    
    Returns the formatted string.
    """
    # Pad values
    padded_game_name = str(game.get_name()).ljust(26)[:26]
    padded_seller_name = str(game.get_seller()).ljust(15)[:15]
    padded_price = f"{game.get_price():06.2f}"[:6] #:06 pads zeroes on the right and .2f locks to 2 decimal places

    return padded_game_name + "_" + padded_seller_name + "_" + padded_price

def encodeCollectedGame(game_name, owner_name):
    """
    Encode a game name and username into string format for the games_collection file.

    Into IIIIIIIIIIIIIIIIIIIIIIIIII_SSSSSSSSSSSSSSS
        
    Returns the formatted string.
    """
    # Pad values
    padded_game_name = str(game_name).ljust(26)[:26]
    padded_owner_name = str(owner_name).ljust(15)[:15]

    return padded_game_name + "_" + padded_owner_name

# User transactions

def createUser(current_user, users, username, user_type):
    """
    Creates a new user account.

    AA users can create all valid account types.

    AM users have administrative privileges,
    but cannot create AA accounts.
    """

    # Current user must exist
    if current_user is None:
        return False

    current_type = current_user.get_user_type()

    # Confirm user type
    if not validUserType(user_type):
        return False

    # Only admins and account managers can create a user
    if current_type not in ["AA", "AM"]:
        return False

    # Account Managers cannot create admins
    if current_type == "AM" and user_type == "AA":
        return False

    # Username validity
    if username == "":
        return False
    if len(username) > 15:
        return False

    # Username must be unique
    if findUser(users, username) is not None:
        return False

    users.append(
        User(
            username,
            user_type,
            0.0
        )
    )

    return True


def deleteUser(current_user, users, username):
    """
    Deletes an existing user account.

    AA users can delete other accounts.

    AM users can delete accounts,
    but cannot delete AA accounts.
    """

    # Current user must exist
    if current_user is None:
        return False

    current_type = current_user.get_user_type()

    # Only admins and account managers can delete a user
    if current_type not in ["AA", "AM"]:
        return False

    target_user = findUser(users,username)

    # Deletion target must exist
    if target_user is None:
        return False

    # User cannot delete themselves
    if (target_user.get_username() == current_user.get_username()):
        return False

    # Account Manager cannot delete an admin
    if (current_type == "AM" and target_user.get_user_type() == "AA"):
        return False

    users.remove(target_user)

    return True


def addCredit(current_user, users, username, amount):
    """
    Adds credit to an existing user account.

    AA and AM accounts have administrative
    privileges for this transaction.
    """

    # Current user must exist
    if current_user is None:
        return False

    # Only Account managers and admins can grant credit
    if current_user.get_user_type() not in ["AA","AM"]:
        return False

    # Cannot grant negative credit
    if amount <= 0:
        return False

    target_user = findUser(users,username)

    # Target must exist
    if target_user is None:
        return False

    target_user.add_credit(amount)

    return True

# Game commands

def sellGame(current_user, games, game_name, price):
    """
    Places a game up for sale.

    AM users cannot sell games.
    BS users cannot sell games.
    """

    # Current user must exist
    if current_user is None:
        return False

    user_type = current_user.get_user_type()

    # Account Manager cannot sell
    if user_type == "AM":
        return False

    # Buy-standard users cannot sell
    if user_type == "BS":
        return False

    # No game name
    if game_name == "":
        return False

    # FrontEndC uses a 26-character game field
    if len(game_name) > 26:
        return False

    # Price cannot be below zero
    if price < 0:
        return False

    # Price cannot be equal or above 1000
    if price > 999.99:
        return False

    # Game name must be unique
    if findGame(games, game_name) is not None:
        return False

    # Add game if all checks pass
    games.append(
        Game(
            game_name,
            current_user.get_username(),
            price
        )
    )

    return True


def buyGame(current_user, users, games, game_name, games_collection):
    """
    Purchases an available game and updates the buyer's game collection.
    AM and SS users cannot buy games.
    """

    # Current user must exist
    if current_user is None:
        return False

    user_type = current_user.get_user_type()

    # Check buyer permissions
    if user_type in ["AM", "SS"]:
        return False

    game = findGame(games, game_name)

    # Game must exist
    if game is None:
        return False

    buyer_name = current_user.get_username()

    # Seller cannot buy their own game
    if game.get_seller() == buyer_name:
        return False

    # Buyer cannot purchase a game they already own
    ownership = (game_name, buyer_name)

    if ownership in games_collection:
        return False

    price = game.get_price()

    # Buyer must have enough credit
    if current_user.get_credit() < price:
        return False

    seller = findUser(users, game.get_seller())

    # Seller must exist
    if seller is None:
        return False

    # Remove money from buyer
    if not current_user.deduct_credit(price):
        return False

    # Give money to seller
    seller.add_credit(price)

    # Add game to the buyer's collection
    games_collection.append(ownership)

    return True


def refundGame(users, buyer_name, seller_name, amount,
               games, games_collection):
    """
    Refunds credit from the seller to the buyer and removes
    the matching game from the buyer's collection.
    """

    # Refund amount must be positive
    if amount <= 0:
        return False

    buyer = findUser(users, buyer_name)
    seller = findUser(users, seller_name)

    # Both users must exist
    if buyer is None:
        return False

    if seller is None:
        return False

    # Buyer and seller cannot be the same user
    if buyer_name == seller_name:
        return False

    # Seller must have enough credit
    if seller.get_credit() < amount:
        return False

    # Find games matching the seller, price and ownership
    matching_games = []

    for game in games:
        ownership = (game.get_name(), buyer_name)

        if (
            game.get_seller() == seller_name
            and round(game.get_price(), 2) == round(amount, 2)
            and ownership in games_collection
        ):
            matching_games.append(game)

    # Exactly one game must match the refund
    if len(matching_games) != 1:
        return False

    refunded_game = matching_games[0]

    # Deduct credit from seller
    if not seller.deduct_credit(amount):
        return False

    # Return credit to buyer
    buyer.add_credit(amount)

    # Remove the refunded game from the buyer's collection
    games_collection.remove(
        (refunded_game.get_name(), buyer_name)
    )

    return True

# Transaction application

def applyTransactions(
        daily_transactions_filepath=str(DEFAULT_PATH / "daily_transactions.txt"), 
        current_users_filepath=str(DEFAULT_PATH / "current_users.txt"),
        available_games_filepath=str(DEFAULT_PATH / "available_games.txt"),
        games_collection_filepath=str(DEFAULT_PATH / "games_collection.txt"),
        log_filepath=str(DEFAULT_PATH / "log.txt")
):
    """
    Parses all data files, applies all transactions to data, and recompose data files.
    Appends transactions to the log with their runtime information. (Success or failure, etc.) 

    Call this method in the frontend after logout, or on its own afterwards.
    """

    # Read all data files into lists of strings
    transaction_lines = file_handler.readFile(daily_transactions_filepath)
    current_users_lines = file_handler.readFile(current_users_filepath)
    available_games_lines = file_handler.readFile(available_games_filepath)
    games_collection_lines = file_handler.readFile(games_collection_filepath)

    # Requires at least 1 transaction
    if not transaction_lines:
        return 

    # Construct list of current users
    users = []
    for line in current_users_lines:
        new_user = parseUser(line)
        
        if new_user is not None: 
            users.append(new_user)

    # Construct list of available games
    games = []
    for line in available_games_lines:
        new_game = parseGame(line)
        
        if new_game is not None: 
            games.append(new_game)

    # Construct list of games in the current user's collection
    games_collection = []
    for line in games_collection_lines:
        game_name, user_name = parseCollectedGame(line)
        
        if game_name is not None and user_name is not None: 
            games_collection.append((game_name, user_name))

    # Get the current user from the logout transaction
    current_user = None
    logout = parseTransaction(transaction_lines[-1])
    if logout is not None:
        current_user = findUser(users, logout.get("username"))
    
    # Apply each transaction to the parsed data and log it
    for transaction in transaction_lines:
        # Apply transaction
        result = applyTransaction(transaction, current_user, users, games, games_collection)
        
        # Log result
        file_handler.appendFile(log_filepath, str(transaction) + "\n" + str(result) + "\n")

    # Encode updated users list back into a list of strings and write it to the file
    encoded_users = []
    for user in users:
        new_line = encodeUser(user)

        encoded_users.append(new_line)
    file_handler.writeFile(current_users_filepath, encoded_users)

    # Encode updated games list back into a list of strings and write it to the file
    encoded_games = []
    for game in games:
        new_line = encodeGame(game)

        encoded_games.append(new_line)
    file_handler.writeFile(available_games_filepath, encoded_games)

    # Encode updated games_collection list back a list of into strings and write it to the file
    encoded_games_collection = []
    for game_name, user_name in games_collection: 
        new_line = encodeCollectedGame(game_name, user_name)
        
        encoded_games_collection.append(new_line)
    file_handler.writeFile(games_collection_filepath, encoded_games_collection)

    # TODO: Delete daily_transactions file? Need to check if the frontend handles cleanup

def applyTransaction(
    transaction, # Transaction to be applied
    current_user, # User doing the transaction
    users, # 
    games,
    games_collection
):
    """
    Applies one transaction produced by FrontEndC.

    Returns True if the transaction succeeds.
    Returns False if the transaction is invalid
    or cannot be completed.
    """

    # Parse the transaction string into the required information
    data = parseTransaction(transaction)

    # Invalid transactions are parsed as None
    if data is None:
        return False

    code = data["code"]

    match code:

        # Logout / end session
        case "00":
            return True

        # Create user
        case "01":
            return createUser(
                current_user,
                users,
                data["username"],
                data["user_type"]
            )

        # Delete user
        case "02":
            return deleteUser(
                current_user,
                users,
                data["username"]
            )

        # Sell game
        case "03":
            seller = findUser(
                users,
                data["seller"]
            )

            return sellGame(
                seller,
                games,
                data["game"],
                data["price"]
            )

                # Buy game
        case "04":
            buyer = findUser(
                users,
                data["buyer"]
            )

            return buyGame(
                buyer,
                users,
                games,
                data["game"],
                games_collection
            )

        # Refund
        case "05":
            return refundGame(
                users,
                data["buyer"],
                data["seller"],
                data["credit"],
                games,
                games_collection
            )

        # Add credit
        case "06":
            return addCredit(
                current_user,
                users,
                data["username"],
                data["credit"]
            )

        case _:
            return False