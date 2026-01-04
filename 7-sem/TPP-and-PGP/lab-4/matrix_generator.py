import sys
import random

def main():
    n = 100
    rng = random.Random(123456)  # фиксированный seed для повторяемости

    # сначала размер матрицы
    print(n)

    for i in range(n):
        row_vals = []
        for j in range(n):
            if j < i:
                # ниже диагонали — нули
                val = 0.0
            elif j == i:
                # диагональ точно ненулевая (определитель != 0)
                val = rng.uniform(0.5, 1.5)
            else:
                # выше диагонали — какие-то умеренные значения
                val = rng.uniform(-0.5, 0.5)
            row_vals.append(f"{val:.10e}")  # формат как у std::scientific
        sys.stdout.write(" ".join(row_vals) + "\n")

if __name__ == "__main__":
    main()
