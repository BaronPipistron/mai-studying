import math
import numpy as np
import matplotlib.pyplot as plt


def tridiagonal_solve(a_diag, b_sub, c_sup, d):
    n = len(a_diag)
    a = a_diag.astype(float).copy()
    b = b_sub.astype(float).copy()
    c = c_sup.astype(float).copy()
    d = d.astype(float).copy()

    for i in range(1, n):
        w = b[i - 1] / a[i - 1]
        a[i] -= w * c[i - 1]
        d[i] -= w * d[i - 1]

    x = np.zeros(n, dtype=float)
    x[-1] = d[-1] / a[-1]

    for i in range(n - 2, -1, -1):
        x[i] = (d[i] - c[i] * x[i + 1]) / a[i]

    return x


# ---------- параметры задачи ----------
a = 1.0  # диффузия
b = 1.0  # конвекция
x_begin = 0.0
x_end = math.pi
t_begin = 0.0
t_end = 0.5

h = math.pi / 160
sigma = 0.45


def F_left(t):
    return -math.exp(-a * t) * (math.cos(b * t) + math.sin(b * t))


def F_right(t):
    return +math.exp(-a * t) * (math.cos(b * t) + math.sin(b * t))


def psi0(x):
    return math.cos(x)


def analytical_solution_fn(x, t):
    return math.exp(-a * t) * math.cos(x + b * t)


def get_analytical_solution(x_range, t_range, h, sigma, a=a):
    tau = sigma * h ** 2 / a
    x = np.arange(x_range[0], x_range[1] + h / 2, h)
    t = np.arange(t_range[0], t_range[1] + tau / 2, tau)
    U = np.zeros((len(t), len(x)))

    for it in range(len(t)):
        for ix in range(len(x)):
            U[it, ix] = analytical_solution_fn(x[ix], t[it])

    return x, t, U


def max_abs_error(A, B):
    assert A.shape == B.shape
    return np.abs(A - B).max()


def mean_abs_error(A, B):
    assert A.shape == B.shape
    return np.abs(A - B).mean()


# -------------------------------------------------------
# bc_type: '2pt_Oh1' (двухточечная O(h))  |  '3pt_Oh2' (трёхточечная O(h^2))
# -------------------------------------------------------

# ================= ЯВНАЯ =================
def explicit_finite_difference_method(x_range, t_range, h, sigma, a=a, b=b, bc_type='3pt_Oh2'):
    tau = sigma * h ** 2 / a
    x = np.arange(x_range[0], x_range[1] + h / 2, h)
    t = np.arange(t_range[0], t_range[1] + tau / 2, tau)

    Nx = len(x)
    Nt = len(t)
    res = np.zeros((Nt, Nx))
    for i in range(Nx):
        res[0, i] = psi0(x[i])

    r = a * tau / h ** 2
    mu = b * tau / (2 * h)

    for n in range(1, Nt):
        # внутренние узлы
        for i in range(1, Nx - 1):
            res[n, i] = (
                    res[n - 1, i]
                    + r * (res[n - 1, i - 1] - 2 * res[n - 1, i] + res[n - 1, i + 1])
                    + mu * (res[n - 1, i + 1] - res[n - 1, i - 1])
            )

        # граничные условия (исправлено!)
        F0 = F_left(t[n]);
        Fp = F_right(t[n])

        if bc_type == '2pt_Oh1':
            res[n, 0] = (res[n, 1] - h * F0) / (1.0 + h)
            res[n, -1] = (res[n, -2] + h * Fp) / (1.0 - h)  # <-- было 1+h, теперь 1-h
        else:  # '3pt_Oh2'
            res[n, 0] = (4 * res[n, 1] - res[n, 2] - 2 * h * F0) / (3.0 + 2.0 * h)
            res[n, -1] = (4 * res[n, -2] - res[n, -3] + 2 * h * Fp) / (3.0 - 2.0 * h)

    return x, t, res


