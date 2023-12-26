fun main() {
    val map = readInputs("day21.in").map { it.toList() }
    val width = map[0].size
    val height = map.size

    val row = readInputs("day21.in").withIndex().filter { (_, row) -> row.contains('S') }[0].index
    val col = map[row].indexOf('S')
    val start = Point(row, col)

    fun posMod(x: Int, m: Int) = ((x % m) + m) % m
    fun validSpot(cur: Point) = map[posMod(cur.first, height)][posMod(cur.second, width)] != '#'

    fun solve(all: MutableSet<Point>, current: Set<Point>, remaining: Int): Int {
        if (remaining == 0) return all.count { ((it.first + it.second) % 2) == 0 }

        val next = current.flatMap { cur ->
            listOf(
                Point(cur.first + 1, cur.second),
                Point(cur.first, cur.second + 1),
                Point(cur.first - 1, cur.second),
                Point(cur.first, cur.second - 1),
            ).filter { !all.contains(it) && validSpot(it) }
        }.toSet()

        for (x in next) all.add(x)

        return solve(all, next, remaining - 1)
    }

    fun partOne() {
        val sol = solve(mutableSetOf(), setOf(start), 1000)
        println(sol)
    }

    partOne()

}