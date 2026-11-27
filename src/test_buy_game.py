from src.backend import buyGame
from src.user import User
from src.game import Game
from src.shared_tests import create_test_users, create_test_game


def test_buy_game_success():
    """
    A valid buyer should be able to purchase an available game.
    """

    buyer, seller, _, _, _ = create_test_users()

    users = [
        buyer,
        seller
    ]

    game = create_test_game()

    games = [
        game
    ]

    result = buyGame(
        buyer,
        users,
        games,
        "Minecraft"
    )

    assert result is True

    # Buyer starts with 100 and pays 25
    assert buyer.get_credit() == 75.00

    # Seller starts with 50 and receives 25
    assert seller.get_credit() == 75.00

    # Purchased game should no longer be available
    assert game not in games


def test_buy_game_not_enough_credit():
    """
    A user cannot purchase a game if they do not have enough credit.
    """

    buyer = User(
        "buyeruser",
        "FS",
        10.00
    )

    seller = User(
        "selleruser",
        "FS",
        50.00
    )

    users = [
        buyer,
        seller
    ]

    game = Game(
        "Minecraft",
        "selleruser",
        25.00
    )

    games = [
        game
    ]

    result = buyGame(
        buyer,
        users,
        games,
        "Minecraft"
    )

    assert result is False
    assert buyer.get_credit() == 10.00
    assert seller.get_credit() == 50.00
    assert game in games


def test_buy_game_missing_game():
    """
    A user cannot buy a game that does not exist.
    """

    buyer, seller, _, _, _ = create_test_users()

    users = [
        buyer,
        seller
    ]

    games = []

    result = buyGame(
        buyer,
        users,
        games,
        "Minecraft"
    )

    assert result is False


def test_buy_game_own_game():
    """
    A seller cannot purchase their own game.
    """

    seller = User(
        "selleruser",
        "FS",
        100.00
    )

    users = [
        seller
    ]

    game = Game(
        "Minecraft",
        "selleruser",
        25.00
    )

    games = [
        game
    ]

    result = buyGame(
        seller,
        users,
        games,
        "Minecraft"
    )

    assert result is False
    assert game in games


def test_buy_game_account_manager():
    """
    AM users are not allowed to buy games.
    """

    _, seller, _, account_manager, _ = create_test_users()

    users = [
        account_manager,
        seller
    ]

    game = create_test_game()

    games = [
        game
    ]

    result = buyGame(
        account_manager,
        users,
        games,
        "Minecraft"
    )

    assert result is False
    assert game in games


def test_buy_game_sell_standard():
    """
    SS users are not allowed to buy games.
    """

    _, seller, _, _, sell_standard = create_test_users()

    users = [
        sell_standard,
        seller
    ]

    game = create_test_game()

    games = [
        game
    ]

    result = buyGame(
        sell_standard,
        users,
        games,
        "Minecraft"
    )

    assert result is False
    assert game in games


def test_buy_game_missing_seller():
    """
    A game cannot be purchased if its seller account does not exist.
    """

    buyer = User(
        "buyeruser",
        "FS",
        100.00
    )

    users = [
        buyer
    ]

    game = Game(
        "Minecraft",
        "missinguser",
        25.00
    )

    games = [
        game
    ]

    result = buyGame(
        buyer,
        users,
        games,
        "Minecraft"
    )

    assert result is False
    assert buyer.get_credit() == 100.00
    assert game in games

  