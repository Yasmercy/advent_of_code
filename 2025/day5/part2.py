import itertools as it


def main():
    with open("input.in") as f:
        lines = [line.strip() for line in f.readlines()]

    # 0 is left range, 1 is right range, 2 is ingrediant
    ranges, ingrediants = [list(group) for k, group in it.groupby(lines, key=bool) if k]
    left = [(int(x.split("-")[0]), 0) for x in ranges]
    right = [(int(x.split("-")[1]), 2) for x in ranges]
    items = left + right

    # scanline
    items.sort()
    fresh, count, start = 0, 0, 0
    for x, y in items:
        if y == 0:
            if fresh == 0:
                start = x
            fresh += 1
        elif y == 2:
            fresh -= 1
            if fresh == 0:
                end = x
                count += end - start + 1

    print(count)


if __name__ == "__main__":
    main()
