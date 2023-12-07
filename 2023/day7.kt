data class Hand(val cards: List<Int>, val bid: Int)

fun getScore1(hand: Hand): Int {
    // 5 > 4 > 3-2 > 3 > 2-2 > 2 > high
    val counts = hand.cards.groupingBy { it }.eachCount().values.toList().sorted()
    return if (counts.last() == 5) 1
    else if (counts.last() == 4) 2
    else if (counts == listOf(2, 3)) 3
    else if (counts.last() == 3) 4
    else if (counts == listOf(1, 2, 2)) 5
    else if (counts.last() == 2) 6
    else 7
}

fun getScore2(hand: Hand): Int {
    val noJoke = hand.cards.filter { it != 1 }
    val joker = 5 - hand.cards.filter { it != 1 }.size
    val counts = noJoke.groupingBy { it }.eachCount().values.toList().sorted()

    return if (counts.isEmpty() || joker + counts.last() == 5) 1
    else if (joker + counts.last() == 4) 2
    else if (counts == listOf(2, 2) || counts == listOf(2, 3)) 3
    else if (joker + counts.last() == 3) 4
    else if (counts == listOf(1, 2, 2)) 5
    else if (joker + counts.last() == 2) 6
    else 7
}

fun compare(a: Hand, b: Hand, scorer: (Hand) -> Int): Int {
    val out = scorer(a) - scorer(b)
    return if (out == 0) {
        val diff = a.cards.zip(b.cards).dropWhile { (a, b) -> a == b }.first()
        diff.second - diff.first
    } else {
        out
    }
}

fun getRank(rank: Char) =
    mapOf(
        '2' to 2,
        '3' to 3,
        '4' to 4,
        '5' to 5,
        '6' to 6,
        '7' to 7,
        '8' to 8,
        '9' to 9,
        'T' to 10,
        'J' to 11,
        'Q' to 12,
        'K' to 13,
        'A' to 14
    )[rank]!!

fun getHand(line: String): Hand {
    val split = line.split(' ')
    val bid = split[1].toInt()
    val cards = split[0].map(::getRank)
    return Hand(cards, bid)
}

fun main() {
    val inputs = readInputs("day7.in")
    val hands = inputs.map(::getHand)

    fun partOne() {
        val sorted = hands.sortedWith { a, b -> compare(a, b, ::getScore1) }
        val score = sorted.withIndex().sumOf { (idx, hand) ->
            (hands.size - idx) * hand.bid
        }
        println(score)
    }

    fun partTwo() {
        val hands = hands.map { hand -> Hand(hand.cards.map { if (it == 11) 1 else it }, hand.bid) }
        val sorted = hands.sortedWith { a, b -> compare(a, b, ::getScore2) }
        val score = sorted.withIndex().sumOf { (idx, hand) ->
            (hands.size - idx) * hand.bid
        }
        println(score)
    }

    partOne()
    partTwo()
}