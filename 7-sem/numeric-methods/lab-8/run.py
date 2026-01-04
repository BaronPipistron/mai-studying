import numpy as np
import matplotlib.pyplot as plt


class ParabolicSolver2D:
    def __init__(self, a=1.0, b=1.0, mu=1.0,
                 Lx=np.pi / 2, Ly=np.pi, T=1.0):
        self.a = a
        self.b = b
        self.mu = mu
        self.Lx = Lx
        self.Ly = Ly
        self.T = T

    # ---------- аналитическое решение и правая часть ----------

    def analytical_solution(self, x, y, t):
        """
        U(x, y, t) = sin(x) * sin(y) * sin(mu * t)
        """
        return np.sin(x) * np.sin(y) * np.sin(self.mu * t)

    def source_term(self, x, y, t):
        """
        f(x, y, t) = sin x sin y ( mu cos(mu t) + (a + b) sin(mu t) )
        """
        return np.sin(x) * np.sin(y) * (
            self.mu * np.cos(self.mu * t)
            + (self.a + self.b) * np.sin(self.mu * t)
        )

    # ---------- граничные условия ----------

    def phi_left_x(self, y, t):
        # u(0, y, t) = 0
        return 0.0

    def phi_right_x(self, y, t):
        # u(pi/2, y, t) = sin y sin(mu t)
        return np.sin(y) * np.sin(self.mu * t)

    def phi_bottom_y(self, x, t):
        # u(x, 0, t) = 0
        return 0.0

    def psi_top_y(self, x, t):
        # u_y(x, pi, t) = - sin x sin(mu t)
        return -np.sin(x) * np.sin(self.mu * t)

    # ---------- сетка ----------

    def setup_grid(self, Nx, Ny, Nt):
        self.Nx, self.Ny, self.Nt = Nx, Ny, Nt
        self.hx = self.Lx / (Nx - 1)
        self.hy = self.Ly / (Ny - 1)
        self.tau = self.T / Nt

        self.x = np.linspace(0, self.Lx, Nx)
        self.y = np.linspace(0, self.Ly, Ny)
        self.t = np.linspace(0, self.T, Nt + 1)

        # u(:,:,n) — решение в момент t_n
        self.u = np.zeros((Nx, Ny, Nt + 1))      # ADI
        self.u_fs = np.zeros((Nx, Ny, Nt + 1))   # дробные шаги

    def apply_initial_conditions(self):
        # u(x, y, 0) = 0  (совпадает с аналитическим при t=0)
        for i in range(self.Nx):
            for j in range(self.Ny):
                val = self.analytical_solution(self.x[i], self.y[j], 0.0)
                self.u[i, j, 0] = val
                self.u_fs[i, j, 0] = val

    # ---------- метод переменных направлений (ADI) ----------

    def solve_adi(self):
        for n in range(self.Nt):
            t_mid = self.t[n] + 0.5 * self.tau

            # 1-й полу-шаг: implicit по x, explicit по y
            u_star = np.zeros((self.Nx, self.Ny))

            for j in range(self.Ny):
                yj = self.y[j]
                A = np.zeros((self.Nx, self.Nx))
                b = np.zeros(self.Nx)

                # внутренние узлы по x
                for i in range(1, self.Nx - 1):
                    A[i, i - 1] = self.a / self.hx ** 2
                    A[i, i] = -2.0 * self.a / self.hx ** 2 - 2.0 / self.tau
                    A[i, i + 1] = self.a / self.hx ** 2

                    if 1 <= j < self.Ny - 1:
                        laplace_y = (
                            self.u[i, j - 1, n]
                            - 2.0 * self.u[i, j, n]
                            + self.u[i, j + 1, n]
                        ) / self.hy ** 2
                    else:
                        laplace_y = 0.0

                    # ВАЖНО: знак у f(x,y,t) — минус
                    b[i] = (-2.0 / self.tau * self.u[i, j, n]
                            - self.b * laplace_y
                            - self.source_term(self.x[i], yj, t_mid))

                # x = 0 (Дирихле)
                A[0, 0] = 1.0
                b[0] = self.phi_left_x(yj, t_mid)

                # x = Lx (Дирихле)
                A[-1, -1] = 1.0
                b[-1] = self.phi_right_x(yj, t_mid)

                u_star[:, j] = np.linalg.solve(A, b)

            # 2-й полу-шаг: implicit по y, explicit по x
            for i in range(self.Nx):
                xi = self.x[i]
                A = np.zeros((self.Ny, self.Ny))
                b = np.zeros(self.Ny)

                for j in range(1, self.Ny - 1):
                    A[j, j - 1] = self.b / self.hy ** 2
                    A[j, j] = -2.0 * self.b / self.hy ** 2 - 2.0 / self.tau
                    A[j, j + 1] = self.b / self.hy ** 2

                    if 1 <= i < self.Nx - 1:
                        laplace_x = (
                            u_star[i - 1, j]
                            - 2.0 * u_star[i, j]
                            + u_star[i + 1, j]
                        ) / self.hx ** 2
                    else:
                        laplace_x = 0.0

                    # тут тоже f с минусом
                    b[j] = (-2.0 / self.tau * u_star[i, j]
                            - self.a * laplace_x
                            - self.source_term(xi, self.y[j], t_mid))

                # y = 0 (Дирихле)
                A[0, 0] = 1.0
                b[0] = self.phi_bottom_y(xi, t_mid)

                # y = Ly (Нейман: u_y = psi_top_y)
                A[-1, -1] = 3.0 / (2.0 * self.hy)
                A[-1, -2] = -4.0 / (2.0 * self.hy)
                A[-1, -3] = 1.0 / (2.0 * self.hy)
                b[-1] = self.psi_top_y(xi, t_mid)

                self.u[i, :, n + 1] = np.linalg.solve(A, b)

    # ---------- метод дробных шагов ----------

    def solve_fractional_steps(self):
        for n in range(self.Nt):
            t_mid = self.t[n] + 0.5 * self.tau

            # 1-й дробный шаг (по x)
            u_half = np.zeros((self.Nx, self.Ny))

            for j in range(self.Ny):
                yj = self.y[j]
                A = np.zeros((self.Nx, self.Nx))
                b = np.zeros(self.Nx)

                for i in range(1, self.Nx - 1):
                    A[i, i - 1] = self.a / (2.0 * self.hx ** 2)
                    A[i, i] = -self.a / self.hx ** 2 - 1.0 / self.tau
                    A[i, i + 1] = self.a / (2.0 * self.hx ** 2)

                    # снова f с минусом
                    b[i] = (-1.0 / self.tau * self.u_fs[i, j, n]
                            - 0.5 * self.source_term(self.x[i], yj, t_mid))

                A[0, 0] = 1.0
                b[0] = self.phi_left_x(yj, t_mid)

                A[-1, -1] = 1.0
                b[-1] = self.phi_right_x(yj, t_mid)

                u_half[:, j] = np.linalg.solve(A, b)

            # 2-й дробный шаг (по y)
            for i in range(self.Nx):
                xi = self.x[i]
                A = np.zeros((self.Ny, self.Ny))
                b = np.zeros(self.Ny)

                for j in range(1, self.Ny - 1):
                    A[j, j - 1] = self.b / (2.0 * self.hy ** 2)
                    A[j, j] = -self.b / self.hy ** 2 - 1.0 / self.tau
                    A[j, j + 1] = self.b / (2.0 * self.hy ** 2)

                    b[j] = (-1.0 / self.tau * u_half[i, j]
                            - 0.5 * self.source_term(xi, self.y[j], t_mid))

                A[0, 0] = 1.0
                b[0] = self.phi_bottom_y(xi, t_mid)

                A[-1, -1] = 3.0 / (2.0 * self.hy)
                A[-1, -2] = -4.0 / (2.0 * self.hy)
                A[-1, -3] = 1.0 / (2.0 * self.hy)
                b[-1] = self.psi_top_y(xi, t_mid)

                self.u_fs[i, :, n + 1] = np.linalg.solve(A, b)

    # ---------- погрешность ----------

    def compute_error(self, time_step, method="adi"):
        u_exact = np.zeros((self.Nx, self.Ny))
        for i in range(self.Nx):
            for j in range(self.Ny):
                u_exact[i, j] = self.analytical_solution(
                    self.x[i], self.y[j], self.t[time_step]
                )

        if method == "adi":
            u_num = self.u[:, :, time_step]
        else:
            u_num = self.u_fs[:, :, time_step]

        error = np.abs(u_num - u_exact)
        return error, u_exact


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


