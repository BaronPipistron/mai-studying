import math
import random
import sympy


def read_data():
    with open("a.txt", "r") as a_file:
        a_values = a_file.readlines()

    with open("b.txt", "r") as b_file:
        b_values = b_file.readlines()

    a_values = [int(x.strip()) for x in a_values]
    b_values = [int(x.strip()) for x in b_values]

    return a_values, b_values


def pollard_rho(n):
    if n % 2 == 0:
        return 2

    x = random.randint(2, n - 1)
    y = x
    c = random.randint(1, n - 1)
    d = 1

    while d == 1:
        x = (x * x + c) % n
        y = (y * y + c) % n
        y = (y * y + c) % n
        d = math.gcd(abs(x - y), n)

    return d if d != n else None


def factorize(n):
    if n <= 1:
        return []
    if n % 2 == 0:
        return [2] + factorize(n // 2)

    factors = []
    stack = [n]

    while stack:
        num = stack.pop()
        if num == 1:
            continue
        if math.isqrt(num) ** 2 == num:
            factors.append(math.isqrt(num))
            factors.append(math.isqrt(num))
            continue

        divisor = pollard_rho(num)

        if divisor is None or divisor == num:
            factors.append(num)
        else:
            stack.append(divisor)
            stack.append(num // divisor)

    return sorted(factors)


if __name__ == "__main__":
    # Концебалов Олег Сергеевич -> 9f515d98e51437f5f8aa21aeb0b70d881567a5f3aea12d9c5d968d33a71f03b9
    MY_VARIANT = int("b9", 16)
    print(f"Variant number is {MY_VARIANT}")

    a_vals, b_vals = read_data()

    MY_A = a_vals[MY_VARIANT]
    a_factors = factorize(MY_A)
    print(f"Number a factorization: {a_factors}")

    MY_B = b_vals[MY_VARIANT]
    b_factors = []

    for i in range(len(a_vals)):
        gcd_b = math.gcd((b_vals[i]), MY_B)
        if gcd_b != 1 and gcd_b != MY_B:
            if sympy.isprime(gcd_b) and sympy.isprime(MY_B // gcd_b):
                b_factors = sorted([gcd_b, MY_B // gcd_b])
                print(f"GCD найден с b[{i}]")

    print(f"Number b factorization: {b_factors}")