from __future__ import annotations

import math
from dataclasses import dataclass
from typing import Callable, Tuple

import matplotlib.pyplot as plt
import numpy as np


def f2(x, y1, y2):
    # y'' = (1/x^(1/2))y' - (1/(4x^2))(x + x^(1/2) - 8)y
    return (1.0 / np.sqrt(x)) * y2 - (1.0 / (4.0 * x * x)) * (x + np.sqrt(x) - 8.0) * y1


def y_exact(x: float | np.ndarray) -> float | np.ndarray:
    return (x ** 2 + 1.0 / x) * np.exp(np.sqrt(x))


def _initial_state() -> Tuple[float, float, float]:
    # Начальные значения: x=1, y=2e, y'=2e
    return 1.0, 2.0 * math.e, 2.0 * math.e


def _make_grid(h: float, x_end: float = 2.0, *,
               min_nodes: int = 2) -> np.ndarray:
    if h <= 0:
        raise ValueError("шаг h должен быть > 0")

    q, r = divmod(x_end - 1.0, h)
    if r > 1e-12 and abs(r - h) > 1e-12:
        raise ValueError(
            f"длина отрезка {x_end - 1.0} не кратна шагу h={h} (остаток {r:g})"
        )

    n = int(round((x_end - 1.0) / h))

    if n + 1 < min_nodes:
        raise ValueError(
            f"нужно ≥{min_nodes} узлов, а получается {n + 1}"
        )

    return np.linspace(1.0, x_end, n + 1)


def euler(h: float, x_end: float = 2.0):
    # Явный метод Эйлера для системы уравнений
    x = _make_grid(h, x_end)
    y1 = np.empty_like(x)
    y2 = np.empty_like(x)

    xi, y1i, y2i = _initial_state()
    y1[0], y2[0] = y1i, y2i

    for k in range(1, len(x)):
        y1i, y2i = y1i + h * y2i, y2i + h * f2(xi, y1i, y2i)
        xi += h
        y1[k], y2[k] = y1i, y2i

    return x, y1, y2


def rk4(h: float, x_end: float = 2.0):
    # Метод Рунге–Кутта 4-го порядка
    x = _make_grid(h, x_end)
    y1 = np.empty_like(x)
    y2 = np.empty_like(x)

    xi, y1i, y2i = _initial_state()
    y1[0], y2[0] = y1i, y2i

    for k in range(1, len(x)):
        k1y1, k1y2 = y2i, f2(xi, y1i, y2i)

        k2y1 = y2i + 0.5 * h * k1y2
        k2y2 = f2(xi + 0.5 * h, y1i + 0.5 * h * k1y1, y2i + 0.5 * h * k1y2)

        k3y1 = y2i + 0.5 * h * k2y2
        k3y2 = f2(xi + 0.5 * h, y1i + 0.5 * h * k2y1, y2i + 0.5 * h * k2y2)

        k4y1 = y2i + h * k3y2
        k4y2 = f2(xi + h, y1i + h * k3y1, y2i + h * k3y2)

        y1i += (h / 6.0) * (k1y1 + 2 * k2y1 + 2 * k3y1 + k4y1)
        y2i += (h / 6.0) * (k1y2 + 2 * k2y2 + 2 * k3y2 + k4y2)
        xi += h

        y1[k], y2[k] = y1i, y2i

    return x, y1, y2


