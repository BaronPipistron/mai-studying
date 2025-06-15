import sys

def f(x):
    return x / (x**2 + 9)

def splitting(x0, xk, h):
    if h <= 0:
        raise ValueError("Шаг h должен быть положительным числом.")
    if x0 >= xk:
        raise ValueError("Начальная точка x0 должна быть меньше конечной точки xk.")
    xs = []
    x = x0
    while x < xk + sys.float_info.epsilon * 10:
        if x > xk:
            x = xk
        xs.append(x)
        x += h
    if abs(xs[-1] - xk) > sys.float_info.epsilon * 10:
        xs.append(xk)
    if len(xs) > 1 and abs(xs[-1] - xs[-2]) < sys.float_info.epsilon * 10 and abs(xs[-1] - xk) < sys.float_info.epsilon * 10:
        xs.pop(-2)
    return xs

def rectangles(x0, xk, h):
    xs = splitting(x0, xk, h)
    if len(xs) < 2:
    #интеграл на единственном отрезке оценивается по значению функции в его середине, если x0!=k и точек меньше 2
        return 0.0 if x0 == xk else f((x0 + xk) / 2) * (xk - x0)
    integral = 0
    for i in range(len(xs) - 1):
        midpoint = (xs[i] + xs[i+1]) / 2
        integral += h * f(midpoint)
    return integral

def trapezoids(x0, xk, h):
    xs = splitting(x0, xk, h)
    if len(xs) < 2:
        return 0.0 if x0 == xk else 0.5 * (f(x0) + f(xk)) * (xk - x0)
    integral = 0
    for i in range(len(xs) - 1):
        integral += 0.5 * h * (f(xs[i]) + f(xs[i+1]))
    return integral

def simpson(x0, xk, h):
    xs = splitting(x0, xk, h)
    n = len(xs) - 1
    if n <= 0:
        return 0.0
    if n % 2 != 0:
        raise ValueError(f"Для метода Симпсона требуется чётное число интервалов. Получено {n}.")
    integral = 0
    for i in range(0, n, 2):
        integral += h / 3 * (f(xs[i]) + 4 * f(xs[i+1]) + f(xs[i+2]))
    return integral

def runge_romberg(F_h, F_kh, k, p):
    return (F_h - F_kh) / (k**p - 1)

# Базовый случай
input_data = {
    "x0": 0,
    "x_end": 2,
    "h": [0.5, 0.25]
}

# Какой-то хитрый тест
# input_data = {
#     "x0": -2,
#     "x_end": 2.5,
#     "h": [1, 0.25]
# }

print(f"Отрезок интегрирования: [{input_data['x0']}, {input_data['x_end']}]\n")

length = abs(input_data['x0'] - input_data['x_end'])
if length % input_data['h'][0] != 0 or length % input_data['h'][1] != 0:
    print("Отрезок не делится на шаги равномерно")
    exit(1)

results = {}
for step in input_data["h"]:
    print(f"Шаг h = {step}")
    try:
        rect = rectangles(input_data["x0"], input_data["x_end"], step)
        trap = trapezoids(input_data["x0"], input_data["x_end"], step)
        simp = None
        try:
            simp = simpson(input_data["x0"], input_data["x_end"], step)
        except ValueError as ve:
            simp = None

        results[step] = {
            "rect": rect,
            "trap": trap,
            "simp": simp
        }

        print(f"  Метод прямоугольников: {rect:.10f}")
        print(f"  Метод трапеций      : {trap:.10f}")
        if simp is not None:
            print(f"  Метод Симпсона      : {simp:.10f}")
        else:
            print(f"  Метод Симпсона      : Невозможно применить (нечётное число интервалов)")
    except Exception as e:
        print(f"Ошибка при шаге h = {step}: {e}")
    print()

print("Оценка погрешностей по методу Рунге-Ромберга:")
h_big, h_small = input_data["h"]
k = h_big / h_small
print(f"  Для h = {h_big} и h = {h_small}")

try:
    eps_rect = runge_romberg(results[h_big]["rect"], results[h_small]["rect"], k, p=1)
    print(f"    Метод прямоугольников: ±{eps_rect:.10f}")
except:
    print("    Метод прямоугольников: ошибка оценки")

try:
    eps_trap = runge_romberg(results[h_big]["trap"], results[h_small]["trap"], k, p=2)
    print(f"    Метод трапеций      : ±{eps_trap:.10f}")
except:
    print("    Метод трапеций      : ошибка оценки")

try:
    if results[h_big]["simp"] is not None and results[h_small]["simp"] is not None:
        eps_simp = runge_romberg(results[h_big]["simp"], results[h_small]["simp"], k, p=4)
        print(f"    Метод Симпсона      : ±{eps_simp:.10f}")
    else:
        print("    Метод Симпсона      : нельзя применить (одно из значений отсутствует)")
except:
    print("    Метод Симпсона      : ошибка оценки")
