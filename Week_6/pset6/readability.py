from collections import Counter


def get_text():
    """get text from users input"""
    text = ""
    while text == "":
        text = input("Text: ")

    return text


def count_letters(text):
    """return counts of letters in the text"""

    text_lst = list(filter(lambda x: True if 65 <= ord(x) <= 90 or 97 <= ord(x) <= 122 else False, text))

    return len(text_lst)


def count_words(text):
    """return number of words in the text"""

    return len(text.split())


def count_sentences(text):
    """return number of sentetces in the text"""

    sent = Counter({'.': 0, '?': 0, '!': 0})
    sent.update(text)

    return sent['.'] + sent['?'] + sent['!']


def count_averages(letters, words, sentences):
    """return average letters per 100 words and sentences per 100 words"""

    return (round(letters * 100 / words, 2), round(sentences * 100 / words, 2))


def calculate_index(L, S):
    """calculate and return readability index"""

    return round(0.0588 * L - 0.296 * S - 15.8)

def main():
    """computes the approximate grade level needed to comprehend some text"""

    # input text
    text = get_text()

    # count letters
    letters = count_letters(text)

    # count words
    words = count_words(text)

    # count sentenses
    sentences = count_sentences(text)

    # count averages
    L, S = count_averages(letters, words, sentences)

    # calculate readability index
    index = round(0.0588 * L - 0.296 * S - 15.8)

    # print grade
    if  1 < index < 16:
        print(f"Grade {index}")
    elif index < 1:
        print("Before Grade 1")
    elif index >= 16:
        print("Grade 16+")


if __name__ == "__main__":
    main()