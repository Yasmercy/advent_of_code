fun main() {
    data class Loc(val row: Int, val col: Int)

    fun getGrid(input: List<String>): MutableMap<Loc, Int> {
        val out = mutableMapOf<Loc, Int>()
        for (row in input.indices) {
            for ((col, char) in input[row].withIndex()) {
                if (char == '#')
                    out[Loc(row, col)] = 1
                if (char == 'O')
                    out[Loc(row, col)] = 2
            }
        }
        return out
    }

    fun moveDir(map: MutableMap<Loc, Int>, pos: Loc, dr: Int, dc: Int, width: Int, height: Int) {
        if (!map.contains(pos) || map[pos] != 2)
            return

        var best = pos
        while (inBounds(best.row + dr, 0, height - 1) && inBounds(best.col + dc, 0, width - 1) &&
            !map.contains(Loc(best.row + dr, best.col + dc))
        ) {
            best = Loc(best.row + dr, best.col + dc)
        }

        map.remove(pos)
        map[best] = 2
    }

    fun moveGrid(map: MutableMap<Loc, Int>, dr: Int, dc: Int, width: Int, height: Int) {
        val r_range: IntProgression
        val c_range: IntProgression
        r_range = if (dr < 0) {
            0..<height
        } else {
            (0..<height).reversed()
        }
        c_range = if (dc < 0) {
            0..<width
        } else {
            (0..<width).reversed()
        }

        for (r in r_range) {
            for (c in c_range) {
                moveDir(map, Loc(r, c), dr, dc, width, height)
            }
        }
    }

    fun displayGrid(map: MutableMap<Loc, Int>, width: Int, height: Int) {
        for (r in 0..<height) {
            for (c in 0..<width) {
                val l = Loc(r, c)
                if (!map.contains(l))
                    print('.')
                else if (map[l] == 1)
                    print('#')
                else print('O')
            }
            println()
        }
    }

    fun getLoad(map: MutableMap<Loc, Int>, height: Int) =
        map.filterValues { it == 2 }.map {
            height - it.key.row
        }.sum()

    val inputs = readInputs("day14.in")
    val height = inputs.size
    val width = inputs[0].length

    fun partOne() {
        val grid = getGrid(inputs)
        moveGrid(grid, -1, 0, width, height)
        val sol = getLoad(grid, height)
        println(sol)
    }

    fun partTwo() {
        val n = 1000000000L
        val dirs = listOf(Pair(-1, 0), Pair(0, -1), Pair(1, 0), Pair(0, 1))
        val cache = mutableMapOf<Map<Loc, Int>, Long>()

        var index = 0L
        val grid = getGrid(inputs)

        while (index < n) {
            for (dir in dirs)
                moveGrid(grid, dir.first, dir.second, width, height)

            if (cache.contains(grid)) {
                val period = index - cache[grid]!!
                val cycles = (n - index) / period
                index += cycles * period
            }

            cache[grid] = index
            index += 1
        }

        val sol = getLoad(grid, height)
        displayGrid(grid, width, height)
        println(sol)
    }

    // partOne()
    partTwo()
}