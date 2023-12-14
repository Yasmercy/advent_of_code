
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

    fun moveUp(map: MutableMap<Loc, Int>, pos: Loc) {
        if (!map.contains(pos) || map[pos] != 2)
            return
        var best = pos.row
        for (row in (0..<pos.row).reversed()) {
            if (!map.contains(Loc(row, pos.col)))
                best = row
            else
                break
        }
        map.remove(pos)
        map[Loc(best, pos.col)] = 2
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

    fun partOne() {
        val inputs = readInputs("day14.in")
        val height = inputs.size
        val width = inputs[0].length
        val grid = getGrid(inputs)

        for (r in 0..height) {
            for (c in 0..width) {
                moveUp(grid, Loc(r, c))
            }
        }

        val sol = grid.filterValues {it == 2 }.map {
            height - it.key.row
        }.sum()
        println(sol)
    }

    partOne()
}