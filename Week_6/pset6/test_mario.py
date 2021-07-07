import pytest
import mock
import builtins
from mario import input_height

def test_input_height():
    with mock.patch.object(builtins, "input",  lambda _: "2"):
        assert input_height() == 2
