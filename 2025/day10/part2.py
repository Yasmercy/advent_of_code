import numpy as np


def search(line):
    split = line.split(" ")
    target = [int(x) for x in split[-1].strip("\n{}").split(",")]
    size = len(target)

    buttons = []
    for i in range(1, len(split) - 1):
        indices = [int(x) for x in split[i].strip(")(").split(",")]
        tmp = np.zeros(size)
        tmp[indices] = 1
        buttons.append(tmp)
    buttons = np.array(buttons)

    reachable = np.array([np.zeros(size)])
    steps = 0
    while not np.all(reachable == target, axis=1).any():
        steps += 1
        # print(steps, reachable.shape)

        # numpy broadcasting to do this fast
        pairwise = reachable[:, None, :] + buttons[None, :, :]
        reachable = pairwise.reshape(-1, size)
        reachable = np.unique(reachable, axis=0)
        reachable = reachable[np.all(reachable <= target, axis=1)]

        # dist = np.min(np.max(target - reachable, axis=1))
        # print(dist)

    return steps


def main():
    with open("input.in") as f:
        lines = f.readlines()

    # print(search(lines[0]))

    # total = 0
    # for line in tqdm(lines):
    # for line in lines:
    # total += search(line)
    # print(total)

    print(sum(search(line) for line in lines))


if __name__ == "__main__":
    main()
