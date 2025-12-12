import numpy as np
from scipy import optimize


def search(line):
    split = line.split(" ")
    target = np.array([int(x) for x in split[-1].strip("\n{}").split(",")])
    m, n = len(target), len(split) - 2

    buttons = np.zeros((m, n), dtype=np.int32)
    for i, button in enumerate(split[1:-1]):
        for b in button.strip("()").split(","):
            buttons[int(b), i] = 1

    bounds = optimize.Bounds(0, np.inf)
    integrality = np.array([True] * n)
    constraints = optimize.LinearConstraint(A=buttons, lb=target, ub=target)

    result = optimize.milp(
        c=np.ones(n),
        constraints=constraints,
        integrality=integrality,
        bounds=bounds,
    )

    return sum(result.x)


def main():
    with open("input.in") as f:
        lines = f.readlines()

    # print(search(lines[0]))

    # total = 0
    # for line in tqdm(lines):
    # for line in lines:
    # total += search(line)
    # print(total)

    print(sum(search(line) for line in lines))


if __name__ == "__main__":
    main()
