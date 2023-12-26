fun main() {
    data class Instruction(val dir: Int, val n: Int)
    data class Point(val row: Int, val col: Int)

    fun toDir(c: Char) = when (c) {
        'R' -> 0
        'D' -> 1
        'L' -> 2
        else -> 3
    }

    fun getInstructions1(inputs: List<String>) = inputs.map {
        val split = it.split(' ')
        Instruction(toDir(split[0][0]), split[1].toInt())
    }

    fun fromHex(s: String): Int {
        var x = 0
        var place = 1
        var digits = listOf('0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f')
        for (i in 1..<s.length - 1) {
            x += digits.indexOf(s[s.length - i]) * place
            place *= 16
        }
        return x
    }

    fun getInstructions2(inputs: List<String>) = inputs.map {
        val hex = it.split(' ')[2]
        val dir = hex[hex.length - 2] - '0'
        val n = fromHex(hex.dropLast(2).drop(1))
        Instruction(dir, n)
    }

    fun drawPath(instructions: List<Instruction>): List<Point> {
        val directions = listOf(Point(0, 1), Point(1, 0), Point(0, -1), Point(-1, 0))
        var cur = Point(0, 0)
        val out = mutableListOf(cur)

        for (instruct in instructions) {
            val dir = directions[instruct.dir]
            for (i in 0..<instruct.n) {
                cur = Point(cur.row + dir.row, cur.col + dir.col)
                out.add(cur)
            }
        }

        return out
    }

    fun toIndex(p: Point, width: Int) = p.row * width + p.col

    fun find(ds: MutableList<Int>, a: Int): Int {
        while (ds[a] >= 0) {
            val out = find(ds, ds[a])
            ds[a] = out
            return out
        }
        return a
    }

    fun union(ds: MutableList<Int>, a: Int, b: Int) {
        // union by size (smaller into larger)
        val keyA = find(ds, a)
        val keyB = find(ds, b)

        if (keyA == keyB) return

        if (ds[keyA] < ds[keyB]) {
            ds[keyB] += ds[keyA]
            ds[keyA] = keyB
        } else {
            ds[keyA] += ds[keyB]
            ds[keyB] = keyA
        }
    }

    fun outBounds(p: Point, w: Int, h: Int) = !inBounds(p.row, 0, h - 1) || !inBounds(p.col, 0, w - 1)

    fun solve(instructions: List<Instruction>): Int {
        val path = drawPath(instructions)
        val minRow = path.minBy { it.row }.row
        val maxRow = path.maxBy { it.row }.row
        val minCol = path.minBy { it.col }.col
        val maxCol = path.maxBy { it.col }.col
        val height = maxRow - minRow + 1
        val width = maxCol - minCol + 1

        // place the path in a rectangular grid with one unit of buffer
        val bufHeight = height + 2
        val bufWidth = width + 2
        val walls = path.map { point ->
            Point(point.row + 1 - minRow, point.col + 1 - minCol)
        }

        // find connected components (add all neighbors to same component excluding walls)
        val ds = MutableList(bufHeight * bufWidth) { -1 }

        // connect walls
        val wallIndex = toIndex(walls[0], bufWidth)
        for (point in walls) union(ds, wallIndex, toIndex(point, bufWidth))

        // connect everything else
        val neighbor = listOf(Point(1, 0), Point(0, 1), Point(-1, 0), Point(0, -1))
        for (row in 0..<bufHeight) {
            for (col in 0..<bufWidth) {
                val cur = Point(row, col)
                val curIndex = toIndex(cur, bufWidth)
                if (find(ds, curIndex) == find(ds, wallIndex)) continue
                for (n in neighbor) {
                    val p = Point(row + n.row, col + n.col)
                    if (!outBounds(p, bufWidth, bufHeight) && find(ds, toIndex(p, bufWidth)) != find(ds, wallIndex))
                        union(ds, toIndex(p, bufWidth), curIndex)

                }
            }
        }

        // count interior by exclusion
        val sol = ds.size + ds[find(ds, 0)]
        return sol
    }

    val inputs = readInputs("day18.in")

    fun partOne() {
        val instructions = getInstructions1(inputs)
        val sol = solve(instructions)
        println(sol)
    }

    fun partTwo() {
        val instructions = getInstructions2(inputs)
        val sol = solve(instructions)
        println(sol)
    }

    partOne()
    partTwo()
}