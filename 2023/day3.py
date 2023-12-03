import string
import itertools as it

def isSymbol(c: str):
    return not c in string.digits and not c == '.'

def getInput() -> list[str]:
    with open("day3.in") as f:
        return [line.strip() for line in f.readlines()]

def multisplit(s: str, delims: list[str]):
    s = [s]
    for delim in delims:
        s = it.chain(*[x.split(delim) for x in s])
    return s

def getSymbols(input: list[str]) -> set[tuple[int, int]]:
    out = set()
    for r, row in enumerate(input):
        for c, char in enumerate(row):
            if isSymbol(char):
                out.add((r, c))
    return out

def getDigitStarts(line: str) -> list[int]:
    digits = [x for x in multisplit(line.strip(), "*$&@+#=/-%*.")]
    digits = [x for x in digits if x]
    starts = []
    cleared = 0
    for digit in digits:
        idx = line.find(digit)
        starts.append(idx + cleared)
        diff = idx + len(str(digit))
        line = line[diff:]
        cleared += diff
    return starts

def isAdjacent(row: int, col: int, symbols: set[tuple[int, int]]) -> bool:
    for r in range(row - 1, row + 2):
        for c in range(col - 1, col + 2):
            if (r, c) in symbols:
                return True
    return False

def isAdjacentNum(start: int, end: int, row: int, symbols: set[tuple[int, int]]) -> bool:
    return any(isAdjacent(row, col, symbols) for col in range(start, end))

def getDigitsAdjacent(line: str, row: int, symbols: set[tuple[int, int]]) -> list[int]:
    digits = [x for x in multisplit(line.strip(), "*$&@+#=/-%*.")]
    digits = [x for x in digits if x]
    starts = getDigitStarts(line)
    indices = [i for i, c in enumerate(starts) if isAdjacentNum(c, c + len(str(digits[i])), row, symbols)]
    return [int(digits[i]) for i in indices]

def main():
    input = getInput()
    symbols = getSymbols(input)
    sol = sum([sum(getDigitsAdjacent(input[r], r, symbols)) for r in range(len(input))])
    print(sol)

main()
