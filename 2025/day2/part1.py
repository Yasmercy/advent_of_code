from math import ceil, log10


def is_repeat(n):
    digits = ceil(log10(n))
    if digits % 2 != 0:
        return False

    half = digits // 2
    x, y = n // 10**half, n % 10**half
    return x == y


def main():
    with open("input.in") as f:
        lines = f.readlines()

    intervals = [interval.split("-") for interval in lines[0].strip().split(",")]
    ans = {i for x, y in intervals for i in range(int(x), int(y) + 1) if is_repeat(i)}

    print(sum(ans))


if __name__ == "__main__":
    main()
