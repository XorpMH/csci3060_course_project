class Game:
    """
    Represents a game in the system.
    """

    def __init__(self, name, seller, price):
        self.name = name
        self.seller = seller
        self.price = price

    def get_name(self):
        return self.name
    def get_seller(self):
        return self.seller
    def get_price(self):
        return self.price
    def set_name(self, name):
        self.name = name
    def set_seller(self, seller):
        self.seller = seller
    def set_price(self, price):
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