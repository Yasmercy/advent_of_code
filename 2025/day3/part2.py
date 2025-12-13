import numpy as np


def max_joltage(line, digits):
    if digits == 1:
        return np.max(line)

    pos = np.argmax(line[: -digits + 1])
    digit = line[pos]
    recur = max_joltage(line[pos + 1 :], digits - 1)
    return digit * 10 ** (digits - 1) + recur


def main():
    with open("input.in") as f:
        lines = [[int(x) for x in line.strip()] for line in f.readlines()]

    ans = sum(max_joltage(line, 12) for line in lines)
    print(ans)


if __name__ == "__main__":
    main()
