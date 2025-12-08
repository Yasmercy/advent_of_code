from sortedcontainers import SortedSet


def find_all(string, token):
    row = SortedSet()
    start = 0
    while (start := string.find(token, start)) != -1:
        row.add(start)
        start = start + 1
    return row


def main():
    with open("input.in") as f:
        lines = f.readlines()

    beams = {lines[0].find("S")}
    splitters = []

    for line in lines:
        splitters.append(find_all(line, "^"))

    count = 0
    for cols in splitters:
        for col in cols:
            if col in beams:
                beams.add(col - 1)
                beams.add(col + 1)
                beams.remove(col)
                count += 1
        # print(beams)

    print(count)


if __name__ == "__main__":
    main()
