import numpy as np
from random import randint


def splitting(x0, xk, h):
    xs = []
    x = x0
    while x < xk:
        xs.append(x)
        x += h
    xs.append(xk)
    return xs


# Рунге-Кутт 4го порядка
p = 4
as_ = [0, 0.5, 0.5, 1]
bs = [[0.5], [0, 0.5], [0, 0, 1]]
cs = [1 / 6, 1 / 3, 1 / 3, 1 / 6]


def getKs(x: float, y: np.ndarray, h):
    dim = y.shape[0]
    Ks = np.empty((p, dim))
    for i in range(p):
        newX = x + as_[i] * h
        newY = np.copy(y)
        for j in range(i):
            newY += bs[i - 1][j] * Ks[j]
        K = h * f(newX, newY)
        Ks[i] = K
    return Ks


def getDeltaY(x: float, y: np.ndarray, h):
    Ks = getKs(x, y, h)
    sum_ = np.zeros_like(y)
    for i in range(p):
        sum_ += cs[i] * Ks[i]
    return sum_


def RungeKutta(xs: list, y0: np.ndarray, h):
    ys = np.zeros((len(xs), y0.shape[0]))
    ys[0] = y0
    for k in range(1, len(xs)):
        ys[k] = ys[k - 1] + getDeltaY(xs[k - 1], ys[k - 1], h)
    return ys


def Shooting(xs, y_prime_0, h, eps):
    # Более разумные начальные значения
    eta0 = 1.0  # Первое предположение для y(0)
    eta1 = 2.0  # Второе предположение для y(0)

    ys0 = RungeKutta(xs, np.array([eta0, y_prime_0]), h)
    ys1 = RungeKutta(xs, np.array([eta1, y_prime_0]), h)
    F0 = y1(ys0)
    F1 = y1(ys1)

    iter_count = 0
    max_iter = 100  # Максимальное число итераций

    while iter_count < max_iter:
        try:
            eta = eta1 - (eta1 - eta0) / (F1 - F0) * F1
        except ZeroDivisionError:
            eta = (eta0 + eta1) / 2  # Если F1 == F0, берем среднее

        ys = RungeKutta(xs, np.array([eta, y_prime_0]), h)
        F = y1(ys)

        if abs(F) < eps:
            return ys, iter_count, eta

        eta0, eta1 = eta1, eta
        F0, F1 = F1, F
        iter_count += 1

    raise ValueError(f"Метод стрельбы не сошелся за {max_iter} итераций")


def RungeError(ys: np.ndarray, ys2: np.ndarray, p: int):
    k = 2
    error = 0.0
    for i in range(min(len(ys), len(ys2) // 2)):
        error = max(error, abs(ys2[2 * i][0] - ys[i][0]) / (k ** p - 1))
    return error


def f(x: float, y: np.ndarray):
    # Обработка x=0 с использованием предельного перехода
    if np.isclose(x, 0):
        # При x→0 уравнение принимает вид y'' - y' - y = 0
        return np.array([y[1], y[1] + y[0]])
    else:
        return np.array([
            y[1],
            ((2 * x + 1) * y[1] - (x + 1) * y[0]) / x
        ])


def getTrueY(x):
    return np.exp(x) * (x ** 2 + 1)


def y1(ys: np.ndarray):
    return ys[-1][1] - 2 * ys[-1][0]  # y'(1) - 2y(1) = 0


# Основная часть программы
a = 0.001  # Начинаем не с 0, чтобы избежать деления на 0
b = 1
y_prime_0 = 1  # y'(0) = 1
h = 0.125
eps = 1e-9

print(f"Шаг: {h}")
print(f"Точность: {eps}")

xs = splitting(a, b, h)
try:
    ysShooting, iterShooting, eta = Shooting(xs, y_prime_0, h, eps)
    print(f"Итераций в стрельбе: {iterShooting}, Вычисленная y(0) = {eta}")

    for i in range(len(xs)):
        y_true = getTrueY(xs[i])
        y_approx = ysShooting[i][0]
        error = abs(y_approx - y_true)
        print(f"xk = {xs[i]:.5f}, y(xk) = {y_true:.5f}")
        print(f"\tСтрельба: yk = {y_approx:.5f}, e = {error:.8f}")

    # Оценка погрешности
    h2 = h / 2
    xs2 = splitting(a, b, h2)
    ysShooting2, _, _ = Shooting(xs2, y_prime_0, h2, eps)
    print("===============================================================")
    print(f"Апостериорная оценка погрешности по Рунге: {RungeError(ysShooting, ysShooting2, 4):.8f}")

except ValueError as e:
    print(f"Ошибка: {e}")