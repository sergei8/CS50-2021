from quote import get_quote
import pytest
from mock import patch, Mock

@pytest.fixture
def mock_req_result():
    return {
        "iexAskPrice": 566.37,
        "iexAskSize": 107,
        "iexBidPrice": 547.4,
        "iexBidSize": 101,
        "latestPrice": 547.45,
        "symbol": "NFLX",
    }

def test_qood_quote(mock_req_result):
    """it should return latestPrice value of NFLX"""
    with patch('quote.requests.get') as mock_get:
        mock_get.return_value.json.return_value = mock_req_result
        mock_get.return_value.status_code = 200
        result = get_quote("NFLX".lower())
        assert result == 547.45

def test_bed_quote(mock_req_result):
    """it should return -1 value of BAD_SUMBOL"""
    with patch('quote.requests.get') as mock_get:
        mock_get.return_value.json.return_value = mock_req_result
        mock_get.return_value.status_code = 400
        result = get_quote("BAD_SUMBOL".lower())
        assert result == -1

    
