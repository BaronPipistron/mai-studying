import numpy as np
import matplotlib.pyplot as plt
import math

# ---------------------------------------------------------
# Параметры задачи
# ---------------------------------------------------------
Lx = math.pi / 2.0
Ly = math.pi / 2.0

Nx_default = 20   # число узлов по x
Ny_default = 20   # число узлов по y

eps = 1e-6
max_iter = 10000

tau = 1.0  # "шаг по времени" = шаг по итерации

# ---------------------------------------------------------
# Аналитическое решение и граничные условия
# u(x,y) = e^{-x} cos x cos y
# ---------------------------------------------------------

def u_analytic(x, y):
    return math.exp(-x) * math.cos(x) * math.cos(y)

def build_analytic_grid(Nx, Ny):
    hx = Lx / (Nx - 1)
    hy = Ly / (Ny - 1)
    xs = np.array([i * hx for i in range(Nx)])
    ys = np.array([j * hy for j in range(Ny)])
    U = np.zeros((Nx, Ny), dtype=float)
    for i, x in enumerate(xs):
        for j, y in enumerate(ys):
            U[i, j] = u_analytic(x, y)
    return U, xs, ys

# Граничные условия
def phi_left_x(y: float) -> float:   # x = 0
    return math.cos(y)

def phi_right_x(y: float) -> float:  # x = Lx
    # u(Lx,y) = e^{-Lx} cos(Lx) cos y = 0, т.к. cos(pi/2) = 0
    return 0.0

def phi_bottom_y(x: float) -> float:  # y = 0
    return math.exp(-x) * math.cos(x)

def phi_top_y(x: float) -> float:     # y = Ly
    # u(x,Ly) = e^{-x} cos x cos(Ly) = 0
    return 0.0

# ---------------------------------------------------------
# Коэффициенты разностной схемы
# PDE: u_xx + u_yy + 2 u_x + 3 u = 0
# ---------------------------------------------------------

def build_coeffs(hx, hy):
    a = hy * hy * (1.0 - hx)
    c = hy * hy * (1.0 + hx)
    d = hx * hx
    e = hx * hx
    b = 2.0 * (-hy * hy - hx * hx) + 3.0 * hx * hx * hy * hy
    return a, b, c, d, e

# ---------------------------------------------------------
# Инициализация сетки с учётом границ
# ---------------------------------------------------------

def init_u(Nx, Ny):
    hx = Lx / (Nx - 1)
    hy = Ly / (Ny - 1)
    U = np.zeros((Nx, Ny), dtype=float)

    # граничные значения по y = const
    for i in range(Nx):
        x = i * hx
        U[i, 0]      = phi_bottom_y(x)  # y = 0
        U[i, Ny - 1] = phi_top_y(x)     # y = Ly

    # граничные значения по x = const
    for j in range(Ny):
        y = j * hy
        U[0, j]      = phi_left_x(y)    # x = 0
        U[Nx - 1, j] = phi_right_x(y)   # x = Lx

    # начальное приближение внутри области
    for i in range(1, Nx - 1):
        for j in range(1, Ny - 1):
            U[i, j] = 0.0

    return U, hx, hy

# ---------------------------------------------------------
# Метод Якоби с накоплением истории
# ---------------------------------------------------------

def solve_jacobi(U0, hx, hy):
    Nx, Ny = U0.shape
    a, b, c, d, e = build_coeffs(hx, hy)
    U = U0.copy()
    U_new = U0.copy()

    history = []  # список снимков решения на каждой итерации

    for it in range(max_iter):
        diff = 0.0
        for i in range(1, Nx - 1):
            for j in range(1, Ny - 1):
                rhs = - (a * U[i - 1, j] + c * U[i + 1, j] +
                         d * U[i, j - 1] + e * U[i, j + 1])
                U_new[i, j] = rhs / b
                diff = max(diff, abs(U_new[i, j] - U[i, j]))
        U[:, :] = U_new
        history.append(U.copy())
        if diff < eps:
            print(f"Якоби: итераций = {it + 1}, max diff = {diff:.2e}")
            break
    else:
        print(f"Якоби: не сошёлся за {max_iter} итераций (diff={diff:.2e})")

    history_arr = np.array(history)  # shape (K, Nx, Ny)
    return U, history_arr

# ---------------------------------------------------------
# Метод Зейделя с накоплением истории
# ---------------------------------------------------------

def solve_zeidel(U0, hx, hy):
    Nx, Ny = U0.shape
    a, b, c, d, e = build_coeffs(hx, hy)
    U = U0.copy()

    history = []

    for it in range(max_iter):
        diff = 0.0
        for i in range(1, Nx - 1):
            for j in range(1, Ny - 1):
                rhs = - (a * U[i - 1, j] + c * U[i + 1, j] +
                         d * U[i, j - 1] + e * U[i, j + 1])
                U_new = rhs / b
                diff = max(diff, abs(U_new - U[i, j]))
                U[i, j] = U_new
        history.append(U.copy())
        if diff < eps:
            print(f"Зейдель: итераций = {it + 1}, max diff = {diff:.2e}")
            break
    else:
        print(f"Зейдель: не сошёлся за {max_iter} итераций (diff={diff:.2e})")

    history_arr = np.array(history)
    return U, history_arr

