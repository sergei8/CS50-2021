import pytest

from readability import count_letters, count_words, count_sentences, count_averages, calculate_index

@pytest.fixture
def long_text():
    long_text = "Harry Potter was a highly unusual boy in many ways. For one thing, he hated the summer holidays more than any other time of year. For another, he really wanted to do his homework, but was forced to do it in secret, in the dead of the night. And he also happened to be a wizard."
    return long_text

def test_count_letters(long_text):
    result = count_letters("abcda 123!")
    assert isinstance(result, int) == True
    assert result == 5
    result = count_letters(long_text)
    assert result == 214

def test_count_words(long_text):
    result = count_words("abcda 123!")
    assert isinstance(result, int) == True
    assert result == 2
    result = count_words(long_text)
    assert result == 56
    
def test_count_sentences(long_text):
    result = count_sentences("abcda 123!")
    assert isinstance(result, int) == True
    assert result == 1
    result = count_sentences(long_text)
    assert result == 4

def test_count_averages():
    result = count_averages(214, 56, 4)
    assert result == (382.14, 7.14)
    
def test_calculate_index():
    result = calculate_index(382.14, 7.14)
    assert result == 5