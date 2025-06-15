def compare(
        filename1: str,
        filename2: str,
        n: int) -> float:
    with open(filename1, 'r', encoding='utf-8') as file1, open(filename2, 'r', encoding='utf-8') as file2:
        text1 = file1.read()
        text2 = file2.read()

        text1 = text1[:min(len(text1), len(text2), n)]
        text2 = text2[:min(len(text1), len(text2), n)]

        counter = 0

        for i in range(min(len(text1), len(text2))):
            if text1[i] == text2[i] and not text1[i].isspace():
                counter += 1

        return counter / len(text1)