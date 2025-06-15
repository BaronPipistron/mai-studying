from random import random
from math import exp

import numpy as np
import matplotlib.pyplot as plt


ALPHA = 0.75
COUNT_POINTS = int(1e4 - 1)
EPS = 0.0001


def plot(x, y, iter):
    plt.plot(x, y, linestyle='-', color=(random(), random(), random()), label=f"{iter} итераций")


def x0(t):
    return 0


if __name__ == '__main__':
    ts = np.linspace(0, 1, COUNT_POINTS)
    prevX = dict()
    for i in range(COUNT_POINTS):
        prevX[i] = x0(ts[i])

    iter = 1
    while (True):
        curX = dict()
        isPlot = iter > 4

        # Строим функцию
        for i in range(COUNT_POINTS):
            t = i / (COUNT_POINTS - 1)
            prevX_0 = prevX[0]
            prevX_1 = prevX[COUNT_POINTS - 1]

            if 0 <= i < COUNT_POINTS / 5:
                curX[i] = (1 / 4) * prevX[5 * i]
            elif COUNT_POINTS / 5 <= i < 2 * COUNT_POINTS / 5:
                curX[i] = (-1 / 4) * (prevX[5 * i - COUNT_POINTS] - prevX_0 - prevX_1)
            elif 2 * COUNT_POINTS / 5 <= i < 3 * COUNT_POINTS / 5:
                curX[i] = 27 * (exp(-abs(t - 0.5)) - exp(-1 / 10)) + (1 / 4) * prevX_0
            elif 3 * COUNT_POINTS / 5 <= i < 4 * COUNT_POINTS / 5:
                curX[i] = (-1 / 4) * (prevX[4 * COUNT_POINTS - 5 * i] - prevX_1 - prevX_0)
            elif 4 * COUNT_POINTS / 5 <= i < COUNT_POINTS:
                curX[i] = (1 / 4) * prevX[5 * COUNT_POINTS - 5 * i]

        if (isPlot and iter == 30):
            plot(ts, curX.values(), iter)

        # Расстояние
        rho = 0
        for i in range(COUNT_POINTS):
            rho = max(rho, abs(curX[i] - x0(ts[i])))

        # Погрешность
        curEps = ALPHA ** iter / (1 - ALPHA) * rho

        if (curEps < EPS):
            if (not isPlot):
                pass
                #plot(ts, curX.values(), iter)
            break

        iter += 1
        prevX = curX

    print(f"{iter} iterations")

    plt.title(f"Неподвижная точка (точность {EPS})")
    plt.legend()

    plt.grid(True)
    plt.show()