# ---------------------------------------------------------
# Метод верхней релаксации (SOR)
# ---------------------------------------------------------

def solve_sor(U0, hx, hy, omega=1.6):
    Nx, Ny = U0.shape
    a, b, c, d, e = build_coeffs(hx, hy)
    U = U0.copy()

    history = []

    for it in range(max_iter):
        diff = 0.0
        for i in range(1, Nx - 1):
            for j in range(1, Ny - 1):
                rhs = - (a * U[i - 1, j] + c * U[i + 1, j] +
                         d * U[i, j - 1] + e * U[i, j + 1])
                U_old = U[i, j]
                U_star = rhs / b
                U[i, j] = (1.0 - omega) * U_old + omega * U_star
                diff = max(diff, abs(U[i, j] - U_old))
        history.append(U.copy())
        if diff < eps:
            print(f"SOR (ω={omega}): итераций = {it + 1}, max diff = {diff:.2e}")
            break
    else:
        print(f"SOR: не сошёлся за {max_iter} итераций (diff={diff:.2e})")

    history_arr = np.array(history)
    return U, history_arr

# ---------------------------------------------------------
# Оценка погрешности (по финальному слою)
# ---------------------------------------------------------

def compute_errors(U_num, U_exact):
    diff = np.abs(U_num - U_exact)
    return np.max(diff), np.mean(diff)

# ---------------------------------------------------------
# Ошибки во времени (по итерациям)
# u_hist: shape (K, Nx, Ny)
# u_exact: shape (Nx, Ny)
# ---------------------------------------------------------

def compute_time_errors(u_hist, u_exact):
    K, Nx, Ny = u_hist.shape
    N = Nx * Ny

    # Перегоняем в форму (N, K), как в твоей старой функции
    u_num_flat = u_hist.reshape(K, N).T   # (N, K)
    u_ex_flat = np.tile(u_exact.reshape(N, 1), (1, K))  # (N, K)

    t_vals = np.arange(K) * tau
    err_max = np.zeros(K)
    err_mean = np.zeros(K)

    for k in range(K):
        diff = np.abs(u_num_flat[:, k] - u_ex_flat[:, k])
        err_max[k] = np.max(diff)
        err_mean[k] = np.mean(diff)

    return t_vals, err_max, err_mean

def pad_to_length(arr, length):
    if len(arr) == length:
        return arr
    if len(arr) == 0:
        return np.zeros(length)
    pad_val = arr[-1]
    pad_size = length - len(arr)
    if pad_size <= 0:
        return arr[:length]
    return np.concatenate([arr, np.full(pad_size, pad_val)])

def plot_time_errors_methods(t_vals,
                             err_max_jac, err_max_zei, err_max_sor,
                             err_mean_jac=None, err_mean_zei=None, err_mean_sor=None):
    plt.figure(figsize=(7, 4))
    plt.plot(t_vals, err_max_jac, label="max error (Jacobi)")
    plt.plot(t_vals, err_max_zei, label="max error (Zeidel)", linestyle="--")
    plt.plot(t_vals, err_max_sor, label="max error (SOR)", linestyle=":")

    if (err_mean_jac is not None and
        err_mean_zei is not None and
        err_mean_sor is not None):
        plt.plot(t_vals, err_mean_jac, label="mean error (Jacobi)", alpha=0.6)
        plt.plot(t_vals, err_mean_zei, label="mean error (Zeidel)", alpha=0.6)
        plt.plot(t_vals, err_mean_sor, label="mean error (SOR)", alpha=0.6)

    plt.yscale("log")
    plt.xlabel("iteration")
    plt.ylabel("error")
    plt.grid(True, which="both", ls=":")
    plt.legend()
    plt.title("Сходимость методов (ошибка от номера итерации)")


def plot_iterations_bar(iter_jac, iter_zei, iter_sor):
    methods = ["Jacobi", "Zeidel", "SOR"]
    iters = [iter_jac, iter_zei, iter_sor]

    plt.figure(figsize=(5, 4))
    bars = plt.bar(methods, iters)

    # подпишем значения над столбиками
    for bar, val in zip(bars, iters):
        height = bar.get_height()
        plt.text(
            bar.get_x() + bar.get_width() / 2,
            height,
            str(val),
            ha="center",
            va="bottom"
        )

    plt.ylabel("Количество итераций")
    plt.title("Сравнение методов по числу итераций до сходимости")
    plt.grid(True, axis="y", alpha=0.3)


# ---------------------------------------------------------
# Визуализация: 3D Plotly и 2D-сечения
# ---------------------------------------------------------

