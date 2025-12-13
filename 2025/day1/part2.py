def main():
    with open("input.in") as f:
        lines = f.readlines()

    nums = [int(line.strip()[1:]) for line in lines]
    sign = [{"L": -1, "R": 1}[line[0]] for line in lines]

    fwd = 50
    bck = 50
    counter = 0
    for b, n in zip(sign, nums):
        fwd1 = fwd + b * n
        bck1 = bck - b * n

        if b == 1:
            counter += abs(fwd1 // 100)
        else:
            counter += abs(bck1 // 100)

        fwd = fwd1 % 100
        bck = bck1 % 100

    print(counter)


if __name__ == "__main__":
    main()