# ---------- 2D-графики ----------

def plot_2d_results(solver, time_steps):
    fixed_x = solver.x[len(solver.x) // 2]
    fixed_y = solver.y[len(solver.y) // 2]

    idx_x = np.argmin(np.abs(solver.x - fixed_x))
    idx_y = np.argmin(np.abs(solver.y - fixed_y))

    colors = ["blue", "red", "green", "orange", "purple"]

    plt.figure(figsize=(15, 10))

    # ===== ADI: срез по y =====
    

    # ===== ADI: срез по x =====
    plt.subplot(2, 2, 2)
    analytic_added = False
    for idx, t_step in enumerate(time_steps):
        if t_step > solver.Nt:
            continue
        color = colors[idx % len(colors)]
        t_val = solver.t[t_step]

        plt.plot(
            solver.x,
            solver.u[:, idx_y, t_step],
            color=color,
            linewidth=2,
            label=f"t = {t_val:.3f}",
        )
        u_an = solver.analytical_solution(solver.x, fixed_y, t_val)
        plt.plot(
            solver.x,
            u_an,
            linestyle="--",
            color=color,
            linewidth=3,
            alpha=0.9,
            zorder=5,
            label="аналитическое" if not analytic_added else None,
        )
        analytic_added = True

    plt.xlabel("x")
    plt.ylabel(f"u(x, y={fixed_y:.2f}, t)")
    plt.title("ADI: фиксированный y")
    plt.grid(True, alpha=0.3)
    plt.legend()

    # ===== Дробные шаги: срез по y =====


    # ===== Дробные шаги: срез по x =====
    plt.subplot(2, 2, 4)
    analytic_added = False
    for idx, t_step in enumerate(time_steps):
        if t_step > solver.Nt:
            continue
        color = colors[idx % len(colors)]
        t_val = solver.t[t_step]

        plt.plot(
            solver.x,
            solver.u_fs[:, idx_y, t_step],
            color=color,
            linewidth=2,
            label=f"t = {t_val:.3f}",
        )
        u_an = solver.analytical_solution(solver.x, fixed_y, t_val)
        plt.plot(
            solver.x,
            u_an,
            linestyle="--",
            color=color,
            linewidth=3,
            alpha=0.9,
            zorder=5,
            label="аналитическое" if not analytic_added else None,
        )
        analytic_added = True

    plt.xlabel("x")
    plt.ylabel(f"u(x, y={fixed_y:.2f}, t)")
    plt.title("Дробные шаги: фиксированный y")
    plt.grid(True, alpha=0.3)
    plt.legend()

    plt.tight_layout()


# ---------- 3D-графики (если нужно) ----------

def plot_3d_plotly(solver, time_steps, method="adi"):
    import plotly.graph_objects as go

    X, Y = np.meshgrid(solver.x, solver.y, indexing="ij")

    for t_step in time_steps:
        if t_step > solver.Nt:
            continue

        t_val = solver.t[t_step]

        if method == "adi":
            u_num = solver.u[:, :, t_step]
            method_title = "ADI"
        else:
            u_num = solver.u_fs[:, :, t_step]
            method_title = "Дробные шаги"

        u_exact = solver.analytical_solution(X, Y, t_val)

        fig = go.Figure()
        fig.add_trace(go.Surface(x=X, y=Y, z=u_num, name="численное"))
        fig.add_trace(
            go.Surface(
                x=X,
                y=Y,
                z=u_exact,
                showscale=False,
                opacity=0.6,
                name="аналитическое",
            )
        )
        fig.update_layout(
            title=f"{method_title}: t = {t_val:.3f}",
            scene=dict(
                xaxis_title="x",
                yaxis_title="y",
                zaxis_title="u(x,y,t)",
            ),
            margin=dict(l=0, r=0, b=0, t=30),
        )
        fig.show()


def plot_3d_analytic(solver, time_steps):
    """
    3D-графики аналитического решения U(x,y,t) = sin x sin y sin(mu t)
    для выбранных моментов времени.
    """
    import plotly.graph_objects as go

    X, Y = np.meshgrid(solver.x, solver.y, indexing="ij")

    for t_step in time_steps:
        if t_step > solver.Nt:
            continue

        t_val = solver.t[t_step]
        Z = solver.analytical_solution(X, Y, t_val)

        fig = go.Figure(
            data=[go.Surface(x=X, y=Y, z=Z)]
        )
        fig.update_layout(
            title=f"Аналитическое решение: t = {t_val:.3f}",
            scene=dict(
                xaxis_title="x",
                yaxis_title="y",
                zaxis_title="U(x,y,t)",
            ),
            margin=dict(l=0, r=0, b=0, t=30),
        )
        fig.show()



if __name__ == "__main__":
    # наборы по варианту:
    # 1) a = 1, b = 1, mu = 1
    # 2) a = 2, b = 1, mu = 1
    # 3) a = 1, b = 2, mu = 1
    # 4) a = 1, b = 1, mu = 2
    a = 1.0
    b = 1.0
    mu = 1.0
    T = 1.0


    solver = ParabolicSolver2D(a=a, b=b, mu=mu, T=T)

    # здесь играешься с Nx, Ny, Nt, чтобы изучать влияние h_x, h_y, tau
    Nx, Ny, Nt = 51, 51, 100
    solver.setup_grid(Nx, Ny, Nt)
    solver.apply_initial_conditions()

    print("Решение уравнения методом ADI...")
    solver.solve_adi()
    print("Готово.")

    print("Решение уравнения методом дробных шагов...")
    solver.solve_fractional_steps()
    print("Готово.")

    time_steps = [0, 25, 50, 75, 100]

    print("\nАнализ погрешности:")
    print("t\tADI max\tADI mean\tFS max\tFS mean")
    for t_step in time_steps:
        if t_step <= solver.Nt:
            err_adi, _ = solver.compute_error(t_step, method="adi")
            err_fs, _ = solver.compute_error(t_step, method="fs")
            print(
                f"{solver.t[t_step]:.3f}\t"
                f"{np.max(err_adi):.3e}\t{np.mean(err_adi):.3e}\t"
                f"{np.max(err_fs):.3e}\t{np.mean(err_fs):.3e}"
            )

    # 2D-графики
    plot_2d_results(solver, time_steps)

    # 3D-графики (если нужно):
    plot_3d_analytic(solver, [50])
    plot_3d_plotly(solver, [50], method="adi")
    plot_3d_plotly(solver, [50], method="fs")


    plt.show()
