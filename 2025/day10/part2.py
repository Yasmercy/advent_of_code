import numpy as np
import itertools as it

# from tqdm import tqdm


def button_to_int(button, base):
    return sum(base**x for x in button)


def counter_to_int(counter, base):
    return sum(c * base**i for i, c in enumerate(counter))


def prune(sum, target, base):
    if sum > target:
        return False

    while target > 0:
        r1, r2 = sum % base, target % base
        if r1 > r2:
            return False
        target = target // base
    return True


# def run_dp(target, buttons):
#     reachable = np.array([0])
#     steps = 0

#     while target not in reachable:
#         steps += 1
#         reachable = (reachable[:, None] + buttons).ravel()
#         reachable = np.unique(reachable)
#         reachable = reachable[reachable < target]
#         print(len(reachable))

#     return steps


def search(line):
    split = line.split(" ")
    counters = [int(x) for x in split[-1].strip("\n{}").split(",")]
    buttons = [
        [int(x) for x in split[i].strip(")(").split(",")]
        for i in range(1, len(split) - 1)
    ]

    base = max(counters) + 1
    buttons = np.array([button_to_int(button, base) for button in buttons])
    target = counter_to_int(counters, base)
    # steps = run_dp(target, buttons)

    return steps


def main():
    with open("sample.in") as f:
        lines = f.readlines()

    print(search(lines[0]))

    # total = 0
    # # for line in tqdm(lines):
    # for line in lines:
    #     total += search(line)
    # print(total)

    print(sum(search(line) for line in lines))


if __name__ == "__main__":
    main()
