import functools
import operator

import numpy as np

ops = {
    "+": operator.add,
    "*": operator.mul,
}

identity = {
    "+": 0,
    "*": 1,
}


def main():
    inputs = []
    with open("sample.in") as f:
        lines = f.readlines()
        for line in lines[:-1]:
            inputs.append([int(x) for x in line.split()])

    operations = [(ops[x], identity[x]) for x in lines[-1].split()]
    inputs = np.array(inputs).T
    total = sum(
        functools.reduce(op, x, init)  #
        for x, (op, init) in zip(inputs, operations)
    )

    print(total)


if __name__ == "__main__":
    main()
