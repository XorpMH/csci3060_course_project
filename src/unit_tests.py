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
    assert findUser(users, "newuser") is None

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

    # Full Standard
    current_user = User("normaluser", "FS", 0.00)
    users = [current_user]
    
    result = createUser(current_user, users, "newuser", "FS")

    assert result is False
    assert findUser(users, "newuser") is None

    # Buy Standard
    current_user = User("buyuser", "BS", 0.00)
    users = [current_user]
    
    result = createUser(current_user, users, "newuser", "FS")

    assert result is False
    assert findUser(users, "newuser") is None

    # Sell Standard
    current_user = User("buyuser", "SS", 0.00)
    users = [current_user]
    
    result = createUser(current_user, users, "newuser", "FS")

    assert result is False
    assert findUser(users, "newuser") is None

def testCreateUser_InvalidType():
    """
    Test that user cannot be created with a type that doesn't exist in the system
    """
    current_user = User("accountmanager", "AM", 0.00)
    users = [current_user]
    
    result = createUser(current_user, users, "newuser", "")

    assert result is False
    assert findUser(users, "newuser") is None

def testCreateUser_AMCannotCreateAA():
    """
    Test that an account manager cannot create an admin user (privilege escalation)
    """
    current_user = User("accountmanager", "AM", 0.00)
    users = [current_user]
    
    result = createUser(current_user, users, "newuser", "AA")

    assert result is False
    assert findUser(users, "newuser") is None

def testCreateUser_UsernameLength():
    """
    Test that the username must be between 1 and 15 characters
    """
    # Username over 15 characters
    current_user = User("accountmanager", "AM", 0.00)
    users = [current_user]
    
    result = createUser(current_user, users, "1234567890123456", "FS")

    assert result is False
    assert findUser(users, "newuser") is None

    # Username empty
    current_user = User("accountmanager", "AM", 0.00)
    users = [current_user]
    
    result = createUser(current_user, users, "", "FS")

    assert result is False
    assert findUser(users, "newuser") is None

def testCreateUser_Unique():
    """
    Test that the username of the created user cannot be one that already exists
    """
    current_user = User("accountmanager", "AM", 0.00)
    existing_user = User("existinguser", "FS", 0.00)
    users = [current_user, existing_user]
    
    result = createUser(current_user, users, "existinguser", "FS")

    assert result is False

def testDeleteUser():
    """
    TODO: ADD DESCRIPTION
    TODO: Write tests
    """
    

def testAddCreditLoggedIn():
    # Test that adding credit requires a logged-in user
    users = []
    
    result = addCredit(None, users, "existinguser", 50)

    assert result is False

def testAddCreditZero():
    # Test that adding zero credit is rejected.

    admin = User("adminuser", "AA", 500.00)
    target = User("existinguser", "FS", 100.00)

    users = [admin, target]

    result = addCredit(admin, users, "existinguser", 0)

    assert result is False

def testAddCreditNegative():
    # Test that negative credit amounts are rejected.

    admin = User("adminuser", "AA", 500.00)
    target = User("existinguser", "FS", 100.00)

    users = [admin, target]

    result = addCredit(admin, users, "existinguser", -50)

    assert result is False
    assert target.get_credit() == 100.00

def testAddCreditSuccess():
    # Test that an admin can successfully add credit.

    admin = User("adminuser", "AA", 500.00)
    target = User("existinguser", "FS", 100.00)

    users = [admin, target]

    result = addCredit(admin, users, "existinguser", 50)

    assert result is True
    assert target.get_credit() == 150.00

def testAddCreditUnauthorized():
    # Test that unauthorized users cannot add credit

    buyer = User("buyeruser", "BS", 100.00)
    target = User("existinguser", "FS", 100.00)

    users = [buyer, target]

    result = addCredit(buyer, users, "existinguser", 50)

    assert result is False
    assert target.get_credit() == 100.00

def testAddCreditMissingUser():
    # Test that credit cannot be added to a nonexistent user

    admin = User("adminuser", "AA", 500.00)

    users = [admin]

    result = addCredit(admin, users, "missinguser", 50)

    assert result is False
    assert admin.get_credit() == 500.00

def testAddCreditManager():
    # Test that an account manager can successfully add credit

    manager = User("manageruser", "AM", 500.00)
    target = User("existinguser", "FS", 100.00)

    users = [manager, target]

    result = addCredit(manager, users, "existinguser", 50)

    assert result is True
    assert target.get_credit() == 150.00

def testSellGameLoggedIn():
    # Test that selling a game requires a logged-in user
    games = []

    result = sellGame(None, games, "New Game", 20.00)

    assert result is False
    assert len(games) == 0

def testSellGameAccountManager():
    # Test that account managers cannot sell games
    manager = User("manageruser", "AM", 100.00)
    games = []

    result = sellGame(manager, games, "New Game", 20.00)

    assert result is False
    assert len(games) == 0

def testSellGameBuyStandard():
    # Test that a standard buyer cannot sell games
    buyer = User("buyeruser", "BS", 100.00)
    games = []

    result = sellGame(buyer, games, "New Game", 20.00)

    assert result is False
    assert len(games) == 0

def testSellGameEmptyName():
    # Test that selling a game with an empty name is rejected
    seller = User("selleruser", "FS", 100.00)
    games = []

    result = sellGame(seller, games, "", 20.00)

    assert result is False
    assert len(games) == 0

def testSellGameExceedNameLength():
    # Test that selling a game with a name exceeding 25 characters is rejected
    seller = User("selleruser", "FS", 100.00)
    games = []

    long_name = "A" * 27  # 27 characters
    result = sellGame(seller, games, long_name, 20.00)

    assert result is False
    assert len(games) == 0

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