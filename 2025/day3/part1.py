import numpy as np


def max_joltage(line):
    pos1 = np.argmax(line[:-1])
    digit1 = line[pos1]
    digit2 = np.max(line[pos1 + 1 :])

    return digit1 * 10 + digit2


def main():
    with open("input.in") as f:
        lines = [[int(x) for x in line.strip()] for line in f.readlines()]

    ans = sum(max_joltage(line) for line in lines)
    print(ans)


if __name__ == "__main__":
    main()
