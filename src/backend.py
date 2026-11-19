from src.user import User
from src.game import Game


# --------------------------------------------------
# Helper functions
# --------------------------------------------------

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


# --------------------------------------------------
# Transaction parsers
# --------------------------------------------------

def parseUserTransaction(line):
    """
    Parses transaction types 00, 01, 02, and 06.

    FrontEndC format:

    CC UUUUUUUUUUUUUUU TT CCCCCCCCC

    Code:      2 characters
    Username: 15 characters
    User type: 2 characters
    Credit:    9 characters
    """

    if len(line) < 31:
        return None

    try:
        code = line[0:2]
        username = line[3:18].rstrip()
        user_type = line[19:21].rstrip()
        credit = float(line[22:31])

        return {
            "code": code,
            "username": username,
            "user_type": user_type,
            "credit": credit
        }

    except ValueError:
        return None


def parseRefundTransaction(line):
    """
    Parses a refund transaction.

    FrontEndC format:

    05 BUYER__________ SELLER_________ CCCCCCCCC
    """

    if len(line) < 44:
        return None

    try:
        buyer = line[3:18].rstrip()
        seller = line[19:34].rstrip()
        credit = float(line[35:44])

        return {
            "code": "05",
            "buyer": buyer,
            "seller": seller,
            "credit": credit
        }

    except ValueError:
        return None


def parseSellTransaction(line):
    """
    Parses a sell transaction.

    FrontEndC format:

    03 GAME______________________ SELLER_________ PPPPPP

    Game name: 26 characters
    Seller:    15 characters
    Price:      6 characters
    """

    if len(line) < 52:
        return None

    try:
        game_name = line[3:29].rstrip()
        seller = line[30:45].rstrip()
        price = float(line[46:52])

        return {
            "code": "03",
            "game": game_name,
            "seller": seller,
            "price": price
        }

    except ValueError:
        return None


def parseBuyTransaction(line):
    """
    Parses a buy transaction.

    FrontEndC format:

    04 GAME______________________ SELLER_________
       BUYER__________ PPPPPP

    Game name: 26 characters
    Seller:    15 characters
    Buyer:     15 characters
    Price:      6 characters
    """

    if len(line) < 68:
        return None

    try:
        game_name = line[3:29].rstrip()
        seller = line[30:45].rstrip()
        buyer = line[46:61].rstrip()
        price = float(line[62:68])

        return {
            "code": "04",
            "game": game_name,
            "seller": seller,
            "buyer": buyer,
            "price": price
        }

    except ValueError:
        return None


def parseTransaction(line):
    """
    Determines the transaction type and calls
    the correct parser.
    """

    if len(line) < 2:
        return None

    code = line[0:2]

    if code in ["00", "01", "02", "06"]:
        return parseUserTransaction(line)

    if code == "03":
        return parseSellTransaction(line)

    if code == "04":
        return parseBuyTransaction(line)

    if code == "05":
        return parseRefundTransaction(line)

    return None


# --------------------------------------------------
# Account commands
# --------------------------------------------------

def createUser(current_user, users, username, user_type):
    """
    Creates a new user account.

    AA can create all account types.

    AM has administrative privileges but cannot
    create an AA account.
    """

    if current_user is None:
        return False

    current_type = current_user.get_user_type()

    if current_type not in ["AA", "AM"]:
        return False

    if not validUserType(user_type):
        return False

    # Account Manager cannot create an admin
    if current_type == "AM" and user_type == "AA":
        return False

    if username == "":
        return False

    if len(username) > 15:
        return False

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

    AA can delete accounts.

    AM can delete accounts except AA accounts.
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

    # Do not delete your own account
    if (
        target_user.get_username()
        == current_user.get_username()
    ):
        return False

    # AM cannot delete admin accounts
    if (
        current_type == "AM"
        and target_user.get_user_type() == "AA"
    ):
        return False

    users.remove(target_user)

    return True


def addCredit(current_user, users, username, amount):
    """
    Adds credit to a user account.

    AA and AM have administrative privileges
    for adding credit.
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


# --------------------------------------------------
# Game commands
# --------------------------------------------------

def sellGame(current_user, games, game_name, price):
    """
    Places a game up for sale.

    AM users cannot sell games.
    BS users cannot sell games.
    """

    if current_user is None:
        return False

    user_type = current_user.get_user_type()

    # AM cannot sell
    if user_type == "AM":
        return False

    # Buy-standard cannot sell
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

    if findGame(games, game_name) is not None:
        return False

    new_game = Game(
        game_name,
        current_user.get_username(),
        price
    )

    games.append(new_game)

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

    # Sell-standard cannot buy
    if user_type == "SS":
        return False

    game = findGame(
        games,
        game_name
    )

    if game is None:
        return False

    # A seller should not buy their own game
    if (
        game.get_seller()
        == current_user.get_username()
    ):
        return False

    price = game.get_price()

    if current_user.get_credit() < price:
        return False

    seller = findUser(
        users,
        game.get_seller()
    )

    if seller is None:
        return False

    if not current_user.deduct_credit(price):
        return False

    seller.add_credit(price)

    # Remove game from available games after purchase
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


# --------------------------------------------------
# Transaction application
# --------------------------------------------------

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

    # 00 - Logout / end session
    if code == "00":
        return True

    # 01 - Create
    if code == "01":
        return createUser(
            current_user,
            users,
            data["username"],
            data["user_type"]
        )

    # 02 - Delete
    if code == "02":
        return deleteUser(
            current_user,
            users,
            data["username"]
        )

    # 03 - Sell
    if code == "03":
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

    # 04 - Buy
    if code == "04":
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

    # 05 - Refund
    if code == "05":
        return refundGame(
            users,
            data["buyer"],
            data["seller"],
            data["credit"]
        )

    # 06 - Add credit
    if code == "06":
        return addCredit(
            current_user,
            users,
            data["username"],
            data["credit"]
        )

    return False