from __future__ import annotations

import numpy as np
import matplotlib.pyplot as plt
from typing import Sequence, List, Tuple

# Параметры задачи
a: float = 0.0
b: float = 1.0
alpha: float = 1.0  # y'(0) = 1
beta: float = 0.0   # y'(1) - 2y(1) = 0

def y_exact(x: float | np.ndarray) -> float | np.ndarray:
    """
    Точное решение краевой задачи:
    y(x) = e^x (x^2 + 1)
    """
    return np.exp(x) * (x**2 + 1.0)

def rhs(x: float, Y: Sequence[float]) -> Tuple[float, float]:
    """
    Правая часть системы первого порядка:
      y1' = y2,
      y2' = [(2x+1)/x]y2 - [(x+1)/x]y1
    """
    y1, y2 = Y
    if np.isclose(x, 0.0):
        # При x=0 используем предельные значения
        return y2, (3.0*y2 - y1)  # Лопиталь для x->0
    return y2, ((2.0*x + 1.0)/x)*y2 - ((x + 1.0)/x)*y1

def rk4_step(f, x: float, Y: Sequence[float], h: float) -> List[float]:
    """
    Один шаг классического РК-4:
    k1 = f(x,    Y)
    k2 = f(x+h/2,Y + h/2·k1)
    k3 = f(x+h/2,Y + h/2·k2)
    k4 = f(x+h,  Y + h·k3)
    Новое Y = Y + h/6·(k1+2k2+2k3+k4)
    """
    k1 = f(x, Y)
    k2 = f(x + h/2, [Y[i] + h * k1[i] / 2 for i in range(2)])
    k3 = f(x + h/2, [Y[i] + h * k2[i] / 2 for i in range(2)])
    k4 = f(x + h,     [Y[i] + h * k3[i]     for i in range(2)])
    return [Y[i] + h * (k1[i] + 2*k2[i] + 2*k3[i] + k4[i]) / 6 for i in range(2)]

def solve_ivp(f, x0: float, Y0: Sequence[float], grid: np.ndarray) -> list[list[float]]:
    """
    Универсальная обёртка: решает задачу Коши y'=f(x,y) методом RK-4 по точкам grid.
    Возвращает список [y1,y2] в каждом узле.
    """
    Y = list(Y0)
    sol = [Y.copy()]
    for k in range(len(grid) - 1):
        h = grid[k + 1] - grid[k]
        Y = rk4_step(f, grid[k], Y, h)
        sol.append(Y.copy())
    return sol

def shooting(a: float, b: float,
             alpha: float, beta: float,
             h: float, tol: float = 1e-12,
            max_it = 50):
    """
    Метод стрельбы для краевой задачи:
    — Интегрируем систему с неизвестным начальным y1(a)=s
    — Подбираем s секущим методом так, чтобы на правом конце условие
      y'(b) − 2y(b) = beta выполнялось с точностью tol.
    """
    x_grid = np.arange(a, b + 1e-13, h)

    def residual(s: float) -> float:
        # Невязка на правом краю
        y_end, yprime_end = solve_ivp(rhs, a, (s, alpha), x_grid)[-1]
        return yprime_end - 2.0*y_end - beta

    # Две начальные догадки для секущего метода
    s0, s1   = 0.0, 1.0
    F0, F1   = residual(s0), residual(s1)
    it = 0
    print("iter |      s_k            F(s_k)")
    print(f"{it:4d} | {s0:16.9e} {F0:16.9e}")
    it += 1
    print(f"{it:4d} | {s1:16.9e} {F1:16.9e}")

    while abs(F1) > tol and it < max_it:
        s0, F0, s1 = s1, F1, s1 - F1 * (s1 - s0) / (F1 - F0)   # секущая
        F1 = residual(s1)
        it += 1
        print(f"{it:4d} | {s1:16.9e} {F1:16.9e}")

    if abs(F1) > tol:
        raise RuntimeError("shooting: не сошлось за max_it итераций")

    sol = solve_ivp(rhs, a, (s1, alpha), x_grid)
    return x_grid, np.array([y[0] for y in sol])

def thomas_algorithm(a, b, c, f):
    """
    a[i] = элемент под диагональю (i-й строки, i-1-й столбец)
    b[i] = диагональ
    c[i] = над диагональю
    f[i] = правая часть
    Результат: x, решение системы 3-диагональной
    """
    n = len(b)
    if any(len(arr) != n for arr in [a, c, f]):
        raise ValueError("Thomas: несовместимые размеры")
    alpha = [0]*n
    beta = [0]*n

     # Диагональное преобладание (|b_i| ≥ |a_i|+|c_i|)
    warn_printed = False
    for i in range(n):
        s = abs(a[i]) + abs(c[i])
        if 1 <= i <= n-2 and abs(b[i]) < abs(a[i])+abs(c[i]):
            # raise ValueError("Диагональное преобладание нарушено")
            if not warn_printed:
                print("WARN: Диагонально преобладание нарушено, Трехдиагональный Матричный Алгоритм может расходиться!!!")
                warn_printed = True
            else:
                pass

    # Прямая прогонка
    alpha[0] = -c[0]/b[0]
    beta[0] = f[0]/b[0]

    for i in range(1, n):
        denom = b[i] + a[i]*alpha[i-1]
        if abs(denom) < 1e-15:
            raise ValueError("Thomas: вырожденная или некорректная матрица")
        alpha[i] = -c[i]/denom
        beta[i]  = (f[i] - a[i]*beta[i-1]) / denom

    # Обратная прогонка
    x = [0]*n
    x[n-1] = beta[n-1]
    for i in range(n-2, -1, -1):
        x[i] = alpha[i]*x[i+1] + beta[i]

    return x

