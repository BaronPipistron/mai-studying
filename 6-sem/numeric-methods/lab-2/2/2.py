import matplotlib.pyplot as plt
import numpy as np

class NonlinearSystemSolver:
    def __init__(self, epsilon=1e-6, max_iter=100, x1_range=(0.5, 2.0), x2_range=(0.5, 2.0)):
        self.epsilon = epsilon
        self.max_iter = max_iter
        self.x1_range = x1_range
        self.x2_range = x2_range
        self.q = self._compute_q()

    def _compute_q(self):
        """Вычисляет константу Липшица q как максимум нормы матрицы Якоби на заданном отрезке"""
        n_points = 100
        x1_samples = np.linspace(*self.x1_range, n_points)
        x2_samples = np.linspace(*self.x2_range, n_points)

        max_q = 0
        for x1 in x1_samples:
            for x2 in x2_samples:
                try:
                    J = self.J_iterations([x1, x2])
                    current_q = np.max(np.sum(np.abs(J), axis=1))
                    if current_q > max_q:
                        max_q = current_q
                except:
                    continue  # Пропускаем точки, где матрица Якоби не определена

        if max_q == 0:
            raise ValueError("Не удалось вычислить q на заданном отрезке")

        print(f"Вычисленная константа сходимости q = {max_q:.6f}")
        return max_q

    @staticmethod
    def F(x):
        """Система уравнений"""
        x1, x2 = x[0], x[1]
        return np.array([
            x1 ** 2 - x2 ** 2 - 4,  # F1(x1,x2)
            x1 - np.exp(x2) + 2  # F2(x1,x2)
        ])

    @staticmethod
    def g1(x2):
        """Преобразование первого уравнения: x1 = g1(x2)"""
        return np.sqrt(x2 ** 2 + 4)

    @staticmethod
    def g2(x1):
        """Преобразование второго уравнения: x2 = g2(x1)"""
        return np.log(x1 + 2)

    @staticmethod
    def J_iterations(x):
        """Матрица Якоби для метода простых итераций"""
        x1, x2 = x[0], x[1]
        return np.array([
            [0, x2 / np.sqrt(x2 ** 2 + 4)],
            [1 / (x1 + 2), 0]
        ])

    @staticmethod
    def J_newton(x):
        """Матрица Якоби для метода Ньютона"""
        x1, x2 = x[0], x[1]
        return np.array([
            [2 * x1, -2 * x2],
            [1, -np.exp(x2)]
        ])

    def check_convergence(self, x0):
        """Проверка условия сходимости для метода простых итераций"""
        J = self.J_iterations(x0)
        norm = np.max(np.sum(np.abs(J), axis=1))
        if norm >= 1:
            raise ValueError(f"Условие сходимости нарушено: норма Якобиана = {norm:.4f} ≥ 1")
        return norm