def adams(h: float, x_end: float = 2.0):
    # Явный многошаговый метод Адамса 4-го порядка
    min_nodes = 5
    x = _make_grid(h, x_end, min_nodes=min_nodes)
    y1 = np.empty_like(x)
    y2 = np.empty_like(x)

    xi, y1i, y2i = _initial_state()
    y1[0], y2[0] = y1i, y2i

    f_hist = []
    for k in range(1, 4):
        k1y1, k1y2 = y2i, f2(xi, y1i, y2i)
        k2y1 = y2i + 0.5 * h * k1y2
        k2y2 = f2(xi + 0.5 * h, y1i + 0.5 * h * k1y1, y2i + 0.5 * h * k1y2)
        k3y1 = y2i + 0.5 * h * k2y2
        k3y2 = f2(xi + 0.5 * h, y1i + 0.5 * h * k2y1, y2i + 0.5 * h * k2y2)
        k4y1 = y2i + h * k3y2
        k4y2 = f2(xi + h, y1i + h * k3y1, y2i + h * k3y2)
        y1i += (h / 6.0) * (k1y1 + 2 * k2y1 + 2 * k3y1 + k4y1)
        y2i += (h / 6.0) * (k1y2 + 2 * k2y2 + 2 * k3y2 + k4y2)
        xi += h
        y1[k], y2[k] = y1i, y2i
        f_hist.append(f2(xi, y1i, y2i))

    for k in range(4, len(x)):
        f_n, f_nm1, f_nm2 = f_hist[-1], f_hist[-2], f_hist[-3]
        f_nm3 = f2(x[k - 4], y1[k - 4], y2[k - 4])
        y1i += (h / 24.0) * (55 * y2i - 59 * y2[k - 2] + 37 * y2[k - 3] - 9 * y2[k - 4])
        y2i += (h / 24.0) * (55 * f_n - 59 * f_nm1 + 37 * f_nm2 - 9 * f_nm3)
        xi += h
        y1[k], y2[k] = y1i, y2i
        f_hist.append(f2(xi, y1i, y2i))

    return x, y1, y2


def compute_errors(method: Callable[[float], Tuple[np.ndarray, np.ndarray, np.ndarray]], h: float, p: int):
    x_coarse, y1_coarse, _ = method(h)
    x_fine, y1_fine, _ = method(h / 2)

    y_fine_on_coarse = y1_fine[::2]
    exact = y_exact(x_coarse)

    err_exact = np.abs(y1_coarse - exact)
    err_est = np.abs(y_fine_on_coarse - y1_coarse) / (2 ** p - 1)

    return np.column_stack((x_coarse, y1_coarse, exact, err_exact, err_est))


def plot_results(name: str, tbl: np.ndarray):
    x, y_num, y_ex, err_ex, err_est = tbl.T

    fig = plt.figure(figsize=(12, 6))

    ax1 = fig.add_subplot(1, 2, 1)
    ax1.plot(x, y_num, label="Численное решение")
    ax1.plot(x, y_ex, "--", label="Точное решение")
    ax1.set_xlabel("x")
    ax1.set_ylabel("y")
    ax1.set_title(f"{name}: решения")
    ax1.legend()

    ax2 = fig.add_subplot(1, 2, 2)
    ax2.semilogy(x, err_ex, label="|ошибка| точная")
    ax2.semilogy(x, err_est, label="оценка RR")
    ax2.set_xlabel("x")
    ax2.set_ylabel("|error|")
    ax2.set_title(f"{name}: погрешности")
    ax2.legend()

    fig.tight_layout()
    fig.savefig(name.replace(" ", "_") + ".png", dpi=150)


@dataclass(slots=True)
class Method:
    func: Callable[[float], Tuple[np.ndarray, np.ndarray, np.ndarray]]
    order: int


METHODS = {
    "Euler": Method(euler, 1),
    "Runge-Kutta-4": Method(rk4, 4),
    "Adams-4": Method(adams, 4),
}

CONTROL_POINTS = [
    (1.5, (1.5 ** 2 + 1 / 1.5) * np.exp(np.sqrt(1.5))),
]


def check_points(method_func: Callable[[float], Tuple[np.ndarray, np.ndarray, np.ndarray]],
                 h: float):
    x, y1, _ = method_func(h)

    for xc, y_expected in CONTROL_POINTS:
        y_interp = np.interp(xc, x, y1)
        err = abs(y_interp - y_expected)
        print(f"  x = {xc:6.3f}  y(x) ≈ {y_interp:12.6f} "
              f"(ожидалось {y_expected:12.6f}),  |err| = {err:.2e}")


def main():
    h = 0.1

    for name, m in METHODS.items():
        tbl = compute_errors(m.func, h, m.order)

        print(f"\n{name} (order {m.order})")
        print("x     y_num        y_exact      |err|         est_RR")
        for row in tbl:
            x_i, y_n, y_ex, e_ex, e_rr = row
            print(f"{x_i:3.3f}  {y_n:12.6f} {y_ex:12.6f} {e_ex:11.2e} {e_rr:11.2e}")

        print("Контроль значения в заданных точках:")
        check_points(m.func, h)

        plot_results(name, tbl)


if __name__ == "__main__":
    main()