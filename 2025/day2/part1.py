from math import ceil, log10


def count_lower(n):
    if n <= 10:
        return 0

    digits = ceil(log10(n))
    half = digits // 2
    even = 2 * half

    n = min(10**even - 1, n)
    x, y = n // 10**half, n % 10**half

    a = 10 ** (half - 1) - 1
    b = x - 1 + int(y >= x)
    return a + b


def main():
    with open("sample.in") as f:
        lines = f.readlines()

    intervals = [interval.split("-") for interval in lines[0].strip().split(",")]
    ans = [count_lower(int(y)) - count_lower(int(x) - 1) for x, y in intervals]

    print(sum(ans))


if __name__ == "__main__":
    main()
