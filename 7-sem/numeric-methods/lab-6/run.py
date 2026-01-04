import numpy as np
import matplotlib.pyplot as plt
import math

# ---------------------------------------------------------
# Параметры задачи
# ---------------------------------------------------------
L = math.pi
T = 1.0
K = 40
N = 40

tau = T / K
h = L / (N - 1)

# ---------------------------------------------------------
# Аналитическое решение u(x,t) = e^{-t} cos x
# ---------------------------------------------------------
def get_analytic_u():
    u = np.zeros((N, K))
    xs = np.zeros(N)
    for j in range(N):
        x = j * h
        xs[j] = x
        for k in range(K):
            t = k * tau
            u[j, k] = math.exp(-t) * math.cos(x)
    return u, xs

# ---------------------------------------------------------
# Граничные и начальные условия
# ---------------------------------------------------------
def u_left(t=0.0):
    return math.exp(-t)          # u(0,t)

def u_right(t=0.0):
    return -math.exp(-t)         # u(pi,t)

def u_0time(x=0.0):
    return math.cos(x)           # u(x,0)

def ut_0time(x=0.0):
    return -math.cos(x)          # u_t(x,0)

def f_source(x, t):
    # правая часть PDE
    return math.sin(x) * math.exp(-t)

# ---------------------------------------------------------
# ЯВНАЯ СХЕМА "КРЕСТ"
# ---------------------------------------------------------
def solve_explicit(approximation_order="2pf"):
    """
    approximation_order:
        "2pf" -> аппроксимация второго начального условия 1-го порядка
        "2ps" -> аппроксимация второго начального условия 2-го порядка
    """
    u = np.zeros((N, K))
    xs = np.zeros(N)

    # k = 0 (t = 0): начальное условие u(x,0) = cos(x)
    for j in range(N):
        x = j * h
        xs[j] = x
        u[j, 0] = u_0time(x)

    # граничные на k = 0
    u[0, 0] = u_left(0.0)
    u[N - 1, 0] = u_right(0.0)

    # k = 1: аппроксимация второго начального условия
    t1 = tau
    for j in range(1, N - 1):
        x = j * h
        if approximation_order == "2pf":
            # 1-й порядок: u^1 = u^0 + tau * u_t(x,0)
            u[j, 1] = u_0time(x) + tau * ut_0time(x)
        else:
            # 2-й порядок: u^1 = u^0 + tau*u_t + 0.5*tau^2*u_tt
            # u_tt(x,0) найдена из PDE: u_tt(x,0) = cos(x)
            u_tt0 = math.cos(x)
            u[j, 1] = u_0time(x) + tau * ut_0time(x) + 0.5 * tau * tau * u_tt0

    # граничные на k = 1
    u[0, 1] = u_left(t1)
    u[N - 1, 1] = u_right(t1)

    for k in range(1, K - 1):
        t_k = k * tau
        # текущие граничные
        u[0, k] = u_left(t_k)
        u[N - 1, k] = u_right(t_k)

        for j in range(1, N - 1):
            x_j = j * h
            num = (
                2.0 * u[j, k]
                - (1.0 - 1.5 * tau) * u[j, k - 1]
                + (tau * tau / (h * h)) * (u[j + 1, k] - 2.0 * u[j, k] + u[j - 1, k])
                + (tau * tau / (2.0 * h)) * (u[j + 1, k] - u[j - 1, k])
                - tau * tau * u[j, k]
                + tau * tau * f_source(x_j, t_k)
            )
            den = 1.0 + 1.5 * tau
            u[j, k + 1] = num / den

        # граничные на новом слое
        t_next = (k + 1) * tau
        u[0, k + 1] = u_left(t_next)
        u[N - 1, k + 1] = u_right(t_next)

    return u, xs

