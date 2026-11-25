from src.user import User
from src.backend import createUser, deleteUser, addCredit, sellGame, findUser
from src import file_handler

# pip install -U pytest
# python -m pytest src/unit_tests.py

def testCreateUser_NotLoggedIn():
    """
    Test that if user is not logged in, the transaction rejects
    """
    users = []
    
    result = createUser(None, users, "newuser", "FS")

    assert result is False

def testCreateUser_AdminOrAccountManagerReq():
    """
    Test that only AA and AM account types can create a user
    AA: Admin
    AM: Account Manager
    """
    # Admin
    current_user = User("adminuser", "AA", 0.00)
    users = [current_user]
    
    result = createUser(current_user, users, "newuser", "FS")

    assert result is True
    assert findUser(users, "newuser") is not None

    # Account Manager
    current_user = User("accountmanager", "AM", 0.00)
    users = [current_user]
    
    result = createUser(current_user, users, "newuser", "FS")

    assert result is True
    assert findUser(users, "newuser") is not None

    # Full Standard TODO

    # Buy Standard TODO

    # Sell Standard TODO
    

def testDeleteUser():
    """
    TODO: ADD DESCRIPTION
    TODO: Write tests
    """

def testAddCredit():
    # Test that adding credit requires a logged-in user
    users = []
    
    result = addCredit(None, users, "existinguser", 50)

    assert result is False

    # Test that adding zero credit is rejected.

    admin = User("adminuser", "AA", 500.00)
    target = User("existinguser", "FS", 100.00)

    users = [admin, target]

    result = addCredit(admin, users, "existinguser", 0)

    assert result is False

    # Test that negative credit amounts are rejected.

    admin = User("adminuser", "AA", 500.00)
    target = User("existinguser", "FS", 100.00)

    users = [admin, target]

    result = addCredit(admin, users, "existinguser", -50)

    assert result is False
    assert target.get_credit() == 100.00

    # Test that an admin can successfully add credit.

    admin = User("adminuser", "AA", 500.00)
    target = User("existinguser", "FS", 100.00)

    users = [admin, target]

    result = addCredit(admin, users, "existinguser", 50)

    assert result is True
    assert target.get_credit() == 150.00

        # Test that unauthorized users cannot add credit

    buyer = User("buyeruser", "BS", 100.00)
    target = User("existinguser", "FS", 100.00)

    users = [buyer, target]

    result = addCredit(buyer, users, "existinguser", 50)

    assert result is False
    assert target.get_credit() == 100.00

        # Test that credit cannot be added to a nonexistent user

    admin = User("adminuser", "AA", 500.00)

    users = [admin]

    result = addCredit(admin, users, "missinguser", 50)

    assert result is False
    assert admin.get_credit() == 500.00

        # Test that an account manager can successfully add credit

    manager = User("manageruser", "AM", 500.00)
    target = User("existinguser", "FS", 100.00)

    users = [manager, target]

    result = addCredit(manager, users, "existinguser", 50)

    assert result is True
    assert target.get_credit() == 150.00

def testSellGame():
    """
    TODO: ADD DESCRIPTION
    TODO: Write tests
    """

def testBuyGame():
    """
    TODO: ADD DESCRIPTION
    TODO: Write tests
    """

def testRefundGame():
    """
    TODO: ADD DESCRIPTION
    TODO: Write tests
    """