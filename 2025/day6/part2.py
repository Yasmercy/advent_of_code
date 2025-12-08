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
    with open("input.in") as f:
        lines = f.readlines()
        for line in lines:
            inputs.append(list(line.strip("\n"))[::-1])

    inputs = np.array(inputs, dtype=np.bytes_).T

    stack = []
    i = 0
    tot = 0
    while i < len(inputs):
        input = inputs[i]

        if chr(ord(input[-1])) in ops:
            c = chr(ord(input[-1]))
            stack.append(int(input[:-1].tobytes()))

            op, id = ops[c], identity[c]
            tot += functools.reduce(op, stack, id)

            stack = []
            i += 2

        else:
            stack.append(int(input.tobytes()))
            i += 1

    print(tot)


if __name__ == "__main__":
    main()