# ================= НЕЯВНАЯ =================
def implicit_finite_difference_method(x_range, t_range, h, sigma, a=a, b=b, bc_type='3pt_Oh2'):
    tau = sigma * h ** 2 / a
    x = np.arange(x_range[0], x_range[1] + h / 2, h)
    t = np.arange(t_range[0], t_range[1] + tau / 2, tau)

    Nx = len(x)
    Nt = len(t)
    res = np.zeros((Nt, Nx))
    for i in range(Nx):
        res[0, i] = psi0(x[i])

    r = a * tau / h ** 2
    mu = b * tau / (2 * h)

    A = -(r - mu)
    B = 1 + 2 * r
    C = -(r + mu)

    n_in = Nx - 2
    for n in range(1, Nt):
        F0 = F_left(t[n]);
        Fp = F_right(t[n])

        a_diag = np.full(n_in, B)
        b_sub = np.full(n_in - 1, A) if n_in > 1 else np.array([])
        c_sup = np.full(n_in - 1, C) if n_in > 1 else np.array([])
        rhs = res[n - 1, 1:-1].copy()

        # левая граница
        if bc_type == '2pt_Oh1':
            alpha = 1.0 / (1.0 + h)
            const = -h * F0 / (1.0 + h)
            a_diag[0] += A * alpha
            rhs[0] -= A * const
        else:
            denom = (3.0 + 2.0 * h)
            c1, c2, c0 = 4.0 / denom, -1.0 / denom, -2.0 * h * F0 / denom
            a_diag[0] += A * c1
            if n_in > 1: c_sup[0] += A * c2
            rhs[0] -= A * c0

        # правая граница (исправлено!)
        if bc_type == '2pt_Oh1':
            beta = 1.0 / (1.0 - h)  # <-- было 1+h
            constR = h * Fp / (1.0 - h)
            a_diag[-1] += C * beta
            rhs[-1] -= C * constR
        else:
            denom = (3.0 - 2.0 * h)
            d1, d2, d0 = 4.0 / denom, -1.0 / denom, 2.0 * h * Fp / denom
            a_diag[-1] += C * d1
            if n_in > 1: b_sub[-1] += C * d2
            rhs[-1] -= C * d0

        res[n, 1:-1] = tridiagonal_solve(a_diag, b_sub, c_sup, rhs)

        # восстановление краёв
        if bc_type == '2pt_Oh1':
            res[n, 0] = (res[n, 1] - h * F0) / (1.0 + h)
            res[n, -1] = (res[n, -2] + h * Fp) / (1.0 - h)
        else:
            res[n, 0] = (4 * res[n, 1] - res[n, 2] - 2 * h * F0) / (3.0 + 2.0 * h)
            res[n, -1] = (4 * res[n, -2] - res[n, -3] + 2 * h * Fp) / (3.0 - 2.0 * h)

    return x, t, res


