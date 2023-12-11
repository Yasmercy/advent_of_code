import kotlin.math.max
import kotlin.math.min

data class Loc(val row: Long, val col: Long)

fun getGalaxies(inputs: List<String>): List<Loc> {
    return inputs.flatMapIndexed { row, it ->
        it.mapIndexed { col, c ->
            if (c == '#') Loc(row.toLong(), col.toLong()) else Loc(-1, -1)
        }
    }.filterNot { it == Loc(-1, -1) }
}

fun getDistance(a: Loc, b: Loc, galaxies: List<Loc>, scale: Long): Long {
    val min_row = min(a.row, b.row)
    val max_row = max(a.row, b.row)
    val min_col = min(a.col, b.col)
    val max_col = max(a.col, b.col)

    val between_rows = (min_row..<max_row).filterNot {r ->
        galaxies.any {it.row == r}
    }
    val between_cols = (min_col..<max_col).filterNot {c ->
        galaxies.any {it.col == c}
    }

    return max_row - min_row + max_col - min_col + between_rows.size * (scale - 1L) + between_cols.size * (scale - 1L)
}

fun main() {
    val inputs = readInputs("day11.in")
    val galaxies = getGalaxies(inputs)

    fun partOne() {
        var sum = 0L
        for (i in galaxies.indices) {
            for (j in i..<galaxies.size) {
                sum += getDistance(galaxies[i], galaxies[j], galaxies, 2L)
            }
        }
        println(sum)
    }

    fun partTwo() {
        var sum = 0L
        for (i in galaxies.indices) {
            for (j in i..<galaxies.size) {
                sum += getDistance(galaxies[i], galaxies[j], galaxies, 1000000L)
            }
        }
        println(sum)
    }

    partOne()
    partTwo()
}