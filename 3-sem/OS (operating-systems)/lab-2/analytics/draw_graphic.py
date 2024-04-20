import matplotlib.pyplot as plt
import csv
import getpass

def main():
    username = getpass.getuser()

    X = []
    Y = []

    with open("/home/" + username + "/MAI_OS/2_Lab/data_files/metrics.txt", "r") as metrics_file :
        plotting = csv.reader(metrics_file, delimiter=' ')

        for ROWS in plotting:
            X.append(float(ROWS[0]))
            Y.append(float(ROWS[1]))

    plt.plot(X, Y)
    plt.title('Time of threads stat')
    plt.xlabel('Number of threads')
    plt.ylabel('Time [ms]')
    plt.grid()
    plt.show()


if __name__ == "__main__" :
    main()
