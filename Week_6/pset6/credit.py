# validate credit card using luhn algorithm
from functools import reduce

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
        return -1


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


def sum_every_other(digits_list):
    """return sum of every other digits in the `list` multiplied by 2"""
    every_other_list = [2 * x for x in digits_list[-2::-2]]
    plained_list = list(
        map(lambda x: [x] if x < 10 else convert_to_list(x),  every_other_list))
    result = reduce(lambda x, acc: x + acc, plained_list, [])
    return sum(result)


def convert_to_list(number):
    """helper convert digits in the number to list of digits"""
    return [int(x) for x in str(number)]


def get_sum_every_first(digits_list):
    """return sum of every first digits starting from the end"""
    return sum(digits_list[-1::-2])


def check_valid_card(number):
    """return `true` if if the total modulo 10 is congruent to 0 """
    return True if number % 10 == 0 else False


def main():

    # get correct credit card number from the user input
    card_number = None
    while card_number == None:
        card_number = input_credit_card()
    
    if card_number == -1:
        print("INVALID")
        return


    # validate card
    digits_list = convert_to_list(card_number)
    valid = check_valid_card(sum_every_other(digits_list) + get_sum_every_first(digits_list))
    if not valid:
        print("INVALID")
        return

    # get 2 fist digits of the card number
    first_two_digit = get_first_two_digit(card_number)
    
    # print card type
    print(get_card_type(first_two_digit))


if __name__ == "__main__":
    main()
