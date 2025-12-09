import itertools as it


def main():
    with open("input.in") as f:
        lines = f.readlines()

    points = []
    for line in lines:
        x, y = line.split(",")
        points.append((int(x), int(y)))

    best = max(
        abs((x1 - x2 + 1) * (y1 - y2 + 1))
        for (x1, y1), (x2, y2) in it.combinations(points, 2)
    )

    print(best)


if __name__ == "__main__":
    main()
