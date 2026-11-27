import pytest

from src.backend import refundGame
from src.user import User
from src.game import Game


@pytest.fixture
def refund_data():
    """Creates fresh user, game and collection data for each test."""

    buyer = User("buyeruser", "FS", 75.00)
    seller = User("selleruser", "FS", 75.00)

    users = [buyer, seller]

    game = Game("Minecraft", "selleruser", 25.00)

    games = [game]

    games_collection = [
        ("Minecraft", "buyeruser")
    ]

    return buyer, seller, users, games, games_collection


def test_refund_game_success(refund_data):
    """Tests a successful refund and ownership removal."""

    buyer, seller, users, games, games_collection = refund_data

    games_collection.append(("Terraria", "buyeruser"))
    games_collection.append(("Minecraft", "otherbuyer"))

    result = refundGame(
        users,
        "buyeruser",
        "selleruser",
        25.00,
        games,
        games_collection
    )

    assert result is True
    assert buyer.get_credit() == 100.00
    assert seller.get_credit() == 50.00

    assert ("Minecraft", "buyeruser") not in games_collection

    assert ("Terraria", "buyeruser") in games_collection
    assert ("Minecraft", "otherbuyer") in games_collection

    assert len(games_collection) == 2
    assert len(games) == 1


def test_refund_game_missing_buyer(refund_data):
    """Tests a refund when the buyer does not exist."""

    buyer, seller, users, games, games_collection = refund_data

    result = refundGame(
        users,
        "missingbuyer",
        "selleruser",
        25.00,
        games,
        games_collection
    )

    assert result is False
    assert seller.get_credit() == 75.00
    assert ("Minecraft", "buyeruser") in games_collection


def test_refund_game_missing_seller(refund_data):
    """Tests a refund when the seller does not exist."""

    buyer, seller, users, games, games_collection = refund_data

    result = refundGame(
        users,
        "buyeruser",
        "missingseller",
        25.00,
        games,
        games_collection
    )

    assert result is False
    assert buyer.get_credit() == 75.00
    assert ("Minecraft", "buyeruser") in games_collection


@pytest.mark.parametrize("amount", [0.00, -10.00])
def test_refund_game_invalid_amount(refund_data, amount):
    """Tests zero and negative refund amounts."""

    buyer, seller, users, games, games_collection = refund_data

    result = refundGame(
        users,
        "buyeruser",
        "selleruser",
        amount,
        games,
        games_collection
    )

    assert result is False
    assert buyer.get_credit() == 75.00
    assert seller.get_credit() == 75.00
    assert ("Minecraft", "buyeruser") in games_collection


def test_refund_game_insufficient_seller_credit(refund_data):
    """Tests a seller with insufficient credit for a refund."""

    buyer, seller, users, games, games_collection = refund_data

    seller.deduct_credit(65.00)

    result = refundGame(
        users,
        "buyeruser",
        "selleruser",
        25.00,
        games,
        games_collection
    )

    assert result is False
    assert buyer.get_credit() == 75.00
    assert seller.get_credit() == 10.00
    assert ("Minecraft", "buyeruser") in games_collection


def test_refund_game_not_owned(refund_data):
    """Tests refunding a game that the buyer does not own."""

    buyer, seller, users, games, games_collection = refund_data

    games_collection.clear()

    result = refundGame(
        users,
        "buyeruser",
        "selleruser",
        25.00,
        games,
        games_collection
    )

    assert result is False
    assert buyer.get_credit() == 75.00
    assert seller.get_credit() == 75.00
    assert games_collection == []


def test_refund_game_price_mismatch(refund_data):
    """Tests a refund amount that does not match the game price."""

    buyer, seller, users, games, games_collection = refund_data

    result = refundGame(
        users,
        "buyeruser",
        "selleruser",
        20.00,
        games,
        games_collection
    )

    assert result is False
    assert buyer.get_credit() == 75.00
    assert seller.get_credit() == 75.00
    assert ("Minecraft", "buyeruser") in games_collection


def test_refund_game_multiple_matches(refund_data):
    """Tests an ambiguous refund involving games with the same price."""

    buyer, seller, users, games, games_collection = refund_data

    second_game = Game("Terraria", "selleruser", 25.00)

    games.append(second_game)

    games_collection.append(("Terraria", "buyeruser"))

    result = refundGame(
        users,
        "buyeruser",
        "selleruser",
        25.00,
        games,
        games_collection
    )

    assert result is False
    assert buyer.get_credit() == 75.00
    assert seller.get_credit() == 75.00

    assert ("Minecraft", "buyeruser") in games_collection
    assert ("Terraria", "buyeruser") in games_collection
    assert len(games_collection) == 2


def test_refund_game_wrong_seller(refund_data):
    """Tests a refund requested from the wrong seller."""

    buyer, seller, users, games, games_collection = refund_data

    other_seller = User("otherseller", "FS", 100.00)

    users.append(other_seller)

    result = refundGame(
        users,
        "buyeruser",
        "otherseller",
        25.00,
        games,
        games_collection
    )

    assert result is False
    assert buyer.get_credit() == 75.00
    assert seller.get_credit() == 75.00
    assert other_seller.get_credit() == 100.00

    assert ("Minecraft", "buyeruser") in games_collection


def test_refund_game_exact_seller_credit(refund_data):
    """Tests a seller whose credit exactly matches the refund amount."""

    buyer, seller, users, games, games_collection = refund_data

    seller.deduct_credit(50.00)

    result = refundGame(
        users,
        "buyeruser",
        "selleruser",
        25.00,
        games,
        games_collection
    )

    assert result is True
    assert buyer.get_credit() == 100.00
    assert seller.get_credit() == 0.00
    assert ("Minecraft", "buyeruser") not in games_collection


def test_refund_game_same_user(refund_data):
    """Tests a refund where the buyer and seller are the same user."""

    buyer, seller, users, games, games_collection = refund_data

    result = refundGame(
        users,
        "buyeruser",
        "buyeruser",
        25.00,
        games,
        games_collection
    )

    assert result is False
    assert buyer.get_credit() == 75.00
    assert seller.get_credit() == 75.00
    assert ("Minecraft", "buyeruser") in games_collection
