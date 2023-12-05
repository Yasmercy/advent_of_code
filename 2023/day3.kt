import kotlin.math.abs

val symbols = "[$&@+#=/\\-%*]".toRegex()
val digits = "\\d+".toRegex()

data class Location(val row: Int, val col: Int)
data class Symbol(val loc: Location, val sym: String)

fun getSymbols(input: List<String>) =
    input.flatMapIndexed { idx, line ->
        symbols.findAll(line).map { sym ->
            Symbol(Location(idx, sym.range.first), sym.value)
        }
    }

fun getNumbers(input: List<String>) =
    input.flatMapIndexed { idx, line ->
        digits.findAll(line).map { n ->
            Symbol(Location(idx, n.range.first), n.value)
        }
    }

fun isAdjacent(a: Location, b: Location) =
    abs(a.row - b.row) < 2 && abs(a.col - b.col) < 2

fun isAdjacent(a: Location, b: Symbol) =
    (b.loc.col..<(b.loc.col + b.sym.length)).map { col ->
        isAdjacent(a, Location(b.loc.row, col))
    }.any { it }

fun findAdjacent(symbols: List<Symbol>, numbers: List<Symbol>) =
    symbols.associateWith { sym ->
        numbers.filter { n ->
            isAdjacent(sym.loc, n)
        }
    }

fun main() {
    val input = readInputs("day3.in")
    val symbols = getSymbols(input)
    val numbers = getNumbers(input)
    val map = findAdjacent(symbols, numbers)

    fun partOne() {
        val sum = map.values.sumOf {
            it.sumOf { num -> num.sym.toInt() }
        }
        println(sum)
    }

    fun partTwo() {
        val sum = map.filter {
            it.key.sym == "*" && it.value.size == 2
        }.values.sumOf {
            it[0].sym.toInt() * it[1].sym.toInt()
        }
        println(sum)
    }

    partOne()
    partTwo()
}