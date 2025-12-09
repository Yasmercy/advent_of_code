import itertools as it
from enum import Enum

from more_itertools import sliding_window


class Direction(Enum):
    N = 1
    E = 2
    S = 3
    W = 4


def get_dir(x1, y1, x2, y2):
    if y1 == y2:
        if x1 < x2:
            return Direction.E
        if x1 > x2:
            return Direction.W
    if x1 == x2:
        if y1 < y2:
            return Direction.S
        if y1 > y2:
            return Direction.N


def cw(dir, r=1):
    for _ in range(r):
        if dir == Direction.N:
            dir = Direction.E
        elif dir == Direction.E:
            dir = Direction.S
        elif dir == Direction.S:
            dir = Direction.W
        elif dir == Direction.W:
            dir = Direction.N
    return dir


def shift(x, y, dir, r=1):
    if dir == Direction.N:
        return x, y - r
    elif dir == Direction.E:
        return x + r, y
    elif dir == Direction.S:
        return x, y + r
    elif dir == Direction.W:
        return x - r, y


def cut_rect(x1, y1, x2, y2, xe1, ye1, xe2, ye2):
    """credits to @edzdez"""
    xl, xr = min(x1, x2), max(x1, x2)
    yb, yt = min(y1, y2), max(y1, y2)

    if xe1 == xe2 and xl < xe1 and xe1 < xr:
        yeb, yet = min(ye1, ye2), max(ye1, ye2)
        return not (yet <= yb or yeb >= yt)
    elif ye1 == ye2 and yb < ye1 and ye1 < yt:
        xel, xer = min(xe1, xe2), max(xe1, xe2)
        return not (xer <= xl or xel >= xr)
    return False


def main():
    with open("input.in") as f:
        lines = f.readlines()

    points = []
    for line in lines:
        x, y = line.split(",")
        points.append((int(x), int(y)))
    points.append(points[0])

    boundary = []
    for p, q in sliding_window(points, 2):
        dir = get_dir(*p, *q)
        exterior = cw(dir, r=3)

        p1 = shift(*p, exterior)
        q1 = shift(*shift(*q, exterior), dir, r=-1)
        boundary.append((p1, q1))

    best = max(
        (abs(x1 - x2) + 1) * (abs(y1 - y2) + 1)
        for (x1, y1), (x2, y2) in it.combinations(points, 2)
        if not any(
            cut_rect(x1, y1, x2, y2, xe1, ye1, xe2, ye2)
            for ((xe1, ye1), (xe2, ye2)) in boundary
        )
    )
    print(best)


if __name__ == "__main__":
    main()
