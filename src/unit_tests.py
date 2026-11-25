from src.user import User
from src.backend import addCredit, sellGame  
from src import file_handler

def runTests():
    """
    TODO: ADD DESCRIPTION
    TODO: run all test functions and output results
    """

def testCreateUser():
    """
    TODO: ADD DESCRIPTION
    TODO: Write tests
    """

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