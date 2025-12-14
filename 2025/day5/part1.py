import itertools as it


def main():
    with open("input.in") as f:
        lines = [line.strip() for line in f.readlines()]

    # 0 is left range, 1 is right range, 2 is ingrediant
    ranges, ingrediants = [list(group) for k, group in it.groupby(lines, key=bool) if k]
    left = [(int(x.split("-")[0]), 0) for x in ranges]
    ingrediants = [(int(x), 1) for x in ingrediants]
    right = [(int(x.split("-")[1]), 2) for x in ranges]
    items = left + right + ingrediants

    # scanline
    items.sort()
    fresh = 0
    count = 0
    for _, y in items:
        if y == 0:
            fresh += 1
        elif y == 1 and fresh:
            count += 1
        elif y == 2:
            fresh -= 1

    print(count)


if __name__ == "__main__":
    main()