def print_3d_plotly(U, xs, ys, title="3D (Plotly)", zlabel="u"):
    """
    Вращаемый 3D-график с помощью Plotly.
    """
    import plotly.graph_objects as go

    X, Y = np.meshgrid(xs, ys, indexing="ij")
    fig = go.Figure(data=[go.Surface(x=X, y=Y, z=U)])
    fig.update_layout(
        title=title,
        scene=dict(
            xaxis_title='x',
            yaxis_title='y',
            zaxis_title=zlabel,
        ),
        margin=dict(l=0, r=0, b=0, t=30)
    )
    fig.show()

def print_2d_slice(U_exact, U_num, i_fixed, xs, ys, title):
    x_val = xs[i_fixed]
    plt.figure(figsize=(7, 4))
    plt.plot(ys, U_num[i_fixed, :], label=title)
    plt.plot(ys, U_exact[i_fixed, :], "--", label="analytic")
    plt.xlabel("y")
    plt.ylabel(f"u(x={x_val:.3f}, y)")
    plt.grid(True, alpha=0.3)
    plt.legend()
    plt.title(f"{title} vs analytic (x = {x_val:.3f})")

# ---------------------------------------------------------
# Исследование сходимости по шагу сетки
# ---------------------------------------------------------

def study_grid_convergence(N_values):
    print("\nЗависимость погрешности SOR-решения от шага сетки:")
    print(" Nx  Ny    hx         hy         max_err      mean_err")
    for Nx, Ny in N_values:
        U0, hx, hy = init_u(Nx, Ny)
        U_sor, _ = solve_sor(U0, hx, hy, omega=1.6)
        U_ex, xs, ys = build_analytic_grid(Nx, Ny)
        max_e, mean_e = compute_errors(U_sor, U_ex)
        print(f"{Nx:3d} {Ny:3d}  {hx:8.5f}  {hy:8.5f}  {max_e:10.3e}  {mean_e:10.3e}")

# ---------------------------------------------------------
# Основной сценарий
# ---------------------------------------------------------

if __name__ == "__main__":
    Nx = Nx_default
    Ny = Ny_default

    # инициализация и аналитика
    U0, hx, hy = init_u(Nx, Ny)
    U_ex, xs, ys = build_analytic_grid(Nx, Ny)

    # численные решения + история по итерациям
    U_jac, hist_jac = solve_jacobi(U0, hx, hy)
    U_zei, hist_zei = solve_zeidel(U0, hx, hy)
    U_sor, hist_sor = solve_sor(U0, hx, hy, omega=1.6)

    # ошибки по всей области (по финальному слою)
    max_j, mean_j = compute_errors(U_jac, U_ex)
    max_z, mean_z = compute_errors(U_zei, U_ex)
    max_s, mean_s = compute_errors(U_sor, U_ex)

    iter_jac = hist_jac.shape[0]
    iter_zei = hist_zei.shape[0]
    iter_sor = hist_sor.shape[0]

    print("\nСравнение ошибок (Nx = Ny = %d):" % Nx)
    print(f"Якоби : max = {max_j:.3e}, mean = {mean_j:.3e}")
    print(f"Зейдель: max = {max_z:.3e}, mean = {mean_z:.3e}")
    print(f"SOR    : max = {max_s:.3e}, mean = {mean_s:.3e}")

    # 3D-графики решения (Plotly)
    print_3d_plotly(U_ex,  xs, ys, "Аналитическое решение", zlabel="u")
    print_3d_plotly(U_jac, xs, ys, "Численное решение (Якоби)", zlabel="u")
    print_3d_plotly(U_zei, xs, ys, "Численное решение (Зейдель)", zlabel="u")
    print_3d_plotly(U_sor, xs, ys, "Численное решение (SOR)", zlabel="u")

    # 2D-сечения (по x = const) для сравнения
    i_view = Nx // 3
    print_2d_slice(U_ex, U_jac, i_view, xs, ys, "Якоби")
    print_2d_slice(U_ex, U_zei, i_view, xs, ys, "Зейдель")
    print_2d_slice(U_ex, U_sor, i_view, xs, ys, "SOR")

    # --------- Ошибки "во времени" (по номеру итерации) ---------
    t_jac, err_max_jac, err_mean_jac = compute_time_errors(hist_jac, U_ex)
    t_zei, err_max_zei, err_mean_zei = compute_time_errors(hist_zei, U_ex)
    t_sor, err_max_sor, err_mean_sor = compute_time_errors(hist_sor, U_ex)

    # приводим к общей длине (макс. число итераций среди трёх методов)
    K_max = max(len(t_jac), len(t_zei), len(t_sor))
    t_vals = np.arange(K_max) * tau

    err_max_jac_p  = pad_to_length(err_max_jac,  K_max)
    err_max_zei_p  = pad_to_length(err_max_zei,  K_max)
    err_max_sor_p  = pad_to_length(err_max_sor,  K_max)

    plot_time_errors_methods(t_vals, err_max_jac_p, err_max_zei_p, err_max_sor_p,)
    plot_iterations_bar(iter_jac, iter_zei, iter_sor)

    # исследование сходимости по сетке
    grids = [(10, 10), (20, 20), (40, 40)]
    study_grid_convergence(grids)

    plt.show()
