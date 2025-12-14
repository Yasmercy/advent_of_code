from functools import partial

import numpy as np


def remove_reachable(paper):
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

    mask = (nbrs < 4) & paper
    removed = np.sum(mask)
    paper &= 1 - mask
    return removed


def main():
    with open("input.in") as f:
        lines = [[c == "@" for c in line.strip()] for line in f.readlines()]

    paper = np.array(lines, dtype=np.int8)
    removed = sum(iter(partial(remove_reachable, paper), 0))
    print(removed)


if __name__ == "__main__":
    main()
