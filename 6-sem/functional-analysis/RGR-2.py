# k = 9, l = 15, variant = 9

# [a; b] = [-1.7; 1.7]

# f(t) = 12 - t^2
# y(t) = cos(3t)

import numpy as np
import matplotlib.pyplot as plt
import sympy

from sympy import symbols, Eq, Add, Mul, Pow, nsimplify
from sympy.printing import pretty

from random import random


sympy.init_printing(pretty_print=True, wrap_line=False)

def f(t):
    return 12 - t ** 2


def y(t):
    return np.cos(3 * t)


def calculate_integral(values):
    integral_value = 0

    for i in range(1, count_points):
        integral_value += step * (values[i - 1] + values[i]) / 2

    return integral_value


def calculate_scalar_product(xs, ys):
    integral_function_values = xs * ys * fs

    return calculate_integral(integral_function_values)


def calculate_norm(xs):
    return np.sqrt(calculate_scalar_product(xs, xs))


def calculate_values_polynom(polynom):
    values = np.zeros(count_points)

    for i in range(count_points):
        current_degree_t = 1
        for coefficient in polynom:
            values[i] += coefficient * current_degree_t
            current_degree_t *= ts[i]

    return values


def calculate_length_projection_error(projection):
    values = np.copy(ys)

    values -= calculate_values_polynom(projection)

    return calculate_norm(values)


def calculate_projection(projection: list, new_element, current_degree):
    projection.append(0)
    coefficient = calculate_scalar_product(calculate_values_polynom(new_element), ys)

    for coordinate in range(current_degree + 1):
        projection[coordinate] += coefficient * new_element[coordinate]


def solve(epsilon):
    system = []
    current_degree = -1
    projection = []

    while True:
        current_eps = calculate_length_projection_error(projection)

        if current_eps < epsilon:
            break

        current_degree += 1
        new_element = [0] * (current_degree + 1)
        new_element[current_degree] = 1

        for element in system:
            coefficient = calculate_scalar_product(calculate_values_polynom(new_element), calculate_values_polynom(element))
            for coordinate in range(len(element)):
                new_element[coordinate] -= coefficient * element[coordinate]

        norm_new_element = calculate_norm(calculate_values_polynom(new_element))
        for coordinate in range(current_degree + 1):
            new_element[coordinate] /= norm_new_element

        system.append(new_element)
        calculate_projection(projection, new_element, current_degree)

        # plt.plot(
        #     ts,
        #     calculate_values_polynom(projection),
        #     linestyle='-',
        #     color=(random(), random(), random()),
        #     label=f"{current_degree} степень"
        # )

        plt.plot(
            ts,
            calculate_values_polynom(new_element),
            linestyle='-',
            color=(random(), random(), random()),
            label=f"phi_{current_degree}"
        )


    system = [[float(x) for x in sublist] if hasattr(sublist, '__iter__') else float(sublist) for sublist in system]
    projection = [[float(x) for x in sublist] if hasattr(sublist, '__iter__') else float(sublist) for sublist in projection]

    t = symbols('t')
    for i, coefs in enumerate(system):
        terms = []
        for power, c in enumerate(coefs):
            if abs(c) < 1e-10:  # Игнорируем очень маленькие коэффициенты
                continue
            if power == 0:
                terms.append(c)
            elif power == 1:
                terms.append(c * t)
            else:
                terms.append(c * t ** power)

        poly = Add(*terms)
        equation = Eq(symbols(f'phi_{i}'), poly)
        print(pretty(equation, use_unicode=True))

    print('y(t) ≈ ', end='')
    for i in range(len(projection)):
        if i % 2 == 0:
            print(f'({projection[i]} * t ** {i}) ', end='')
        if i < len(projection) - 1:
            print(' + ', end='')


if __name__ == '__main__':
    a = -1.7
    b = 1.7

    count_points = 100000
    eps = float(input('Input epsilon: '))

    ts = np.linspace(a, b, count_points)
    fs = np.array([f(t) for t in ts])
    ys = np.array([y(t) for t in ts])
    step = (b - a) / (count_points - 1)

    plt.plot(ts, ys, linestyle='-.', color='#FF0000', label=f"cos(3x)", linewidth=1, zorder=15)

    solve(eps)

    plt.title(f"Approximation cos(3x) (eps = {eps})")
    plt.legend()

    plt.grid(True)
    plt.show()