# ================= КРАНК–НИКОЛСОН =================
def crank_nicolson_method(x_range, t_range, h, sigma, a=a, b=b, bc_type='3pt_Oh2', theta=0.5):
    tau = sigma * h ** 2 / a
    x = np.arange(x_range[0], x_range[1] + h / 2, h)
    t = np.arange(t_range[0], t_range[1] + tau / 2, tau)

    Nx = len(x)
    Nt = len(t)
    res = np.zeros((Nt, Nx))
    for i in range(Nx):
        res[0, i] = psi0(x[i])

    r = a * tau / h ** 2
    mu = b * tau / (2 * h)

    # LHS
    AL = -0.5 * (r - mu)
    BL = 1.0 + r
    CL = -0.5 * (r + mu)
    # RHS
    AR = +0.5 * (r - mu)
    BR = 1.0 - r
    CR = +0.5 * (r + mu)

    n_in = Nx - 2
    for n in range(1, Nt):
        # базовый RHS
        rhs = np.zeros(n_in)
        for j in range(1, Nx - 1):
            rhs[j - 1] = AR * res[n - 1, j - 1] + BR * res[n - 1, j] + CR * res[n - 1, j + 1]

        a_diag = np.full(n_in, BL)
        b_sub = np.full(n_in - 1, AL) if n_in > 1 else np.array([])
        c_sup = np.full(n_in - 1, CL) if n_in > 1 else np.array([])

        # полушаговые F
        tmid = 0.5 * (t[n] + t[n - 1])
        F0m, Fpm = F_left(tmid), F_right(tmid)

        # ---- левый край ----
        if bc_type == '2pt_Oh1':
            # LHS: u0^{k+1} = alpha*u1^{k+1} + cst
            alpha = 1.0 / (1.0 + h);
            cst = -h * F0m / (1.0 + h)
            a_diag[0] += AL * alpha
            rhs[0] -= AL * cst

            # RHS: заменить AR*u0^k на AR*(alpha_k*u1^k + cst_k)
            alpha_k = 1.0 / (1.0 + h);
            cst_k = -h * F_left(t[n - 1]) / (1.0 + h)
            rhs[0] -= AR * res[n - 1, 0]  # убрать базовый u0^k
            rhs[0] += AR * (alpha_k * res[n - 1, 1] + cst_k)  # добавить подстановку

        else:  # '3pt_Oh2'
            # LHS: u0^{k+1} = c1*u1^{k+1} + c2*u2^{k+1} + c0
            denom = (3.0 + 2.0 * h)
            c1, c2, c0 = 4.0 / denom, -1.0 / denom, -2.0 * h * F0m / denom
            a_diag[0] += AL * c1
            if n_in > 1: c_sup[0] += AL * c2
            rhs[0] -= AL * c0

            # RHS: заменить AR*u0^k
            denom_k = (3.0 + 2.0 * h)
            c1k, c2k, c0k = 4.0 / denom_k, -1.0 / denom_k, -2.0 * h * F_left(t[n - 1]) / denom_k
            rhs[0] -= AR * res[n - 1, 0]  # убрать базовый u0^k
            rhs[0] += AR * (c1k * res[n - 1, 1] + c2k * res[n - 1, 2] + c0k)  # добавить подстановку

        # ---- правый край ----
        if bc_type == '2pt_Oh1':
            # LHS: uN^{k+1} = beta*u_{N-1}^{k+1} + d, beta=1/(1-h)
            beta = 1.0 / (1.0 - h);
            d = h * Fpm / (1.0 - h)
            a_diag[-1] += CL * beta
            rhs[-1] -= CL * d

            # RHS: заменить CR*uN^k
            beta_k = 1.0 / (1.0 - h);
            d_k = h * F_right(t[n - 1]) / (1.0 - h)
            rhs[-1] -= CR * res[n - 1, -1]  # убрать базовый uN^k
            rhs[-1] += CR * (beta_k * res[n - 1, -2] + d_k)  # добавить подстановку

        else:  # '3pt_Oh2'
            # LHS: uN^{k+1} = d1*u_{N-1}^{k+1} + d2*u_{N-2}^{k+1} + d0
            denomR = (3.0 - 2.0 * h)
            d1, d2, d0 = 4.0 / denomR, -1.0 / denomR, 2.0 * h * Fpm / denomR
            a_diag[-1] += CL * d1
            if n_in > 1: b_sub[-1] += CL * d2
            rhs[-1] -= CL * d0

            # RHS: заменить CR*uN^k
            denomRk = (3.0 - 2.0 * h)
            d1k, d2k, d0k = 4.0 / denomRk, -1.0 / denomRk, 2.0 * h * F_right(t[n - 1]) / denomRk
            rhs[-1] -= CR * res[n - 1, -1]  # убрать базовый uN^k
            rhs[-1] += CR * (d1k * res[n - 1, -2] + d2k * res[n - 1, -3] + d0k)  # добавить подстановку

        # решить и восстановить края (на среднем времени)
        sol = tridiagonal_solve(a_diag, b_sub, c_sup, rhs)
        res[n, 1:-1] = sol

        if bc_type == '2pt_Oh1':
            res[n, 0] = (res[n, 1] - h * F0m) / (1.0 + h)
            res[n, -1] = (res[n, -2] + h * Fpm) / (1.0 - h)  # <-- 1-h
        else:
            res[n, 0] = (4 * res[n, 1] - res[n, 2] - 2 * h * F0m) / (3.0 + 2.0 * h)
            res[n, -1] = (4 * res[n, -2] - res[n, -3] + 2 * h * Fpm) / (3.0 - 2.0 * h)

    return x, t, res


# =================== запуск расчётов ===================
x_ana, t_ana, analytical = get_analytical_solution(
    x_range=(x_begin, x_end),
    t_range=(t_begin, t_end),
    h=h,
    sigma=sigma,
    a=a,
)

solutions = {}
x_exp, t_exp, explicit_sol = explicit_finite_difference_method(
    x_range=(x_begin, x_end),
    t_range=(t_begin, t_end),
    h=h,
    sigma=sigma,
    bc_type='3pt_Oh2'
)
solutions["Явное"] = explicit_sol

x_imp, t_imp, implicit_sol = implicit_finite_difference_method(
    x_range=(x_begin, x_end),
    t_range=(t_begin, t_end),
    h=h,
    sigma=sigma,
    bc_type='3pt_Oh2'
)
solutions["Неявное"] = implicit_sol

