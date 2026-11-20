class Game:
    """
    Represents a game in the system.
    """

    def __init__(self, name, seller, price):
        self.name = name
        self.seller = seller
        self.price = price

    def get_name(self):
        """
        Access game name by value.
        """
        return self.name
    def get_seller(self):
        """
        Access seller name by value.
        """
        return self.seller
    def get_price(self):
        """
        Access game price by value.
        """
        return self.price
    def set_name(self, name):
        """
        Change the name of a game.
        """
        self.name = name
    def set_seller(self, seller):
        """
        Change the seller of a game.
        """
        self.seller = seller
    def set_price(self, price):
        """
        Change the price of a game.
        """
        if price < 0:
            return False
        self.price = price
        return True

    def __str__(self):
        return (
            f"Game: {self.name}, "
            f"Seller: {self.seller}, "
            f"Price: ${self.price:.2f}"
        )