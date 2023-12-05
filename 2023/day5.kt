import kotlin.math.min

data class Interval(val start: Long, val change: Long, val length: Long)

fun getSeeds1(seeds: String) = getAllNums(seeds).map {
    Interval(it, 0, 1)
}

fun getSeeds2(seeds: String) =
    getAllNums(seeds).chunked(2).map {
        Interval(it[0], 0, it[1])
    }

fun getMap(map: String) = getAllNums(map).chunked(3).map {
    Interval(it[1], it[0] - it[1], it[2])
}

fun getMaps(input: List<String>): List<List<Interval>> {
    val joined = input.joinToString().split(':')
    val maps = joined.slice(2..<joined.size).map {
        getMap(it)
    }
    return maps
}

fun intersectInterval(a: Interval, b: Interval): Interval {
    if (b.start >= a.start) {
        return Interval(b.start, b.change, min(b.length, a.length - (b.start - a.start)))
    }
    return Interval(a.start, b.change, min(a.length, b.length - (a.start - b.start)))
}

fun differenceInterval(a: Interval, b: Interval): List<Interval> {
    val intersect = intersectInterval(a, b)
    if (intersect.length <= 0) {
        return listOf(a)
    }

    return if (a.start <= intersect.start) {
        listOf(
            Interval(a.start, 0, intersect.start - a.start),
            Interval(
                intersect.start + intersect.length, 0,
                a.length + a.start - intersect.start - intersect.length
            )
        )
    } else {
        listOf(
            Interval(
                intersect.start + intersect.length, 0,
                a.length + a.start - intersect.start - intersect.length
            )
        )
    }
}

fun emptyInterval(a: Interval) = a.length <= 0L

fun remapInterval(a: Interval, b: List<Interval>): List<Interval> {
    val remap = b.map {
        val int = intersectInterval(a, it)
        Interval(int.start + int.change, 0, int.length)
    }.filter { !emptyInterval(it) }

    var remaining = listOf(a)
    for (int in b) {
        remaining = remaining.flatMap { differenceInterval(it, int) }.filter { !emptyInterval(it) }
    }
    return remap + remaining
}

fun remapIntervals(a: List<Interval>, b: List<Interval>): List<Interval> {
    return a.flatMap {remapInterval(it, b)}
}

fun unionAll(intervals: List<List<Interval>>): List<Interval> {
    return intervals.reduce { a, b -> remapIntervals(a, b) }
}

fun main() {
    val input = readInputs("day5.in")

    fun solve(seeds: List<Interval>): Long {
        val maps = getMaps(input)
        val joined = listOf(seeds) + maps
        val union = unionAll(joined)
        val min = union.minBy { it.start }.start
        return min
    }

    println(solve(getSeeds1(input[0])))
    println(solve(getSeeds2(input[0])))
}
