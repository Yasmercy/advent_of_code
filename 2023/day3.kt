fun Char.isSymbol() = !this.isDigit() && this != '.'

data class Numb(val start: Int, val end: Int, val row: Int)

fun getNumbs(line: String, row: Int): List<Numb> {
    val mask = line.map {it.isDigit()}


    return out
}


fun main() {
    fun partOne(input: List<String>) {
        val symMask = getSymMask(input)
        val adjMask = getAdjMask(symMask)
        println(getDigits(input[0], 0, adjMask))
//        println(adjMask)

    }

    partOne(readInputs("day3.in"))
}