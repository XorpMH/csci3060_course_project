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
    """Test that adding credit requires a logged-in user"""
    
    users = []
    
    result = addCredit(None, users, "existinguser", 50)

    assert result is False

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