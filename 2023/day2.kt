class Game(private var record: List<List<Int>>) {
    constructor(line: String) : this(listOf()) {
        val colors = listOf("red", "green", "blue")
        record = line.split(';').map { l ->
            colors.map { color ->
                "\\d+ $color".toRegex().find(l)?.value?.split(' ')?.get(0)?.toInt() ?: 0
            }
        }
    }
    private fun possibleSet(set: List<Int>, total: List<Int>) =
        set.zip(total) { a, b -> a <= b }.all { it }

    fun possible(total: List<Int>) =
        record.all { possibleSet(it, total) }

    fun power() =
        (0..2).map { idx ->
            record.maxOfOrNull { it[idx] }!!
        }.reduce(Int::times)

}

fun main() {
    fun partOne() {
        val amount = listOf(12, 13, 14)
        val sum = readInputs("day2.in").map {
            Game(it)
        }.mapIndexed { idx, game ->
            (idx + 1) * game.possible(amount).toInt()
        }.sum()
        println(sum)
    }

    fun partTwo() {
        val sum = readInputs("day2.in").map {
            Game(it).power()
        }.sum()
        println(sum)
    }

    partOne()
    partTwo()
}