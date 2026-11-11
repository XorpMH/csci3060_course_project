class User:
    """
    Represents a user account in the system.
    """

    def __init__(self, username, user_type, credit=0.0):
        self.username = username
        self.user_type = user_type
        self.credit = credit

    def get_username(self):
        return self.username
    def get_user_type(self):
        return self.user_type
    def get_credit(self):
        return self.credit
    def add_credit(self, amount):
        if amount < 0:
            return False
        self.credit += amount
        return True

    def deduct_credit(self, amount):
        if amount < 0:
            return False
        if self.credit < amount:
            return False

        self.credit -= amount
        return True

    def __str__(self):
        return (
            f"Username: {self.username}, "
            f"Type: {self.user_type}, "
            f"Credit: ${self.credit:.2f}"
        )
    def __str__(self):
        return(
            f"username: {self.username}"
        )