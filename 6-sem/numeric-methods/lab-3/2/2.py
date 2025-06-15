from typing import Tuple, Union
import matplotlib.pyplot as plt
import numpy as np


def solve_tridiagonal_system(matrix_A: np.ndarray, matrix_B: np.ndarray) -> np.ndarray:
    """Решает трехдиагональную систему уравнений методом прогонки (Томаса).

    Args:
        matrix_A: Матрица коэффициентов (n x n).
        matrix_B: Вектор правых частей (n x 1).

    Returns:
        Вектор решений.
    """
    n = len(matrix_A)
    augmented_matrix = np.concatenate((matrix_A, matrix_B), axis=1)

    # Инициализация прогоночных коэффициентов
    a = np.zeros(n + 1)  # Нижняя диагональ (a[0] не используется)
    b = np.zeros(n + 1)  # Главная диагональ
    c = np.zeros(n + 1)  # Верхняя диагональ
    d = np.zeros(n + 1)  # Правые части

    # Заполнение коэффициентов
    for row in range(n):
        for col in range(len(augmented_matrix[0])):
            if row == col:
                b[row + 1] = augmented_matrix[row][col]
            elif row - 1 == col:
                a[row + 1] = augmented_matrix[row][col]
            elif row + 1 == col and row + 1 < n:
                c[row + 1] = augmented_matrix[row][col]
            elif col == len(augmented_matrix[0]) - 1:
                d[row + 1] = augmented_matrix[row][col]

    # Прямой ход прогонки
    P = np.zeros(n + 1)
    Q = np.zeros(n + 1)
    for i in range(1, n + 1):
        denominator = b[i] + a[i] * P[i - 1]
        P[i] = -c[i] / denominator
        Q[i] = (d[i] - a[i] * Q[i - 1]) / denominator

    # Обратный ход прогонки
    x = np.zeros(n + 1)
    x[n] = Q[n]
    for i in range(n - 1, 0, -1):
        x[i] = P[i] * x[i + 1] + Q[i]

    return x[1:n + 1]


def compute_cubic_spline_coefficients(
        x_nodes: np.ndarray,
        y_values: np.ndarray,
        verbose: bool = True  # Добавлен флаг для вывода таблицы
) -> Tuple[np.ndarray, np.ndarray, np.ndarray, np.ndarray]:
    """Вычисляет коэффициенты кубического сплайна и выводит таблицу."""
    n = len(x_nodes)
    if n != len(y_values):
        raise ValueError("Количество x и y значений должно совпадать")

    h = np.diff(x_nodes)

    # Построение системы уравнений
    A = np.zeros((n - 2, n - 2))
    B = np.zeros(n - 2)

    for i in range(1, n - 1):
        B[i - 1] = 3 * ((y_values[i + 1] - y_values[i]) / h[i] -
                        (y_values[i] - y_values[i - 1]) / h[i - 1])

    for i in range(n - 2):
        if i > 0:
            A[i, i - 1] = h[i]
        A[i, i] = 2 * (h[i] + h[i + 1])
        if i < n - 3:
            A[i, i + 1] = h[i + 1]

    # Решение системы
    c_internal = solve_tridiagonal_system(A, B.reshape(-1, 1))
    c = np.zeros(n)
    c[1:-1] = c_internal

    # Вычисление коэффициентов
    a = y_values[:-1]
    b = np.zeros(n - 1)
    d = np.zeros(n - 1)

    for i in range(n - 1):
        b[i] = ((y_values[i + 1] - y_values[i]) / h[i] -
                h[i] * (2 * c[i] + c[i + 1]) / 3)
        d[i] = (c[i + 1] - c[i]) / (3 * h[i])

    # Вывод таблицы коэффициентов
    if verbose:
        print("\nТаблица коэффициентов кубического сплайна:")
        print("{:<20} {:<12} {:<12} {:<12} {:<12} {:<12}".format(
            "Интервал", "a_i", "b_i", "c_i", "d_i", "h_i"))
        for i in range(n - 1):
            print("[{:<5.2f}, {:<5.2f}]   {:<12.6f} {:<12.6f} {:<12.6f} {:<12.6f} {:<12.6f}".format(
                x_nodes[i], x_nodes[i + 1], a[i], b[i], c[i], d[i], h[i]))

    return a, b, c[:-1], d


