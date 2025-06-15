import numpy as np
import matplotlib.pyplot as plt


def getValue(as_: list, x):
    value = 0
    for i in range(len(as_)):
        value += as_[i] * x ** i
    return value


n = int(input("Степень многочлена (1-я или 2-я): ")) + 1

if n not in [2, 3]:
    print("Степень многочлена должна быть первой или второй")
    exit(0)

# Базовый случай
xs = np.array([-0.7, -0.4, -0.1, 0.2, 0.5, 0.8])
ys = np.array([2.3462, 1.9823, 1.671, 1.3694, 1.0472, 0.6435])

# Убрали точки
# xs = np.array([-0.7, -0.4, -0.1, 0.2, 0.5])
# ys = np.array([2.3462, 1.9823, 1.671, 1.3694, 1.0472])

# Добавили точки
# xs = np.array([-0.7, -0.4, -0.1, 0.2, 0.5, 0.8, 1.1])
# ys = np.array([2.3462, 1.9823, 1.671, 1.3694, 1.0472, 0.6435, 0.3593])

# Ломаная функция
# xs = np.array([-1, 0, 3, 4])
# ys = np.array([-2, 6, 0, 1])

N = len(xs)

A = np.zeros((n, n))
b = np.zeros(n)
for i in range(n):
    for j in range(N):
        b[i] += ys[j] * xs[j] ** i

    for j in range(n):
        for k in range(N):
            A[i][j] += xs[k] ** (i + j)

as_ = np.linalg.solve(A, b)

print("Приближающий многочлен:")
polynom = []
for i in range(len(as_)):
    polynom.append(f"{np.round(as_[i], 4)} * x^{i}")
print(" + ".join(polynom))

fs = [getValue(as_, x) for x in xs]

error = sum([(fs[i] - ys[i]) ** 2 for i in range(N)])
print(f"Сумма квадратов ошибок: {np.round(error, 4)}")

x_plot = np.linspace(min(xs) - 1, max(xs) + 1, 100)
f_plot = [getValue(as_, x) for x in x_plot]

plt.plot(x_plot, f_plot, linestyle='-', color=(0, 0, 1), label=f"Приближение")
plt.scatter(xs, ys, color='green', label='Заданные точки функции', zorder=5)
plt.legend()
plt.grid(True)
plt.show()