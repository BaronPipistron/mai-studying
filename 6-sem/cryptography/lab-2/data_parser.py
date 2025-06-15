import re


def parse_numbers(input_filename="data.txt"):
    with open(input_filename, "r") as f:
        text = f.read()

    text = text.replace("\f", "\n")
    pattern = re.compile(
        r'a\[(\d+)\]\s*=\s*([0-9\s]+?)\s*b\[\1\]\s*=\s*([0-9\s]+)',
        re.DOTALL
    )

    matches = pattern.findall(text)

    a_numbers = []
    b_numbers = []

    for _, a_str, b_str in matches:
        a_num = "".join(a_str.split())
        b_num = "".join(b_str.split())

        a_numbers.append(a_num)
        b_numbers.append(b_num)

    return a_numbers, b_numbers


def split_numbers(a_numbers, b_numbers, a_filename="a.txt", b_filename="b.txt"):
    with open(a_filename, "w") as file_a:
        for num in a_numbers:
            file_a.write(num + "\n")

    with open(b_filename, "w") as file_b:
        for num in b_numbers:
            file_b.write(num + "\n")


if __name__ == "__main__":
    print("INFO: Starts processing numbers")

    try:
        a_list, b_list = parse_numbers()
        split_numbers(a_list, b_list)
    except Exception as error:
        print(f"ERROR: {error}")
        exit(1)

    print("INFO: Processing numbers finished successfully")