class NonlinearSystemSolver:
    def __init__(self, epsilon=1e-6, max_iter=100, x1_range=(0.5, 2.0), x2_range=(0.5, 2.0)):
        self.epsilon = epsilon
        self.max_iter = max_iter
        self.x1_range = x1_range
        self.x2_range = x2_range
        self.q = self._compute_q()

    def _compute_q(self):
        """Вычисляет константу Липшица q как максимум нормы матрицы Якоби на заданном отрезке"""
        n_points = 100
        x1_samples = np.linspace(*self.x1_range, n_points)
        x2_samples = np.linspace(*self.x2_range, n_points)

        max_q = 0
        for x1 in x1_samples:
            for x2 in x2_samples:
                try:
                    J = self.J_iterations([x1, x2])
                    current_q = np.max(np.sum(np.abs(J), axis=1))
                    if current_q > max_q:
                        max_q = current_q
                except:
                    continue  # Пропускаем точки, где матрица Якоби не определена

        if max_q == 0:
            raise ValueError("Не удалось вычислить q на заданном отрезке")

        print(f"Вычисленная константа сходимости q = {max_q:.6f}")
        return max_q

    @staticmethod
    def F(x):
        """Система уравнений"""
        x1, x2 = x[0], x[1]
        return np.array([
            x1 ** 2 - x2 ** 2 - 4,  # F1(x1,x2)
            x1 - np.exp(x2) + 2  # F2(x1,x2)
        ])

    @staticmethod
    def g1(x2):
        """Преобразование первого уравнения: x1 = g1(x2)"""
        return np.sqrt(x2 ** 2 + 4)

    @staticmethod
    def g2(x1):
        """Преобразование второго уравнения: x2 = g2(x1)"""
        return np.log(x1 + 2)

    @staticmethod
    def J_iterations(x):
        """Матрица Якоби для метода простых итераций"""
        x1, x2 = x[0], x[1]
        return np.array([
            [0, x2 / np.sqrt(x2 ** 2 + 4)],
            [1 / (x1 + 2), 0]
        ])

    @staticmethod
    def J_newton(x):
        """Матрица Якоби для метода Ньютона"""
        x1, x2 = x[0], x[1]
        return np.array([
            [2 * x1, -2 * x2],
            [1, -np.exp(x2)]
        ])

    def check_convergence(self, x0):
        """Проверка условия сходимости для метода простых итераций"""
        J = self.J_iterations(x0)
        norm = np.max(np.sum(np.abs(J), axis=1))
        if norm >= 1:
            raise ValueError(f"Условие сходимости нарушено: норма Якобиана = {norm:.4f} ≥ 1")
        return norm

    def simple_iteration(self, x0, verbose=True):
        try:
            if x0[1] ** 2 + 4 < 0:  # Всегда истинно, оставляем для формальности
                raise ValueError("Начальное x2 приводит к отрицательному подкоренному выражению в g1(x2)")

            if x0[0] + 2 <= 0:
                raise ValueError(f"Начальное x1 = {x0[0]} приводит к неположительному аргументу логарифма в g2(x1)")

            # Проверка значения q
            if self.q >= 1:
                raise ValueError(f"Метод не сойдется: q = {self.q:.4f} ≥ 1")

        except ValueError as e:
            if verbose:
                print(f"Ошибка в начальном приближении: {e}")
            return None, 0, [], []

        if verbose:
            print(f"\nМетод простых итераций с q = {self.q:.6f}")
            print(f"Начальное приближение x0 = [{x0[0]}, {x0[1]}] входит в область определения")
            print("Теоретическая оценка погрешности после n итераций: |x_n - x*| ≤ q^n/(1-q) * |x1 - x0|")

        x = np.array(x0, dtype=float)
        history = [x.copy()]
        errors = []

        for iter_count in range(1, self.max_iter + 1):
            try:
                # Проверка области определения на каждой итерации
                if x[1] ** 2 + 4 < 0:
                    raise ValueError("Подкоренное выражение в g1(x2) стало отрицательным")
                if x[0] + 2 <= 0:
                    raise ValueError("Аргумент логарифма в g2(x1) стал неположительным")

                x_new = np.array([self.g1(x[1]), self.g2(x[0])])
                error = np.linalg.norm(x_new - x, np.inf)
                errors.append(error)

                if verbose:
                    theoretical_error = (self.q ** iter_count) / (1 - self.q) * errors[0]
                    print(f"Итерация {iter_count:3d}: x = [{x_new[0]:.8f}, {x_new[1]:.8f}]")
                    print(f"Ошибка: {error:.2e} | Теор. оценка: {theoretical_error:.2e}")

                if error < self.epsilon:
                    if verbose:
                        print(f"\nСходимость достигнута за {iter_count} итераций")
                    return x_new, iter_count, errors, history

                x = x_new
                history.append(x.copy())

            except ValueError as e:
                if verbose:
                    print(f"\nОшибка на итерации {iter_count}: {e}")
                return x, iter_count, errors, history

        if verbose:
            print(f"\nДостигнут максимум итераций ({self.max_iter})")
        return x, self.max_iter, errors, history

    def newton_method(self, x0, verbose=True):
        try:
            # Проверка системы уравнений
            F_val = self.F(x0)

            if x0[0] + 2 <= 0:
                raise ValueError(f"Начальное x1 = {x0[0]} приводит к неположительному аргументу логарифма")

            # Проверка матрицы Якоби
            J = self.J_newton(x0)
            det = np.linalg.det(J)
            if abs(det) < 1e-12:
                raise ValueError(f"Матрица Якоби вырождена в начальной точке (det(J) = {det:.2e})")

        except ValueError as e:
            if verbose:
                print(f"Ошибка в начальном приближении: {e}")
            return None, 0, [], []

        if verbose:
            print(f"\nМетод Ньютона")
            print(f"Начальное приближение x0 = [{x0[0]}, {x0[1]}] входит в область определения")
            print(f"Начальная невязка: F(x0) = [{F_val[0]:.6f}, {F_val[1]:.6f}]")

        x = np.array(x0, dtype=float)
        history = [x.copy()]
        errors = []

        for iter_count in range(1, self.max_iter + 1):
            try:
                # Проверка области определения перед вычислениями
                if x[0] + 2 <= 0:
                    raise ValueError(f"Аргумент логарифма стал неположительным (x1 = {x[0]})")

                F_val = self.F(x)
                J = self.J_newton(x)

                # Проверка определителя матрицы Якоби
                det = np.linalg.det(J)
                if abs(det) < 1e-12:
                    raise ValueError(f"Матрица Якоби вырождена (det(J) = {det:.2e})")

                dx = np.linalg.solve(J, -F_val)
                x_new = x + dx
                error = np.linalg.norm(dx, np.inf)
                errors.append(error)

                if verbose:
                    print(f"Итерация {iter_count}: x = [{x_new[0]:.8f}, {x_new[1]:.8f}]")
                    print(f"Ошибка: {error:.2e} | Абсолютная погрешность: [{F_val[0]:.2e}, {F_val[1]:.2e}]")

                if error < self.epsilon:
                    if verbose:
                        print(f"\nСходимость достигнута за {iter_count} итераций")
                    return x_new, iter_count, errors, history

                x = x_new
                history.append(x.copy())

            except ValueError as e:
                if verbose:
                    print(f"\nОшибка на итерации {iter_count}: {e}")
                return x, iter_count, errors, history

        if verbose:
            print(f"\nДостигнут максимум итераций ({self.max_iter})")
        return x, self.max_iter, errors, history

    @staticmethod
    def plot_system():
        """Визуализация системы уравнений"""
        x2_vals = np.linspace(0, 3, 400)
        x1_vals = np.linspace(0.01, 3, 400)

        plt.figure(figsize=(10, 6))
        plt.plot(np.sqrt(x2_vals ** 2 + 4), x2_vals, label=r'$x_1 = \sqrt{x_2^2 + 4}$')
        plt.plot(x1_vals, np.log(x1_vals + 2), label=r'$x_2 = \ln(x_1 + 2)$')
        plt.xlabel("x1")
        plt.ylabel("x2")
        plt.title("Графическое представление системы уравнений")
        plt.legend()
        plt.grid(True)
        plt.show()

    @staticmethod
    def plot_system():
        """Визуализация системы уравнений"""
        x2_vals = np.linspace(0, 3, 400)
        x1_vals = np.linspace(0.01, 3, 400)

        plt.figure(figsize=(10, 6))
        plt.plot(np.sqrt(x2_vals ** 2 + 4), x2_vals, label=r'$x_1 = \sqrt{x_2^2 + 4}$')
        plt.plot(x1_vals, np.log(x1_vals + 2), label=r'$x_2 = \ln(x_1 + 2)$')
        plt.xlabel("x1")
        plt.ylabel("x2")
        plt.title("Графическое представление системы уравнений")
        plt.legend()
        plt.grid(True)
        plt.show()