# ---------------------------------------------------------
# НЕЯВНАЯ СХЕМА (трёхдиагональная СЛАУ)
# ---------------------------------------------------------
def solve_implicit(approximation_order="2pf"):
    u = np.zeros((N, K))
    xs = np.zeros(N)

    # k = 0
    for j in range(N):
        x = j * h
        xs[j] = x
        u[j, 0] = u_0time(x)
    u[0, 0] = u_left(0.0)
    u[N - 1, 0] = u_right(0.0)

    # k = 1
    if approximation_order == "2pf":
        for j in range(1, N - 1):
            x = j * h
            u[j, 1] = u_0time(x) + tau * ut_0time(x)
    else:
        for j in range(1, N - 1):
            x = j * h
            u_tt0 = math.cos(x) 
            u[j, 1] = u_0time(x) + tau * ut_0time(x) + 0.5 * tau * tau * u_tt0

    u[0, 1] = u_left(tau)
    u[N - 1, 1] = u_right(tau)

    # коэффициенты трёхдиагональной матрицы
    A = tau * tau * (1.0 / (2.0 * h) - 1.0 / (h * h))
    C = tau * tau * (-1.0 / (2.0 * h) - 1.0 / (h * h))
    B = 1.0 + 3.0 * tau + tau * tau + 2.0 * tau * tau / (h * h)

    m = N - 2  # число внутренних точек по x

    _check_tridiag_safety(A, B, C, tag=" init")

    for k in range(1, K - 1):
        t_next = (k + 1) * tau
        left = u_left(t_next)
        right = u_right(t_next)

        # правая часть
        rhs = np.zeros(m)
        for j in range(1, N - 1):
            x_j = j * h
            rhs[j - 1] = (
                tau * tau * f_source(x_j, t_next)
                + 3.0 * tau * u[j, k]
                + 2.0 * u[j, k]
                - u[j, k - 1]
            )

        # учёт граничных
        rhs[0] -= A * left
        rhs[-1] -= C * right

        # прогонка
        alpha = np.zeros(m)
        beta = np.zeros(m)

        # первый коэффициент
        alpha[0] = -C / B
        if not (0.0 < alpha[0] < 1.0):
            raise RuntimeError(f"alpha[0] out of (0,1): {alpha[0]}")
        beta[0] = rhs[0] / B

        for i in range(1, m):
            denom = B + A * alpha[i - 1]
            if denom <= 0:
                raise RuntimeError(f"Thomas denom <= 0 at i={i}: {denom}")
            if i < m - 1:
                alpha[i] = -C / denom
                if not (0.0 < alpha[i] < 1.0):
                    raise RuntimeError(f"alpha[{i}] out of (0,1): {alpha[i]}")
            beta[i] = (rhs[i] - A * beta[i - 1]) / denom

        # обратный ход
        u[N - 2, k + 1] = beta[m - 1]
        for i in range(m - 2, -1, -1):
            u[i + 1, k + 1] = alpha[i] * u[i + 2, k + 1] + beta[i]

        # граничные
        u[0, k + 1] = left
        u[N - 1, k + 1] = right

    return u, xs

# ---------------------------------------------------------
# Визуализация
# ---------------------------------------------------------
def print_3d(u, x, title="3D"):
    Nloc, Kloc = u.shape
    t = np.arange(Kloc) * tau

    # X — по x, T — по t
    X, T = np.meshgrid(x, t, indexing="ij")

    fig = plt.figure(figsize=(9, 4))
    ax = fig.add_subplot(111, projection='3d')

    surf = ax.plot_surface(
        X, T, u,
        cmap='plasma',
        edgecolor='none',
        rstride=1,
        cstride=1,
        antialiased=True
    )

    ax.set_xlabel('x')
    ax.set_ylabel('t')
    ax.set_zlabel('u')
    ax.set_title(title)

    ax.view_init(elev=25, azim=-110)

    fig.colorbar(surf, shrink=0.5, aspect=10)



def print_2d(u_analytic, u_num, j, title):
    plt.figure()
    ts = [i * tau for i in range(K)]
    for k in range(K):
        err = abs(u_num[j, k] - u_analytic[j, k])
        print(f"t={k*tau:.3f}  num={u_num[j,k]:.6f}  exact={u_analytic[j,k]:.6f}  err={err:.2e}")
    plt.plot(ts, [u_num[j, i] for i in range(K)], label=title)
    plt.plot(ts, [u_analytic[j, i] for i in range(K)], label="analytic", linestyle="--")
    plt.legend()
    plt.xlabel("t")
    plt.ylabel(f"u(x_{j}, t)")
    plt.grid(True)


# ---------------------------------------------------------
# Ошибки
# ---------------------------------------------------------

