import numpy as np


def count_adj(padded):
    padded = np.pad(np.array(padded, dtype=np.int64), 1, constant_values=0)

    paper = padded[1:-1, 1:-1]
    nbrs = (
        padded[2:, 1:-1]
        + padded[:-2, 1:-1]
        + padded[1:-1, 2:]
        + padded[1:-1, :-2]
        + padded[2:, 2:]
        + padded[2:, :-2]
        + padded[:-2, 2:]
        + padded[:-2, :-2]
    )

    return np.sum((nbrs < 4) & paper)


def main():
    with open("input.in") as f:
        lines = [[c == "@" for c in line.strip()] for line in f.readlines()]

    ans = count_adj(lines)
    print(ans)


if __name__ == "__main__":
    main()
