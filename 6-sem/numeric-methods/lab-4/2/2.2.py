import numpy as np

def splitting(x0, xk, h):
    xs = []
    x = x0
    while x < xk:
        xs.append(x)
        x += h
    xs.append(xk)
    return xs

def tridiagonalMatrixAlgorithm(A, b):
    n = len(b)
    P = np.empty(n)
    Q = np.empty(n)

    P[0] = -A[0][2] / A[0][1]
    Q[0] = b[0] / A[0][1]
    for i in range(1, n):
        denominator = A[i][1] + A[i][0] * P[i-1]
        P[i] = -A[i][2] / denominator
        Q[i] = (b[i] - A[i][0] * Q[i-1]) / denominator
    x = np.empty(n)
    x[-1] = Q[-1]
    for i in range(n-2, -1, -1):
        x[i] = P[i] * x[i+1] + Q[i]
    return x

def FiniteDifference(n, xs, h, A_b1, A_c1, A_an, A_bn, b1, bn):
    A = np.zeros((n, 3))
    b = np.empty(n)
    A[0][1] = A_b1
    A[0][2] = A_c1
    b[0] = b1
    A[n-1][0] = A_an
    A[n-1][1] = A_bn
    b[n-1] = bn
    for k in range(1, n-1):
        A[k][0] = 1 - p(xs[k]) * h / 2
        A[k][1] = -2 + h**2 * q(xs[k])
        A[k][2] = 1 + p(xs[k]) * h / 2
        b[k] = h**2 * f(xs[k])
    ys = tridiagonalMatrixAlgorithm(A, b)
    return ys

def p(x):
    return (2 * x + 1) / x  # Коэффициент при y'

def q(x):
    return -(x + 1) / x  # Коэффициент при y

def f(x):
    return 0  # Правая часть уравнения

def RungeError(ys: np.ndarray, ys2: np.ndarray, p):
    k = 2
    error = 0
    for i in range(ys.shape[0]):
        error = max(error, abs(ys2[i*2] - ys[i]) / (k**p - 1))
    return error

def getTrueY(x):
    return np.exp(x) * (x**2 + 1)  # Точное решение

a = 0.001  # Избегаем деления на 0
b = 1
h = 0.001

print(f"Шаг: {h}\n")

xs = splitting(a, b, h)
n = len(xs)

# Краевые условия:
# Левое условие y'(0) = 1 аппроксимируем как (y1 - y0)/h = 1 => -y0 + y1 = h
A_b1 = -1
A_c1 = 1
b1 = h

# Правое условие y'(1) - 2y(1) = 0 аппроксимируем как (yn - yn-1)/h - 2yn = 0 => -yn-1 + (1 - 2h)yn = 0
A_an = -1
A_bn = 1 - 2 * h
bn = 0

ys = FiniteDifference(n, xs, h, A_b1, A_c1, A_an, A_bn, b1, bn)

for i in range(len(xs) - 1):
    y = getTrueY(xs[i])
    print(f"xk = {np.round(xs[i], 5)}, y(xk) = {np.round(y, 5)}")
    error = abs(ys[i] - y)
    print(f"yk = {np.round(ys[i], 5)}, e = {np.round(error, 16)}\n")

print("===================================================================")
h2 = h / 2
xs2 = splitting(a, b, h2)
n2 = len(xs2)
ys2 = FiniteDifference(n2, xs2, h2, -1, 1, -1, 1 - 2*h2, h2, 0)
print(f"Апостериорная оценка погрешности по Рунге: {RungeError(ys, ys2, 2)}")