def compute_time_errors(u_num, u_exact):
    N, K = u_num.shape
    t_vals = np.arange(K) * tau
    err_max = np.zeros(K)
    err_mean = np.zeros(K)

    for k in range(K):
        diff = np.abs(u_num[:, k] - u_exact[:, k])
        err_max[k] = np.max(diff)
        err_mean[k] = np.mean(diff)

    return t_vals, err_max, err_mean


def plot_time_errors(t_vals, err_max_exp, err_max_imp, err_mean_exp=None, err_mean_imp=None):
    plt.figure(figsize=(7, 4))
    plt.plot(t_vals, err_max_exp, label="max error (explicit)")
    plt.plot(t_vals, err_max_imp, label="max error (implicit)", linestyle="--")
    if err_mean_exp is not None and err_mean_imp is not None:
        plt.plot(t_vals, err_mean_exp, label="mean error (explicit)", alpha=0.6)
        plt.plot(t_vals, err_mean_imp, label="mean error (implicit)", alpha=0.6, linestyle=":")
    plt.yscale("log")
    plt.xlabel("t")
    plt.ylabel("error")
    plt.grid(True, which="both", ls=":")
    plt.legend()
    plt.title("Ошибки во времени")


def plot_space_error_at_time(u_num, u_exact, x, k, title="Ошибка по x"):
    plt.figure(figsize=(7, 3))
    err = np.abs(u_num[:, k] - u_exact[:, k])
    plt.plot(x, err, marker="o")
    plt.xlabel("x")
    plt.ylabel("abs error")
    plt.grid(True)
    plt.title(f"{title}, t = {k * tau:.3f}")


def print_3d_plotly(u, x, title="3D (Plotly)"):
    import plotly.graph_objects as go
    Nloc, Kloc = u.shape
    t = np.arange(Kloc) * tau
    X, T = np.meshgrid(x, t, indexing="ij")
    fig = go.Figure(data=[go.Surface(x=X, y=T, z=u)])
    fig.update_layout(
        title=title,
        scene=dict(xaxis_title='x', yaxis_title='t', zaxis_title='u'),
        margin=dict(l=0, r=0, b=0, t=30)
    )
    fig.show()


def _check_tridiag_safety(A, B, C, tag=""):
    margin = B - (abs(A) + abs(C))
    print(f"[tri{tag}] B={B:.9f}, |A|+|C|={(abs(A)+abs(C)):.9f}, margin={margin:.9f}")
    if margin <= 0:
        raise RuntimeError("Tridiagonal matrix is not strictly diagonally dominant.")
    alpha0 = -C / B
    print(f"[tri{tag}] alpha0={alpha0:.9f}, B-|A|={B-abs(A):.9f}, "
          f"alpha_bound={(-C)/(B-abs(A)):.9f}")
    if not (0.0 < alpha0 < 1.0):
        raise RuntimeError("alpha0 is not in (0,1) — Thomas forward sweep may be unsafe.")


if __name__ == "__main__":
    u_exp, xs = solve_explicit("2ps")
    u_imp, xs2 = solve_implicit("2ps")
    u_an, xs_an = get_analytic_u()

    j_view = N // 3
    print_2d(u_an, u_exp, j_view, "Явная схема")
    print_2d(u_an, u_imp, j_view, "Неявная схема")

    # 3D
    # print_3d(u_exp, xs, "Явная схема")
    # print_3d(u_imp, xs2, "Неявная схема")
    # print_3d(u_an, xs_an, "Аналитическое решение")

    print_3d_plotly(u_exp, xs, "Явная схема — интерактивно")
    print_3d_plotly(u_imp, xs2, "Неявная схема - интерактивно")
    print_3d_plotly(u_an, xs_an, "Аналитическое решение - интерактивно")

    # --------- ошибки во времени ---------
    t_vals, err_max_exp, err_mean_exp = compute_time_errors(u_exp, u_an)
    _, err_max_imp, err_mean_imp = compute_time_errors(u_imp, u_an)

    plot_time_errors(t_vals, err_max_exp, err_max_imp,
                     err_mean_exp, err_mean_imp)

    # --------- ошибка по x в конкретный момент ---------
    # например, в последний момент времени:
    # k_last = K - 1
    # plot_space_error_at_time(u_exp, u_an, xs, k_last,
    #                          title="Ошибка явной схемы по x")
    # plot_space_error_at_time(u_imp, u_an, xs, k_last,
    #                          title="Ошибка неявной схемы по x")

    plt.show()
