from math import ceil

from more_itertools import chunked, sliding_window


def all_eq(arr):
    return all(x == y for x, y in sliding_window(arr, 2))


def is_repeat(n):
    digits = len(n)
    if digits == 1:
        return False
    return any(all_eq(chunked(n, d)) for d in range(1, ceil(digits / 2) + 1))


def main():
    with open("input.in") as f:
        lines = f.readlines()

    intervals = [interval.split("-") for interval in lines[0].strip().split(",")]
    ans = {
        i
        for x, y in intervals  #
        for i in range(int(x), int(y) + 1)  #
        if is_repeat(str(i))
    }

    print(sum(ans))


if __name__ == "__main__":
    main()
