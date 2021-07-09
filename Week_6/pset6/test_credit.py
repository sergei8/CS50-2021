import pytest
import mock
import builtins
from credit import input_credit_card, get_first_two_digit, get_card_type, \
    convert_to_list, sum_every_other, get_sum_every_first, check_valid_card, \
    MAX_CARD_NUMBER, MIN_CARD_NUMBER

def test_input_credit_card():
    with mock.patch.object(builtins, "input", lambda _: "foo"):
        input_credit_card() == None
        
    with mock.patch.object(builtins, "input", lambda _: "111"):
        input_credit_card() == None
    
    with mock.patch.object(builtins, "input", lambda _: "378282246310005"):
        input_credit_card() == 378282246310005
    

def test_get_first_two_digit():
    assert get_first_two_digit(123456) == 12
    
def test_get_card_type():
    assert get_card_type(37) == "AMEX"
    assert get_card_type(52) == "MASTERCARD"
    assert get_card_type(45) == "VISA"
    assert get_card_type(99) == "INVALID"
    
    
def test_convert_to_list():
    assert convert_to_list(123) == [1,2,3]

def test_sum_every_other():
    assert sum_every_other([1, 2, 3]) == 4
    assert sum_every_other([1, 3]) == 2
    assert sum_every_other([0, 0, 2, 1]) == 4
    assert sum_every_other([4, 0, 0, 3, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 4]) == 13
    
def test_get_sum_every_first():
    assert get_sum_every_first([1, 2, 3]) == 4
    assert get_sum_every_first([4, 0, 0, 3, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 4]) == 7

def test_check_valid_card():
    assert check_valid_card(20) == True
    assert check_valid_card(2) == False