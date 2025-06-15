import numpy as np
import matplotlib.pyplot as plt


def lagrange_interpolation(x, xi, yi):
    n = len(xi)
    Lx = 0
    # Для каждого узла интерполяции формируем лагранжев многочлен
    for i in range(n):
        li = 1
        for j in range(n):
            if i != j:
                li *= (x - xi[j]) / (xi[i] - xi[j])
        Lx += yi[i] * li

    return Lx


# Поиск подходящего интервала [x_i, x_{i+1}]
def get_index(x_star, x):
    for i in range(len(x) - 2):  # нужно минимум 3 точки: i, i+1, i+2
        if x[i] <= x_star <= x[i + 1]:
            return i

    print("ERROR: x* вне допустимого диапазона для формулы (3.20)")
    exit(0)


if __name__ == "__main__":
    # Базовый случай
    x_star = 1.0
    x = [-1.0, 0.0, 1.0, 2.0, 3.0]
    y = [2.3562, 1.5708, 0.7854, 0.46365, 0.32175]

    # Точка за пределами интервала
    # x_star = 0.4
    # x = [0.0, 0.1, 0.2, 0.3, 0.4]
    # y = [1.0, 1.1052, 1.2214, 1.3499, 1.4918]

    # Неравномерные шаги
    # x_star = 0.5
    # x = [-1.0, 0.3, 1.0, 2.5, 3.7]
    # y = [2.3562, 1.2, 0.7, 0.4, 0.2]

    if x_star < min(x) or x_star > max(x):
        print("ERROR: X* не входит в диапазон значений x")
        exit(0)

    i = get_index(x_star, x)

    dy1 = (y[i + 1] - y[i]) / (x[i + 1] - x[i])
    dy2 = (y[i + 2] - y[i + 1]) / (x[i + 2] - x[i + 1])
    correction = (dy2 - dy1) / (x[i + 2] - x[i]) * (2 * x_star - x[i] - x[i + 1])
    first_derivative = dy1 + correction

    # Вторая производная по формуле (3.21)
    second_derivative = 2 * (dy2 - dy1) / (x[i + 1] - x[i - 1])

    print(f"Первая производная в x* = {x_star}: {first_derivative:.6f}")
    print(f"Вторая производная в x* = {x_star}: {second_derivative:.6f}\n")

    tan = lambda x_star: (lagrange_interpolation(x_star+1e-3, x, y) - lagrange_interpolation(x_star, x, y))/1e-3
    print(f"Первая производная по функции интерполяции = {tan(x_star)}")


    x_vals = np.linspace(-1.2, 3.3, 4000)
    tangent_line = tan(x_star)*(x_vals-x_star) + lagrange_interpolation(x_star, x, y)

    lagrange_vals = [lagrange_interpolation(point, x, y) for point in x_vals]

    # График
    plt.figure(figsize=(10, 10))
    plt.plot(x_vals, lagrange_vals, label='Интерполяция Лагранжа', linestyle='--', color='blue')

    plt.scatter(x, y, color='red', label='Узлы интерполяции', zorder=5)

    plt.plot(x_vals, tangent_line, '--', label='Касательная в x*', color='red')

    # Подписи и легенда
    plt.title('Интерполяция по точкам')
    plt.xlabel('x')
    plt.ylabel('y')
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.axis('square')
    plt.show()
    plt.savefig("3_4_interpolation")


    x_vals2 = np.linspace(-1.2, 3.3, 4000)
    derivative_line = [tan(elem) for elem in x_vals2]


    plt.figure(figsize=(10, 10))
    plt.title('Производная')
    plt.xlabel('x')
    plt.ylabel('y')
    plt.grid(True)
    plt.tight_layout()
    plt.axvline(x_star, color='gray', linestyle='--', label=f"x* = {x_star}")
    plt.plot(x_vals2, derivative_line, label='Вторая производная', linestyle='-', color='purple')
    plt.legend()
    plt.axis('square')
    plt.savefig("derivative")


    sec_deriv = (tan(x_star+1e-3) - tan(x_star))/+1e-3
    print(f"Вторая производная по функции интерполяции = {sec_deriv}")
