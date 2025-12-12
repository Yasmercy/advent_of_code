import itertools as it

import numpy as np
from scipy import optimize


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
    n_rots = 8
    n_shape = len(reqs)

    vars = [
        (s, r, i, j)
        for s in range(n_shape)
        for r in range(n_rots)
        for i in range(m - 2)
        for j in range(n - 2)
    ]

    collisions = [
        [
            (0 <= i - a <= 2 and 0 <= j - b <= 2) and bitmasks[s][r][i - a, j - b]
            for (s, r, a, b) in vars
        ]
        for i in range(m)
        for j in range(n)
    ]
    budgets = [[shape == s for (s, *_) in vars] for shape in range(n_shape)]

    bounds = optimize.Bounds(0, 1)
    integrality = np.ones(len(vars), dtype=np.bool)

    A = np.array(collisions + budgets)
    lb = np.array([0] * (m * n) + reqs)
    ub = np.array([1] * (m * n) + reqs)
    constraints = optimize.LinearConstraint(
        A=A,
        lb=lb,
        ub=ub,
    )

    result = optimize.milp(
        c=np.ones(len(vars)),
        constraints=constraints,
        integrality=integrality,
        bounds=bounds,
    )

    return result.status != 2


def main():
    with open("input.in") as f:
        lines = [line.strip() for line in f.readlines()]

    chunks = [list(group) for k, group in it.groupby(lines, key=bool) if k]
    shapes = [get_shapes(row) for row in chunks[:-1]]

    count = 0
    for line in chunks[-1]:
        # for line in [chunks[-1][2]]:
        dims, reqs = line.split(":")
        m, n = [int(x) for x in dims.strip().split("x")]
        reqs = [int(x) for x in reqs.strip().split(" ")]

        if check_feasible(m, n, reqs, shapes):
            count += 1

        print("hi")
        break

    print(count)


if __name__ == "__main__":
    main()
