def main():
    with open("input.in") as f:
        lines = f.readlines()

    nums = [int(l.strip()[1:]) for l in lines]
    sign = [{"L": -1, "R": 1}[l[0]] for l in lines]

    pos = 50
    counter = 0
    for b, n in zip(sign, nums):
        pos = (pos + b * n) % 100
        if pos == 0:
            counter += 1

    print(counter)


if __name__ == "__main__":
    main()
