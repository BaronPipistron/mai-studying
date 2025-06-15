import numpy as np
import matplotlib.pyplot as plt


def f(x):
    return x**3 + x**2 - 2*x - 1


def df(x):
    return 3*(x**2) + 2*x - 2


def ddf(x):
    return 6*x + 2

# Пример функции phi для метода простой итерации:
# x = phi(x) <=> x = x - lambda*f(x)
# При правильном выборе lambda метод сходится

lambda_ = 0.1

def phi(x):
    return x**3 + x**2 - x - 1


def d_phi(x):
    return 3*(x**2) + 2*x - 1


def phi_2(x):
    return (x**3 + x**2 - 1) / 2


def d_phi_2(x):
    return 1.5*(x**2) + x


def phi_3(x):
    return (2*x + 1 - x**3) ** 0.5


def d_phi_3(x):
    return (2 - 3*(x**2)) / (2 * (2*x + 1 - x**3)**0.5)


def phi_4(x):
    return (1 + 2*x - x**2) ** (1/3)


def d_phi_4(x):
    return (2 - 2*x) / (3 * ((2*x + 1 - x**2)**2)**(1/3))


# Графики на промежутке [-1, 1.5]
x_vals = np.linspace(-2, 2, 4000)

# Значения функций
f_vals = f(x_vals)

phi_vals = phi(x_vals)
d_phi_vals = d_phi(x_vals)

phi_2_vals = phi_2(x_vals)
d_phi_2_vals = d_phi_2(x_vals)

phi_3_vals = phi_3(x_vals)
d_phi_3_vals = d_phi_3(x_vals)

phi_4_vals = phi_4(x_vals)
d_phi_4_vals = d_phi_4(x_vals)

# Построение графиков
fig, axs = plt.subplots(5, 1, figsize=(20, 15))

# График функции f(x)
axs[0].plot(x_vals, f_vals, label=r'$f(x) = x^3 + x^2 - 2x - 1$', color='blue')
axs[0].axhline(0, color='red', linestyle='--')
axs[0].set_title('Функция $f(x)$')
axs[0].set_xlabel('x')
axs[0].set_ylabel('f(x)')
axs[0].legend()
axs[0].grid(True)

axs[0].axvspan(-2, -1.8, color='green', alpha=0.2)
axs[0].axvspan(-0.5, -0.2, color='green', alpha=0.2)
axs[0].axvspan(1.2, 1.4, color='green', alpha=0.2)

# Графики функции φ'(x)
axs[1].plot(x_vals, d_phi_vals, label=r'$3x^2 + 2x - 1$', color='blue')
axs[1].set_ylim(-2, 4)

axs[2].plot(x_vals, d_phi_2_vals, label=r'$\frac{x^3 + x^2 - 1}{2}$', color='blue')
axs[2].set_ylim(-2, 4)

axs[3].plot(x_vals, d_phi_3_vals, label=r'$\frac{2 - 3x^2}{2\sqrt{-x^3 + 2x + 1}}$', color='blue')
axs[3].set_ylim(-2, 4)

axs[4].plot(x_vals, d_phi_4_vals, label=r'$\frac{2 - 2x}{3\sqrt[3]{(-x^2 + 2x + 1)^2}}$', color='blue')
axs[4].set_ylim(-2, 4)

for i in range(1, 5):
    axs[i].axhline(1, color='red', linestyle='--')
    axs[i].axhline(-1, color='red', linestyle='--')
    axs[i].set_title(f'{i}-й кандидат на функцию φ\'(x) для метода простой итерации')
    axs[i].set_xlabel('x')
    axs[i].set_ylabel('φ\'(x)')
    axs[i].legend()
    axs[i].grid(True)

    # Закрашиваем отрезки, на которых ищем корни, чтобы выбрать подходящие функции φ(x)
    axs[i].axvspan(-2, -1.8, color='green', alpha=0.2)
    axs[i].axvspan(-0.5, -0.2, color='green', alpha=0.2)
    axs[i].axvspan(1.2, 1.4, color='green', alpha=0.2)

plt.tight_layout()
plt.show()
