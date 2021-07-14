import builtins
import mock
import pytest
import sys
# from pytest_mock import mocker
from mock import patch

from dna import get_params, read_database, read_dna_file, probe_STR
    
def test_get_params():
    with patch.object(sys, "argv", ["", "simple.csv", "1.txt"]):
        actual = get_params()
        assert actual == ("simple.csv", "1.txt")
    with patch.object(sys, "argv", ["", "simple.csv", ]):
        actual = get_params()
        assert actual == None
        
        
def test_read_database():
    with patch.object(builtins, "open", mock.mock_open(read_data="name,AGATC,AATG,TATC\nAlice,2,8,3\n")):
        actual = read_database("/dev/null")
    assert [{'name': 'Alice', 'AGATC': '2', 'AATG': '8', 'TATC': '3'}] == actual
    assert read_dna_file("dummy") == None


def test_read_dna_file():
    with patch.object(builtins, "open", mock.mock_open(read_data="AAGGTAAGTTTAGAATATAAAAGGTGAGTT")):
        actual = read_dna_file("/dev/null")
    assert "AAGGTAAGTTTAGAATATAAAAGGTGAGTT" == actual
    assert read_dna_file("dummy") == None

def test_probe_STR():
    expected = 3
    actual = probe_STR("AAA", "BAAAAAAAAADDAAAAAAEDAAA")
    assert actual == expected
    
    assert probe_STR("111", "BAAAAAAAAADDAAAAAAEDAAA") == 0
    assert probe_STR("111", "") == 0
     