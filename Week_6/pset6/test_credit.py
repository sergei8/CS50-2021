import pytest
import mock
import builtins
from credit import input_credit_card, get_first_two_digit, get_card_type, \
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