def finite_difference(a: float, b: float,
                      alpha: float, beta: float,
                      h: float):
    """
    xy'' + (2x+1)y' - (x+1)y = 0
    y'(0)=α, y'(1) - 2y(1) = β
    """

    x = np.arange(a, b + h*0.5, h)
    N = len(x) - 1

    A = np.zeros(N+1)
    B = np.zeros(N+1)
    C = np.zeros(N+1)
    F = np.zeros(N+1)

    # Левое граничное условие: y'(0) = alpha
    # Используем одностороннюю разностную аппроксимацию вперед:
    #   y'(0) ≈ (y₁ - y₀)/h = alpha
    B[0] = -1.0/h
    C[0] =  1.0/h
    F[0] =  alpha

    # Уравнения для внутренних точек (j = 1, 2, ..., N-1)
    for j in range(1, N):
        xj = x[j]
        A[j] = xj/h**2 - (2*xj + 1)/(2*h)  # Коэф. при y_{j-1}
        B[j] = -2*xj/h**2 + (xj + 1)       # Коэф. при y_j
        C[j] = xj/h**2 + (2*xj + 1)/(2*h)   # Коэф. при y_{j+1}

    # Правое граничное условие: y'(1) - 2y(1) = beta
    # Используем одностороннюю разностную аппроксимацию назад:
    #   y'(1) ≈ (y_N - y_{N-1})/h
    A[N] = -1.0/h
    B[N] =  1.0/h - 2.0
    F[N] =  beta

    y = thomas_algorithm(A, B, C, F)
    return x, np.asarray(y)

def runge_romberg(y_h: np.ndarray, y_h2: np.ndarray, p: int = 4) -> float:
    """
    Оценка погрешности по методу Рунге–Ромберга:
    max|y(h/2) − y(h)|/(2^p − 1)
    """
    return np.max(np.abs(y_h2[::2] - y_h)) / (2 ** p - 1)

def main():
    h  = (b - a) / 100
    h2 = h / 2

    x_h,  y_h  = shooting(a, b, alpha, beta, h)
    x_h2, y_h2 = shooting(a, b, alpha, beta, h2)

    x_fd, y_fd = finite_difference(a, b, alpha, beta, h)
    x_fd2, y_fd2 = finite_difference(a, b, alpha, beta, h2)

    err_exact = np.max(np.abs(y_h - y_exact(x_h)))
    err_rr    = runge_romberg(y_h, y_h2)
    err_exactfd = np.max(np.abs(y_fd - y_exact(x_fd)))
    err_rrfd    = runge_romberg(y_fd, y_fd2)
    print(f"[shoot] max|числ-точн| = {err_exact:.3e}")
    print(f"[shoot] Runge–Romberg  = {err_rr:.3e}")

    print("\n[Shooting]".ljust(54, "─"))
    print("     x        y_num        y_exact      |err|")
    for k in range(0, len(x_h), 5):
        print(f"{x_h[k]:7.4f}  {y_h[k]:12.8f}  "
              f"{y_exact(x_h[k]):12.8f}  "
              f"{abs(y_h[k] - y_exact(x_h[k])):9.2e}")

    print(f"\n[FD] max|числ-точн| = {err_exactfd:.3e}")
    print(f"[FD] Runge–Romberg  = {err_rrfd:.3e}")

    print("\n[Finite difference]".ljust(54, "─"))
    print("     x        y_num        y_exact      |err|")
    for k in range(0, len(x_fd), 5):
        print(f"{x_fd[k]:7.4f}  {y_fd[k]:12.8f}  "
              f"{y_exact(x_fd[k]):12.8f}  "
              f"{abs(y_fd[k] - y_exact(x_fd[k])):9.2e}")

    plt.figure(figsize=(10, 6))
    plt.plot(x_h,  y_h,  "bo-", label="стрельба  h")
    plt.plot(x_h2, y_h2, "g--", label="стрельба  h/2")
    plt.plot(x_fd, y_fd, "k-.", label="конечно-разн.")
    plt.plot(x_h,  y_exact(x_h), "r:", label="точное")
    plt.title("Краевая задача: сравнение методов")
    plt.xlabel("x"); plt.ylabel("y"); plt.grid(True); plt.legend()
    plt.savefig("bvp_compare.png", dpi=150)
    plt.show()

if __name__ == "__main__":
    main()