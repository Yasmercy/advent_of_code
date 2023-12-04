import kotlin.math.pow

fun getWinnings(card: String) = card.split(':')[1].split('|').map {
        "\\d+".toRegex().findAll(it).map { res -> res.value.toInt() }
    }

fun getCardCount(card: String): Int {
    val winnings = getWinnings(card)
    val good = winnings[0]
    val have = winnings[1]
    return have.filter {
        good.contains(it)
    }.toList().size
}

fun getCardScore(card: String) = 2.0.pow(getCardCount(card) - 1).toInt()

fun accumulateCards(acc: MutableList<Int>, counts: List<Int>, index: Int) {
    if (index == acc.size - 1)
        return
    for (i in 1..counts[index]) {
        acc[index + i] += acc[index]
    }
    accumulateCards(acc, counts, index + 1)
}
fun main() {
    fun partOne(input: List<String>) {
        val sum = input.sumOf { card ->
            getCardScore(card)
        }
        println(sum)
    }

    fun partTwo(input: List<String>) {
        val counts = input.map {card -> getCardCount(card)}
        val acc = MutableList(input.size) { 1 }
        accumulateCards(acc, counts, 0)
        println(acc.sum())
    }

    partOne(readInputs("day4.in"))
    partTwo(readInputs("day4.in"))
}