x_cn, t_cn, cn_sol = crank_nicolson_method(
    x_range=(x_begin, x_end),
    t_range=(t_begin, t_end),
    h=h,
    sigma=sigma,
    bc_type='3pt_Oh2',
    theta=0.5
)
solutions["Кранк-Николсон"] = cn_sol

print("shapes:", explicit_sol.shape, analytical.shape)
print("Явное: max, mean error =",
      max_abs_error(explicit_sol, analytical),
      mean_abs_error(explicit_sol, analytical))
print("Неявное: max, mean error =",
      max_abs_error(implicit_sol, analytical),
      mean_abs_error(implicit_sol, analytical))
print("Кранк-Николсон: max, mean error =",
      max_abs_error(cn_sol, analytical),
      mean_abs_error(cn_sol, analytical))


def plot_scheme_evolution_separate_windows(solutions, x, t):
    time_points = [0.1, 0.2, 0.35, 0.45, t[-1]]
    colors = plt.cm.viridis(np.linspace(0, 1, len(time_points)))

    schemes = list(solutions.keys())

    for idx, scheme_name in enumerate(schemes):
        plt.figure(figsize=(10, 6))
        sol = solutions[scheme_name]

        for i, tp in enumerate(time_points):
            cur_t_id = int(np.abs(t - tp).argmin())
            plt.plot(x, sol[cur_t_id],
                     color=colors[i],
                     label=f't={t[cur_t_id]:.3f}',
                     linewidth=2)

        plt.xlabel('x', fontsize=12)
        plt.ylabel('u(x, t)', fontsize=12)
        plt.title(f'Эволюция во времени: {scheme_name} схема', fontsize=14)
        plt.legend(fontsize=10)
        plt.grid(True, alpha=0.3)
        plt.xlim([x_begin, x_end])
        plt.tight_layout()
        plt.show()


def plot_errors_from_time(solutions, analytical, t):
    plt.figure(figsize=(10, 6))

    for name, sol in solutions.items():
        err = np.array([max_abs_error(sol[i], analytical[i]) for i in range(len(t))])
        plt.plot(t, err, label=name)

    plt.xlabel('time')
    plt.ylabel('max abs error')
    plt.title('Максимальная ошибка численных схем во времени')
    plt.legend()
    plt.grid()
    plt.show()


def plot_3d_solutions(solutions, analytical, x, t):
    T, X = np.meshgrid(t, x, indexing='ij')

    for name, sol in solutions.items():
        fig = plt.figure(figsize=(12, 8))
        ax = fig.add_subplot(111, projection='3d')
        surf = ax.plot_surface(X, T, sol, cmap='viridis', alpha=0.8,
                               linewidth=0, antialiased=True)
        ax.set_xlabel('x')
        ax.set_ylabel('t')
        ax.set_zlabel('U(x,t)')
        ax.set_title(f'3D график решения: {name} схема')
        fig.colorbar(surf, ax=ax, shrink=0.5, aspect=5)
        plt.show()

    fig = plt.figure(figsize=(12, 8))
    ax = fig.add_subplot(111, projection='3d')
    surf = ax.plot_surface(X, T, analytical, cmap='plasma', alpha=0.8,
                           linewidth=0, antialiased=True)
    ax.set_xlabel('x')
    ax.set_ylabel('t')
    ax.set_zlabel('U(x,t)')
    ax.set_title('3D график аналитического решения')
    fig.colorbar(surf, ax=ax, shrink=0.5, aspect=5)
    plt.show()


def plot_final_time_errors_abs(solutions, analytical, x, t):
    final_time_idx = -1
    plt.figure(figsize=(12, 8))

    for name, sol in solutions.items():
        abs_error = np.abs(sol[final_time_idx] - analytical[final_time_idx])
        plt.plot(x, abs_error, label=f'{name} схема', linewidth=2)

    plt.xlabel('x', fontsize=12)
    plt.ylabel('Абсолютная ошибка |U_num(x) - U_ana(x)|', fontsize=12)
    plt.title(f'Абсолютные ошибки численных схем на конечном временном срезе t={t[final_time_idx]:.4f}', fontsize=14)
    plt.legend(fontsize=12)
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.show()


plot_scheme_evolution_separate_windows(solutions, x_ana, t_ana)
plot_errors_from_time(solutions, analytical, t_ana)
plot_3d_solutions(solutions, analytical, x_ana, t_ana)
plot_final_time_errors_abs(solutions, analytical, x_ana, t_ana)