def evaluate_cubic_spline(
        x_nodes: np.ndarray,
        coefficients: Tuple[np.ndarray, np.ndarray, np.ndarray, np.ndarray],
        x_eval: Union[float, np.ndarray]
) -> Union[float, np.ndarray]:
    """Вычисляет значение кубического сплайна в заданных точках.

    Args:
        x_nodes: Узлы интерполяции.
        coefficients: Коэффициенты сплайна (a, b, c, d).
        x_eval: Точка(и) для вычисления.

    Returns:
        Значение(я) сплайна в точках x_eval.
    """
    if type(x_eval) is not np.ndarray:
        if x_eval < min(x_nodes) or x_eval > max(x_nodes):
            print(f"Тестовая точка x = {x_eval} выходит за пределы отрезка [{min(x_nodes)}; {max(x_nodes)}]")
            return
    else:
        pass

    a, b, c, d = coefficients

    if np.isscalar(x_eval):
        i = np.clip(np.searchsorted(x_nodes, x_eval) - 1, 0, len(a) - 1)
        dx = x_eval - x_nodes[i]
        return a[i] + b[i] * dx + c[i] * dx ** 2 + d[i] * dx ** 3
    else:
        result = np.zeros_like(x_eval)
        for idx, x_val in enumerate(x_eval):
            i = np.clip(np.searchsorted(x_nodes, x_val) - 1, 0, len(a) - 1)
            dx = x_val - x_nodes[i]
            result[idx] = a[i] + b[i] * dx + c[i] * dx ** 2 + d[i] * dx ** 3
        return result


def plot_spline_results(
        x_nodes: np.ndarray,
        y_values: np.ndarray,
        coefficients: Tuple[np.ndarray, np.ndarray, np.ndarray, np.ndarray],
        eval_point: float
) -> None:
    """Визуализирует результаты с таблицей коэффициентов."""
    eval_result = evaluate_cubic_spline(x_nodes, coefficients, eval_point)

    x_plot = np.linspace(x_nodes[0], x_nodes[-1], 500)
    y_plot = evaluate_cubic_spline(x_nodes, coefficients, x_plot)

    plt.figure(figsize=(12, 8))

    # Основной график
    plt.plot(x_nodes, y_values, 'go', markersize=8, label='Узлы интерполяции')
    plt.plot(x_plot, y_plot, 'r-', linewidth=2, label='Кубический сплайн')
    plt.plot(eval_point, eval_result, 'bx', markersize=10,
             markeredgewidth=2, label=f'Точка x={eval_point:.2f}')

    # Добавляем таблицу коэффициентов на график
    a, b, c, d = coefficients
    table_data = []
    for i in range(len(a)):
        table_data.append([
            f"[{x_nodes[i]:.2f}, {x_nodes[i + 1]:.2f}]",
            f"{a[i]:.6f}",
            f"{b[i]:.6f}",
            f"{c[i]:.6f}",
            f"{d[i]:.6f}"
        ])

    # Создаем текст для отображения
    table_text = "Коэффициенты сплайна:\n"
    table_text += "{:<15} {:<15} {:<15} {:<15} {:<15}\n".format(
        "Интервал", "a_i", "b_i", "c_i", "d_i")
    for row in table_data:
        table_text += "{:<15} {:<15} {:<15} {:<15} {:<15}\n".format(*row)

    plt.figtext(0.5, 0.01, table_text, ha="center", fontsize=9,
                bbox={"facecolor": "white", "alpha": 0.8, "pad": 5})

    plt.xlabel('x', fontsize=12)
    plt.ylabel('f(x)', fontsize=12)
    plt.title('Интерполяция кубическим сплайном', fontsize=14)
    plt.legend(fontsize=10, loc='upper left')
    plt.grid(True)
    plt.tight_layout()
    plt.show()

x_nodes = np.array([-1, 0, 3, 4])
y_values = np.array([-2, 6, 0, 1])
eval_point = 0.4

spline_coeffs = compute_cubic_spline_coefficients(x_nodes, y_values)

result = evaluate_cubic_spline(x_nodes, spline_coeffs, eval_point)

if result is not None:
    print(f"Значение сплайна в точке x = {eval_point}: {result:.6f}")

    plot_spline_results(x_nodes, y_values, spline_coeffs, eval_point)