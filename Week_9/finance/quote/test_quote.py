from quote import get_quote
import pytest
from mock import patch, Mock

@pytest.fixture
def mock_lookup_result():
    return {
        "name": "NetFlix",
        "price": 547.45,
        "symbol": "NFLX",
    }

def test_qood_quote(mock_lookup_result):
    """it should return latestPrice value of NFLX"""
    with patch('quote.lookup') as mock_lookup:
        mock_lookup.return_value = mock_lookup_result
        result = get_quote("NFLX".lower())
        assert result == 547.45

def test_bed_quote(mock_lookup_result):
    """it should return -1 value of BAD_SUMBOL"""
    with patch('quote.lookup') as mock_lookup:
        mock_lookup.return_value = None
        result = get_quote("BAD_SUMBOL".lower())
        assert result == -1

    
