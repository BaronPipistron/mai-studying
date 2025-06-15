import operator
import math
from functools import reduce
from typing import List, Tuple

import numpy as np
import matplotlib.pyplot as plt
import sympy
from sympy import simplify, symbols, lambdify


def original_function(x: float) -> float:
    """Исходная функция для интерполяции"""
    return np.cos(x)


def compute_lagrange_polynomial(
        x_nodes: List[float], y_nodes: List[float], symbolic_x: symbols
) -> "sympy.Expr":
    """Строит многочлен Лагранжа по заданным узлам"""
    n = len(x_nodes)
    if len(y_nodes) != n:
        raise ValueError("Количество X и Y должно совпадать")

    lagrange_terms = []
    for i in range(n):
        term = y_nodes[i]
        for j in range(n):
            if j != i:
                term *= simplify((symbolic_x - x_nodes[j]) / (x_nodes[i] - x_nodes[j]))
        lagrange_terms.append(term)

    poly = simplify(sum(lagrange_terms))
    return poly


def compute_newton_polynomial(
        x_nodes: List[float], y_nodes: List[float], symbolic_x: symbols
) -> "sympy.Expr":
    """Строит многочлен Ньютона с разделенными разностями"""
    n = len(x_nodes)
    if len(y_nodes) != n:
        raise ValueError("Количество X и Y должно совпадать")

    # Таблица разделенных разностей
    divided_diffs = y_nodes.copy()
    coefficients = [divided_diffs[0]]

    for order in range(1, n):
        for j in range(n - order):
            divided_diffs[j] = (divided_diffs[j + 1] - divided_diffs[j]) / (
                    x_nodes[j + order] - x_nodes[j]
            )
        coefficients.append(divided_diffs[0])

    # Построение полинома
    newton_terms = []
    for i, coef in enumerate(coefficients):
        term = coef
        for j in range(i):
            term *= simplify((symbolic_x - x_nodes[j]))
        newton_terms.append(term)

    poly = simplify(sum(newton_terms))
    return poly


def format_polynomial(poly: "sympy.Expr") -> str:
    """Форматирует полином в читаемом виде."""
    from sympy import expand
    return str(expand(poly))


def compute_interpolation_errors(
        x_nodes: List[float], y_nodes: List[float], test_point: float
) -> Tuple[float, float, str, str]:
    """Вычисляет погрешности и возвращает полиномы."""
    symbolic_x = symbols("x", real=True)
    lagrange_poly = compute_lagrange_polynomial(x_nodes, y_nodes, symbolic_x)
    newton_poly = compute_newton_polynomial(x_nodes, y_nodes, symbolic_x)

    true_value = original_function(test_point)
    lagrange_value = float(lagrange_poly.subs(symbolic_x, test_point))
    newton_value = float(newton_poly.subs(symbolic_x, test_point))

    return (
        abs(lagrange_value - true_value),
        abs(newton_value - true_value),
        format_polynomial(lagrange_poly),
        format_polynomial(newton_poly)
    )


def plot_interpolation_comparison(
        x_nodes: List[float],
        y_nodes: List[float],
        test_point: float,
        lagrange_error: float,
        newton_error: float,
        lagrange_poly: str,
        newton_poly: str
) -> None:
    """Визуализация с отображением полиномов."""
    symbolic_x = symbols("x", real=True)
    lagrange_poly_expr = compute_lagrange_polynomial(x_nodes, y_nodes, symbolic_x)
    newton_poly_expr = compute_newton_polynomial(x_nodes, y_nodes, symbolic_x)

    lagrange_func = lambdify(symbolic_x, lagrange_poly_expr, "numpy")
    newton_func = lambdify(symbolic_x, newton_poly_expr, "numpy")

    x_plot = np.linspace(min(x_nodes) - 1, max(x_nodes) + 1, 1000)
    y_true = original_function(x_plot)
    y_lagrange = lagrange_func(x_plot)
    y_newton = newton_func(x_plot)

    plt.figure(figsize=(12, 8))

    # Добавляем текст с полиномами
    plt.figtext(0.5, 0.01,
                f"Полином Лагранжа: {lagrange_poly}\nПолином Ньютона: {newton_poly}",
                ha="center", fontsize=10, bbox={"facecolor": "white", "alpha": 0.8, "pad": 5})

    plt.plot(x_plot, y_true, label="Исходная функция")
    plt.plot(x_plot, y_newton, label="Полином Ньютона")
    plt.plot(x_plot, y_lagrange, "--", label="Полином Лагранжа")
    plt.scatter(x_nodes, y_nodes, marker="o", label="Узлы интерполяции")
    plt.axvline(test_point, linestyle="--", label=f"X* = {test_point}")
    plt.title(f"Погрешность: Лагранжа = {lagrange_error:.2e}, Ньютона = {newton_error:.2e}")
    plt.xlabel("x")
    plt.ylabel("y")
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.show()


def run_interpolation_example(
        x_nodes: List[float], test_point: float, case_name: str
) -> None:
    """Запускает пример интерполяции с выводом полиномов."""
    is_test = True

    if test_point > max(x_nodes):
        print("ERROR: Точка за границами интерполяции")
        exit(1)

    if test_point < min(x_nodes) or test_point > max(x_nodes):
        is_test = False

    y_nodes = [original_function(x) for x in x_nodes]
    print(f"\nСлучай {case_name}: X = {x_nodes}")
    print(f"Y = {[float(node) for node in y_nodes]}")

    error_lagrange, error_newton, lagrange_poly, newton_poly = compute_interpolation_errors(
        x_nodes, y_nodes, test_point
    )

    print("\nРезультаты:")
    print(f"Полином Лагранжа: {lagrange_poly}")
    print(f"Полином Ньютона: {newton_poly}")

    if not is_test:
        print(f"Тестовая точка выходит за пределы отрезка, посчитать погрешность невозможно")
    else:
        print(f"\nПогрешность Лагранжа: {error_lagrange:.6f}")
        print(f"Погрешность Ньютона: {error_newton:.6f}")

    plot_interpolation_comparison(
        x_nodes, y_nodes, test_point,
        error_lagrange, error_newton,
        lagrange_poly, newton_poly
    )


# Базовый случай
# run_interpolation_example(
#     x_nodes=[-0.4, -0.1, 0.2, 0.5], test_point=0.1, case_name="base"
# )

# Больше точек
run_interpolation_example(
    x_nodes=[-0.4, -0.1, 0.2, 0.5, 0.8], test_point=0.1, case_name="many points"
)

# Меньше точек
# run_interpolation_example(
#     x_nodes=[-0.4, -0.1, 0.2], test_point=0.1, case_name="little point"
# )

 # X* = 100
# run_interpolation_example(
#     x_nodes=[-0.4, -0.1, 0.2, 0.5], test_point=100, case_name="X* = 100"
# )

# Хитрый тест
# run_interpolation_example(
#     x_nodes=[-1, 0, 3, 4], test_point=0.1, case_name="cool test"
# )