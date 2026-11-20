class User:
    """
    Represents a user account in the system.
    """

    def __init__(self, username, user_type, credit=0.0):
        self.username = username
        self.user_type = user_type
        self.credit = credit

    def get_username(self):
        """
        Access username by value.
        """
        return self.username
    def get_user_type(self):
        """
        Access user type by value.
        """
        return self.user_type
    def get_credit(self):
        """
        Access user credit by value.
        """
        return self.credit

    def add_credit(self, amount):
        """
        Add credit to the user equal to amount (positive numbers and 0 only)

        Returns boolean of success state.
        """
        if amount < 0:
            return False
        self.credit += amount
        return True
    def deduct_credit(self, amount):
        """
        Deduct credit from the user equal to amount (positive numbers and 0 only)

        Returns boolean of success state.
        """
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