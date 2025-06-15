import nltk
import os

from utils.compare import compare

from utils.generate import generate_random_letters_text
from utils.generate import generate_random_words_text


nltk.download('wordnet')

GEORGE_ORWELL_1984 = 'texts/1984.txt'
YEVGENY_ZAMYATIN_WE = 'texts/We.txt'


def meaningful_texts(n: int) -> None:
    percentage_of_match = compare(GEORGE_ORWELL_1984, YEVGENY_ZAMYATIN_WE, n)

    print(
        'Two meaningful texts'.ljust(50),
        f'Length: {n}'.ljust(20),
        f'Percentage of match: {percentage_of_match:.5f}',
        end='\n\n'
    )


def meaningful_and_random_letters_texts(n: int) -> None:
    random_letters_text = 'random_letters_text'
    generate_random_letters_text(random_letters_text, n)

    percentage_of_match = compare(GEORGE_ORWELL_1984, random_letters_text, n)

    print(
        'Meaningful and Random Letters texts'.ljust(50),
        f'Length: {n}'.ljust(20),
        f'Percentage of match: {percentage_of_match:.5f}',
        end='\n\n'
    )

    os.remove(random_letters_text)


def meaningful_and_random_words_texts(n: int) -> None:
    random_words_text = 'random_words_text'
    generate_random_words_text(random_words_text, n)

    percentage_of_match = compare(GEORGE_ORWELL_1984, random_words_text, n)

    print(
        'Meaningful and Random Words texts'.ljust(50),
        f'Length: {n}'.ljust(20),
        f'Percentage of match: {percentage_of_match:.5f}',
        end='\n\n'
    )

    os.remove(random_words_text)


def random_letters_texts(n: int) -> None:
    random_letters_text_1 = 'random_letters_text_1'
    generate_random_letters_text(random_letters_text_1, n)

    random_letters_text_2 = 'random_letters_text_2'
    generate_random_letters_text(random_letters_text_2, n)

    percentage_of_match = compare(random_letters_text_1, random_letters_text_2, n)

    print(
        'Two random letters texts'.ljust(50),
        f'Length: {n}'.ljust(20),
        f'Percentage of match: {percentage_of_match:.5f}',
        end='\n\n'
    )

    os.remove(random_letters_text_1)
    os.remove(random_letters_text_2)


def random_words_texts(n: int) -> None:
    random_words_text_1 = 'random_words_text_1'
    generate_random_words_text(random_words_text_1, n)

    random_words_text_2 = 'random_words_text_2'
    generate_random_words_text(random_words_text_2, n)

    percentage_of_match = compare(random_words_text_1, random_words_text_2, n)

    print(
        'Two random words texts'.ljust(50),
        f'Length: {n}'.ljust(20),
        f'Percentage of match: {percentage_of_match:.5f}',
        end='\n\n'
    )

    os.remove(random_words_text_1)
    os.remove(random_words_text_2)


if __name__ == '__main__':
    text_lengths = [1000, 5000, 10000, 15000, 20000]

    for text_length in text_lengths:
        meaningful_texts(text_length)
        meaningful_and_random_letters_texts(text_length)
        meaningful_and_random_words_texts(text_length)
        random_letters_texts(text_length)
        random_words_texts(text_length)

