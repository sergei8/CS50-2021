import pytest
from register import register
from mock import patch

def test_portfolio():
    ...
    
@pytest.fixture
def mock_register():
    def _register(u_name, psw):
        return (1, "")
    return _register

def test_register(mock_register):
    with patch("application.register", mock_register):
        result = register()
        

    
    