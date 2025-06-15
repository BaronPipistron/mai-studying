import random
import string

from nltk.corpus import wordnet


def generate_random_letters_text(filename: str, n: int) -> None:
    random_letters_text = ''.join(
        random.choice(
            string.ascii_letters
        ) for _ in range(n)
    )

    with open(filename, 'w') as file:
        file.write(random_letters_text)


def generate_random_words_text(filename: str, n: int) -> None:
    words_list = [synset.lemmas()[0].name() for synset in wordnet.all_synsets()]
    random_words_text = ''

    while len(random_words_text) < n:
        random_word = random.choice(words_list)
        random_words_text += random_word + ' '

    random_words_text = random_words_text[:n]

    with open(filename, 'w') as file:
        file.write(random_words_text)

