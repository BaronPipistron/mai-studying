import csv
import sys
import matplotlib.pyplot as plt


def main(path: str):
    ranks, freqs, preds = [], [], []
    with open(path, newline="", encoding="utf-8") as f:
        r = csv.DictReader(f)
        for row in r:
            rank = int(row["rank"])
            freq = float(row["freq"])
            pred = float(row["zipf_pred"])
            ranks.append(rank)
            freqs.append(freq)
            preds.append(pred)

    plt.figure()
    plt.xscale("log")
    plt.yscale("log")
    plt.plot(ranks, freqs, label="corpus")
    plt.plot(ranks, preds, label="Zipf C/r")
    plt.xlabel("rank (log)")
    plt.ylabel("frequency (log)")
    plt.legend()
    plt.title("Zipf law")
    plt.show()


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python plot_zipf.py zipf.csv")
        sys.exit(1)
    main(sys.argv[1])
