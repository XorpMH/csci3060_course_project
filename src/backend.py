from src.user import User
from src.game import Game

# Helper functions

def findUser(users, username):
    """
    Finds a user by username.

    Returns the User object if found.
    Returns None if the user does not exist.
    """

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

    for game in games:
        if game.get_name() == game_name:
            return game

    return None


def validUserType(user_type):
    """
    Checks whether a user type is valid.
    """

    valid_types = [
        "AA",
        "FS",
        "BS",
        "SS",
        "AM"
    ]

    return user_type in valid_types

# Transaction parser


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

    if line is None or len(line) < 2:
        return None

    code = line[0:2]

    try:
        match code:
            case "00" | "01" | "02" | "06":

                if len(line) < 31:
                    return None

                username = line[3:18].rstrip()
                user_type = line[19:21].rstrip()
                credit = float(line[22:31])

                return {
                    "code": code,
                    "username": username,
                    "user_type": user_type,
                    "credit": credit
                }

            case "03":

                if len(line) < 52:
                    return None

                game_name = line[3:29].rstrip()
                seller = line[30:45].rstrip()
                price = float(line[46:52])

                return {
                    "code": code,
                    "game": game_name,
                    "seller": seller,
                    "price": price
                }

            case "04":

                if len(line) < 68:
                    return None

                game_name = line[3:29].rstrip()
                seller = line[30:45].rstrip()
                buyer = line[46:61].rstrip()
                price = float(line[62:68])

                return {
                    "code": code,
                    "game": game_name,
                    "seller": seller,
                    "buyer": buyer,
                    "price": price
                }

            case "05":

                if len(line) < 44:
                    return None

                buyer = line[3:18].rstrip()
                seller = line[19:34].rstrip()
                credit = float(line[35:44])

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

# Account commands

def createUser(current_user, users, username, user_type):
    """
    Creates a new user account.

    AA users can create all valid account types.

    AM users have administrative privileges,
    but cannot create AA accounts.
    """

    if current_user is None:
        return False

    current_type = current_user.get_user_type()

    if current_type not in ["AA", "AM"]:
        return False

    if not validUserType(user_type):
        return False

    # Account Managers cannot create admins
    if current_type == "AM" and user_type == "AA":
        return False

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

    if current_user is None:
        return False

    current_type = current_user.get_user_type()

    if current_type not in ["AA", "AM"]:
        return False

    target_user = findUser(
        users,
        username
    )

    if target_user is None:
        return False

    # User cannot delete themselves
    if (
        target_user.get_username()
        == current_user.get_username()
    ):
        return False

    # Account Manager cannot delete an admin
    if (
        current_type == "AM"
        and target_user.get_user_type() == "AA"
    ):
        return False

    users.remove(target_user)

    return True


def addCredit(current_user, users, username, amount):
    """
    Adds credit to an existing user account.

    AA and AM accounts have administrative
    privileges for this transaction.
    """

    if current_user is None:
        return False

    if current_user.get_user_type() not in [
        "AA",
        "AM"
    ]:
        return False

    if amount <= 0:
        return False

    target_user = findUser(
        users,
        username
    )

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

    if current_user is None:
        return False

    user_type = current_user.get_user_type()

    # Account Manager cannot sell
    if user_type == "AM":
        return False

    # Buy-standard users cannot sell
    if user_type == "BS":
        return False

    if game_name == "":
        return False

    # FrontEndC uses a 26-character game field
    if len(game_name) > 26:
        return False

    if price < 0:
        return False

    if price > 999.99:
        return False

    # Game name must be unique
    if findGame(games, game_name) is not None:
        return False

    games.append(
        Game(
            game_name,
            current_user.get_username(),
            price
        )
    )

    return True


def buyGame(current_user, users, games, game_name):
    """
    Purchases an available game.

    AM users cannot buy games.
    SS users cannot buy games.
    """

    if current_user is None:
        return False

    user_type = current_user.get_user_type()

    # Account Manager cannot buy
    if user_type == "AM":
        return False

    # Sell-standard users cannot buy
    if user_type == "SS":
        return False

    game = findGame(
        games,
        game_name
    )

    if game is None:
        return False

    # Seller cannot buy their own game
    if (
        game.get_seller()
        == current_user.get_username()
    ):
        return False

    price = game.get_price()

    # Buyer must have enough credit
    if current_user.get_credit() < price:
        return False

    seller = findUser(
        users,
        game.get_seller()
    )

    if seller is None:
        return False

    # Remove money from buyer
    if not current_user.deduct_credit(price):
        return False

    # Give money to seller
    seller.add_credit(price)

    # Remove purchased game from available games
    games.remove(game)

    return True


def refundGame(users, buyer_name, seller_name, amount):
    """
    Refunds money from the seller to the buyer.
    """

    if amount <= 0:
        return False

    buyer = findUser(
        users,
        buyer_name
    )

    seller = findUser(
        users,
        seller_name
    )

    if buyer is None:
        return False

    if seller is None:
        return False

    if seller.get_credit() < amount:
        return False

    if not seller.deduct_credit(amount):
        return False

    buyer.add_credit(amount)

    return True

# Transaction application

def applyTransaction(
    transaction,
    current_user,
    users,
    games
):
    """
    Applies one transaction produced by FrontEndC.

    Returns True if the transaction succeeds.
    Returns False if the transaction is invalid
    or cannot be completed.
    """

    data = parseTransaction(transaction)

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
                data["game"]
            )

        # Refund
        case "05":
            return refundGame(
                users,
                data["buyer"],
                data["seller"],
                data["credit"]
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