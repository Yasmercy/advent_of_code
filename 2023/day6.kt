data class Race(val time: Long, val record: Long)

fun getRaces1(input: List<String>): List<Race> {
    val times = getAllNums(input[0])
    val records = getAllNums(input[1])
    return times.zip(records).map { (a, b) ->
        Race(a, b)
    }
}

fun getRaces2(input: List<String>): Race {
    val time = getAllNums(input[0]).joinToString("").toLong()
    val record = getAllNums(input[1]).joinToString("").toLong()
    return Race(time, record)
}

fun getDistance(time: Long, held: Long) =
    held * (time - held)

fun lowerBound(race: Race, lower: Long, upper: Long): Long {
    val med = (lower + upper) / 2
    if (lower + 1 == upper)
        return upper
    if (getDistance(race.time, med) > race.record) {
        return lowerBound(race, lower, med)
    }
    return lowerBound(race, med, upper)
}

fun upperBound(race: Race, lower: Long, upper: Long): Long {
    val med = (lower + upper) / 2
    if (lower + 1 == upper)
        return lower
    if (getDistance(race.time, med) <= race.record) {
        return upperBound(race, lower, med)
    }
    return upperBound(race, med, upper)
}

fun main() {
    val input = readInputs("day6.in")

    fun partOne() {
        val races = getRaces1(input)
        val total = races.map { race ->
            upperBound(race, 0, race.time) - lowerBound(race, 0, race.time) + 1
        }.reduce { a, b -> a * b }
        println(total)
    }

    fun partTwo() {
        val race = getRaces2(input)
        val total = upperBound(race, 0, race.time) - lowerBound(race, 0, race.time) + 1
        println(total)
    }

    partOne()
    partTwo()
}