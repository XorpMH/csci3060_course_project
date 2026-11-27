import pytest

from src.backend import buyGame
from src.user import User
from src.game import Game


@pytest.fixture
def buy_data():
    """Creates fresh user, game and collection data for each test."""

    buyer = User("buyeruser", "FS", 100.00)
    seller = User("selleruser", "FS", 50.00)

    users = [buyer, seller]

    game = Game("Minecraft", "selleruser", 25.00)

    games = [game]
    games_collection = []

    return buyer, seller, users, games, games_collection


def test_buy_game_success(buy_data):
    """Tests a successful game purchase."""

    buyer, seller, users, games, games_collection = buy_data

    result = buyGame(
        buyer, users, games, "Minecraft", games_collection
    )

    assert result is True
    assert buyer.get_credit() == 75.00
    assert seller.get_credit() == 75.00

    assert ("Minecraft", "buyeruser") in games_collection

    assert len(games) == 1
    assert games[0].get_name() == "Minecraft"


def test_buy_game_not_enough_credit(buy_data):
    """Tests a buyer with insufficient credit."""

    buyer, seller, users, games, games_collection = buy_data

    buyer.deduct_credit(90.00)

    result = buyGame(
        buyer, users, games, "Minecraft", games_collection
    )

    assert result is False
    assert buyer.get_credit() == 10.00
    assert seller.get_credit() == 50.00
    assert games_collection == []
    assert len(games) == 1


def test_buy_game_missing_game(buy_data):
    """Tests purchasing a game that does not exist."""

    buyer, seller, users, games, games_collection = buy_data

    result = buyGame(
        buyer, users, games, "UnknownGame", games_collection
    )

    assert result is False
    assert buyer.get_credit() == 100.00
    assert seller.get_credit() == 50.00
    assert games_collection == []


def test_buy_game_own_game(buy_data):
    """Tests a seller attempting to purchase their own game."""

    buyer, seller, users, games, games_collection = buy_data

    result = buyGame(
        seller, users, games, "Minecraft", games_collection
    )

    assert result is False
    assert seller.get_credit() == 50.00
    assert games_collection == []
    assert len(games) == 1


@pytest.mark.parametrize("user_type", ["AM", "SS"])
def test_buy_game_invalid_user_type(buy_data, user_type):
    """Tests account types that cannot purchase games."""

    buyer, seller, users, games, games_collection = buy_data

    restricted_user = User("restricteduser", user_type, 100.00)

    users.append(restricted_user)

    result = buyGame(
        restricted_user,
        users,
        games,
        "Minecraft",
        games_collection
    )

    assert result is False
    assert restricted_user.get_credit() == 100.00
    assert seller.get_credit() == 50.00
    assert games_collection == []
    assert len(games) == 1


def test_buy_game_missing_seller(buy_data):
    """Tests purchasing a game whose seller does not exist."""

    buyer, seller, users, games, games_collection = buy_data

    users.remove(seller)

    result = buyGame(
        buyer, users, games, "Minecraft", games_collection
    )

    assert result is False
    assert buyer.get_credit() == 100.00
    assert games_collection == []
    assert len(games) == 1


def test_buy_game_already_owned(buy_data):
    """Tests purchasing a game that the buyer already owns."""

    buyer, seller, users, games, games_collection = buy_data

    games_collection.append(("Minecraft", "buyeruser"))

    result = buyGame(
        buyer, users, games, "Minecraft", games_collection
    )

    assert result is False
    assert buyer.get_credit() == 100.00
    assert seller.get_credit() == 50.00

    assert games_collection == [
        ("Minecraft", "buyeruser")
    ]


def test_buy_game_multiple_buyers(buy_data):
    """Tests multiple users purchasing the same available game."""

    buyer, seller, users, games, games_collection = buy_data

    second_buyer = User("secondbuyer", "FS", 100.00)

    users.append(second_buyer)

    first_result = buyGame(
        buyer, users, games, "Minecraft", games_collection
    )

    second_result = buyGame(
        second_buyer,
        users,
        games,
        "Minecraft",
        games_collection
    )

    assert first_result is True
    assert second_result is True

    assert buyer.get_credit() == 75.00
    assert second_buyer.get_credit() == 75.00
    assert seller.get_credit() == 100.00

    assert ("Minecraft", "buyeruser") in games_collection
    assert ("Minecraft", "secondbuyer") in games_collection

    assert len(games_collection) == 2
    assert len(games) == 1


def test_buy_game_no_current_user(buy_data):
    """Tests purchasing without a current user."""

    buyer, seller, users, games, games_collection = buy_data

    result = buyGame(
        None, users, games, "Minecraft", games_collection
    )

    assert result is False
    assert buyer.get_credit() == 100.00
    assert seller.get_credit() == 50.00
    assert games_collection == []


def test_buy_game_exact_credit(buy_data):
    """Tests a buyer whose credit exactly matches the game price."""

    buyer, seller, users, games, games_collection = buy_data

    buyer.deduct_credit(75.00)

    result = buyGame(
        buyer, users, games, "Minecraft", games_collection
    )

    assert result is True
    assert buyer.get_credit() == 0.00
    assert seller.get_credit() == 75.00
    assert ("Minecraft", "buyeruser") in games_collection
    assert len(games) == 1
