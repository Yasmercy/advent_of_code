import numpy as np


def can_remove(paper):
    padded = np.pad(paper, 1, constant_values=0)
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

    return (nbrs < 4) & paper


def main():
    with open("input.in") as f:
        lines = [[c == "@" for c in line.strip()] for line in f.readlines()]

    paper = np.array(lines, dtype=np.int8)
    removed, number = 0, 1
    while number:
        remove = can_remove(paper)
        paper = paper & (1 - remove)
        number = np.sum(remove)
        removed += number

    print(removed)


if __name__ == "__main__":
    main()
