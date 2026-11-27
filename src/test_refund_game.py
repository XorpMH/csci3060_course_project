from src.backend import refundGame
from src.user import User
from src.shared_tests import create_test_users


def test_refund_game_success():
    """
    A valid refund should transfer credit from seller to buyer.
    """

    buyer, seller, _, _, _ = create_test_users()

    users = [
        buyer,
        seller
    ]

    result = refundGame(
        users,
        "buyeruser",
        "selleruser",
        25.00
    )

    assert result is True

    # Buyer starts with 100 and receives 25
    assert buyer.get_credit() == 125.00

    # Seller starts with 50 and refunds 25
    assert seller.get_credit() == 25.00


def test_refund_game_missing_buyer():
    """
    Refund should fail if the buyer does not exist.
    """

    seller = User(
        "selleruser",
        "FS",
        50.00
    )

    users = [
        seller
    ]

    result = refundGame(
        users,
        "missingbuyer",
        "selleruser",
        25.00
    )

    assert result is False
    assert seller.get_credit() == 50.00


def test_refund_game_missing_seller():
    """
    Refund should fail if the seller does not exist.
    """

    buyer = User(
        "buyeruser",
        "FS",
        100.00
    )

    users = [
        buyer
    ]

    result = refundGame(
        users,
        "buyeruser",
        "missingseller",
        25.00
    )

    assert result is False
    assert buyer.get_credit() == 100.00


def test_refund_game_zero_amount():
    """
    A refund amount of zero should be rejected.
    """

    buyer, seller, _, _, _ = create_test_users()

    users = [
        buyer,
        seller
    ]

    result = refundGame(
        users,
        "buyeruser",
        "selleruser",
        0.00
    )

    assert result is False

    assert buyer.get_credit() == 100.00
    assert seller.get_credit() == 50.00


def test_refund_game_negative_amount():
    """
    A negative refund amount should be rejected.
    """

    buyer, seller, _, _, _ = create_test_users()

    users = [
        buyer,
        seller
    ]

    result = refundGame(
        users,
        "buyeruser",
        "selleruser",
        -10.00
    )

    assert result is False

    assert buyer.get_credit() == 100.00
    assert seller.get_credit() == 50.00


def test_refund_game_insufficient_seller_credit():
    """
    Refund should fail when the seller does not have enough credit.
    """

    buyer = User(
        "buyeruser",
        "FS",
        100.00
    )

    seller = User(
        "selleruser",
        "FS",
        10.00
    )

    users = [
        buyer,
        seller
    ]

    result = refundGame(
        users,
        "buyeruser",
        "selleruser",
        25.00
    )

    assert result is False

    assert buyer.get_credit() == 100.00
    assert seller.get_credit() == 10.00