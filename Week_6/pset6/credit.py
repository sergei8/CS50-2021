# validate credit card using luhn algorithm

MAX_CARD_NUMBER = 9999_9999_9999_9999_99
MIN_CARD_NUMBER = 1000_0000_0000_0


def input_credit_card():
    """get card number from user input in the appropriate format"""
    
    try:
        number = int(input("Number: "))
    except ValueError:
        return None

    if MIN_CARD_NUMBER <= number <= MAX_CARD_NUMBER:
        return number
    else:
        return None


def get_first_two_digit(card_number):
    """extract first 2 digits from card number"""
    
    return int(str(card_number)[:2])


def get_card_type(first_two_digit):
    """return card type based on firs 2 digits"""
    
    if first_two_digit == 34 or first_two_digit == 37:
        return "AMEX"

    if 51 <= first_two_digit <= 55:
        return "MASTERCARD"

    if 40 <= first_two_digit <= 49:
        return "VISA"

    return "INVALID"


def main():
    
    # get correct credit card number from the user input
    card_number = None
    while card_number == None:
        card_number = input_credit_card()

    # get 2 fist digits of the card number
    first_two_digit = get_first_two_digit(card_number)

    # get card type
    card_type = get_card_type(first_two_digit)


if __name__ == "__main__":
    main()
