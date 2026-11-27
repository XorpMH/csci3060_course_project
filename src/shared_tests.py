from src.user import User
from src.game import Game


def create_test_users():
    """
    Creates a standard set of users for backend tests.
    """

    buyer = User("buyeruser", "FS", 100.00)
    seller = User("selleruser", "FS", 50.00)
    admin = User("adminuser", "AA", 500.00)
    account_manager = User("manageruser", "AM", 100.00)
    sell_standard = User("selluser", "SS", 100.00)
    return buyer, seller, admin, account_manager, sell_standard


def create_test_game():
    """
    Creates a standard game for backend tests.
    """

    return Game(
        "Minecraft",
        "selleruser",
        25.00
    )