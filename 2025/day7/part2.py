import functools


def find_all(string, token):
    row = set()
    start = 0
    while (start := string.find(token, start)) != -1:
        row.add(start)
        start = start + 1
    return row


splitters = []


@functools.lru_cache
def count_possibilities(beam, row):
    if row == len(splitters):
        return 1

    if beam in splitters[row]:
        a = count_possibilities(beam - 1, row + 1)
        b = count_possibilities(beam + 1, row + 1)
        return a + b

    return count_possibilities(beam, row + 1)


def main():
    with open("input.in") as f:
        lines = f.readlines()

    for line in lines:
        splitters.append(find_all(line, "^"))

    beam = lines[0].find("S")
    print(count_possibilities(beam, 0))


if __name__ == "__main__":
    main()
