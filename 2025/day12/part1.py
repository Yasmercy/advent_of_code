import itertools as it

import numpy as np


def get_shapes(rows):
    s1 = np.array([list(r) for r in rows[1:]]) == "#"
    s2 = np.rot90(s1)
    s3 = np.rot90(s2)
    s4 = np.rot90(s3)
    s5 = np.flip(s1, axis=0)
    s6 = np.flip(s2, axis=0)
    s7 = np.flip(s3, axis=0)
    s8 = np.flip(s4, axis=0)

    return [s1, s2, s3, s4, s5, s6, s7, s8]


def check_feasible(m, n, reqs, bitmasks):
    possible = (m - (m % 3)) * (n - (n % 3)) >= sum(reqs) * 9
    impossible = sum(r * sum(b[0].ravel()) for r, b in zip(reqs, bitmasks)) > m * n
    assert possible or impossible
    return possible


def main():
    with open("input.in") as f:
        lines = [line.strip() for line in f.readlines()]

    chunks = [list(group) for k, group in it.groupby(lines, key=bool) if k]
    shapes = [get_shapes(row) for row in chunks[:-1]]

    count = 0
    for line in chunks[-1]:
        dims, reqs = line.split(":")
        m, n = [int(x) for x in dims.strip().split("x")]
        reqs = [int(x) for x in reqs.strip().split(" ")]

        if check_feasible(m, n, reqs, shapes):
            count += 1

    print(count)


if __name__ == "__main__":
    main()
