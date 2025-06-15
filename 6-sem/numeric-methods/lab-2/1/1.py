import numpy as np


def f(x):
    return x**3 + x**2 - 2*x - 1


def df(x):
    return 3*(x**2) + 2*x - 2


def ddf(x):
    return 6*x + 2


def phi_2(x):
    return (x**3 + x**2 - 1) / 2


def d_phi_2(x):
    return 1.5*(x**2) + x


def phi_4(x):
    return np.cbrt(1 + 2*x - x**2)


def d_phi_4(x):
    under = 1 + 2*x - x**2
    return (2 - 2*x) / (3 * np.cbrt(under**2))


def check_phi_first_derivative(a, b, eps, d_phi):
    max_q = 0
    x_vals = np.arange(a, b, eps * 2)

    for x in x_vals:
        q = abs(d_phi(x))
        if q - 1 > eps:
            print("ERROR: Условие сходимости нарушено: |phi'(x)| >= 1 для некоторого x")
            exit(0)

        if q > max_q:
            max_q = q

    print(f"Максимальное значение |phi'(x)| на интервале [{a}, {b}] = {max_q}")
    return max_q


def simple_iteration_method(eps, left, right, phi, d_phi):
    x = (left + right) / 2
    counter = 0
    q = check_phi_first_derivative(left, right, eps, d_phi)

    while True:
        # q = abs(d_phi(x))
        # if q >= 1:
        #     print("ERROR: Условие сходимости не выполнено: |phi'(x)| >= 1")
        #     exit(0)

        prev_x = x
        x = phi(x)
        counter += 1

        if counter > 1000:
            print(f"ERROR: Method is diverged for interval between {left} and {right}")
            exit(0)

        if (q / (1 - q)) * abs(x - prev_x) <= eps:
            break

    print(f"Простые Итерации на [{left}, {right}] — количество итераций: ", counter)
    return x


def newton_method(eps, left, right, func, first_d, second_d):
    f_left = func(left)
    f_double_left = second_d(left)
    f_right = func(right)
    f_double_right = second_d(right)

    if f_left * f_double_left > 0:
        x = left
    elif f_right * f_double_right > 0:
        x = right
    else:
        print("WARN: Метод может не сходиться")
        x = (left + right) / 2

    counter = 0
    while True:
        f_x = func(x)
        d_f_x = first_d(x)

        if abs(d_f_x) < eps:
            print("ERROR: Деление на 0!")
            exit(0)

        prev_x = x
        x = x - f_x / d_f_x
        counter += 1

        if abs(x - prev_x) <= eps:
            break

    print(f"Метод Ньютона на [{left}, {right}] — количество итераций: ", counter)

    return x


def check_result(eps, func, x):
    eps = 0.001
    return abs(func(x)) < eps


if __name__ == '__main__':
    eps = float(input("Введите точность: "))

    print("Simple Iterations Method\n")

    check_phi_first_derivative(-2, -1.8, eps, d_phi_4)
    x_1 = simple_iteration_method(eps, -2, -1.8, phi_4, d_phi_4)
    print(f"x_1 = {x_1}, is correct: {check_result(eps, f, x_1)}\n")

    check_phi_first_derivative(-0.5, -0.2, eps, d_phi_2)
    x_2 = simple_iteration_method(eps, -0.5, -0.2, phi_2, d_phi_2)
    print(f"x_2 = {x_2}, is correct: {check_result(eps, f, x_2)}\n")

    check_phi_first_derivative(1, 1.5, eps, d_phi_4)
    x_3 = simple_iteration_method(eps, 1, 1.5, phi_4, d_phi_4)
    print(f"x_3 = {x_3}, is correct: {check_result(eps, f, x_3)}\n")

    print("-------------------------------------------------------------------\n")

    print("Newton's Method\n")

    x_1 = newton_method(eps, -2, -1.8, f, df, ddf)
    print(f"x_1 = {x_1}, is correct: {check_result(eps, f, x_1)}\n")

    x_2 = newton_method(eps, -0.5, -0.2, f, df, ddf)
    print(f"x_2 = {x_2}, is correct: {check_result(eps, f, x_2)}\n")

    x_3 = newton_method(eps, 1, 1.5, f, df, ddf)
    print(f"x_3 = {x_3}, is correct: {check_result(eps, f, x_3)}\n")