def main():  # проверка области определения начального приближения
    solver = NonlinearSystemSolver(epsilon=1e-6, max_iter=100, x1_range=(0.5, 2.0), x2_range=(0.5, 2.0))
    x0 = [2.25, 2.75]

    print("=" * 50)
    print(f"РЕШЕНИЕ СИСТЕМЫ НЕЛИНЕЙНЫХ УРАВНЕНИЙ")
    print("=" * 50)
    print(f"Начальное приближение: x1 = {x0[0]}, x2 = {x0[1]}")
    print(f"Точность: {solver.epsilon}")
    print(f"Максимальное число итераций: {solver.max_iter}")
    print("-" * 50)

    solver.plot_system()

    print("\n" + "=" * 20 + " МЕТОД ПРОСТЫХ ИТЕРАЦИЙ " + "=" * 20)
    sol_si, iter_si, errors_si, history_si = solver.simple_iteration(x0)

    print("\n" + "=" * 20 + " МЕТОД НЬЮТОНА " + "=" * 20)
    sol_newton, iter_newton, errors_newton, history_newton = solver.newton_method(x0)

    if sol_si is None or sol_newton is None:
        return

    print("\n" + "=" * 20 + " РЕЗУЛЬТАТЫ " + "=" * 20)
    print(f"{'Метод':<25} | {'x1':<15} | {'x2':<15} | {'Итерации':<10}")
    print("-" * 65)
    if sol_si is not None:
        print(f"{'Простой итерации':<25} | {sol_si[0]:.8f} | {sol_si[1]:.8f} | {iter_si:>10}")
    print(f"{'Ньютона':<25} | {sol_newton[0]:.8f} | {sol_newton[1]:.8f} | {iter_newton:>10}")
    print("=" * 65)


main()