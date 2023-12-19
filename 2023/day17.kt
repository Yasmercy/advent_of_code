typealias Point = Pair<Int, Int>

fun main() {
    val map = readInputs("day17.in").map { it.toList() }
    val width = map[0].size
    val height = map.size
    val end = Pair(height - 1, width - 1)
    var best = -1

    fun outBounds(cur: Point) = !inBounds(cur.first, 0, height - 1) || !inBounds(cur.second, 0, width - 1)

    fun setBest(new: Int): Int {
        if (best == -1 || new < best) best = new
        return new
    }

    fun solve(cur: Point, prev: Point, count: Int, visited: MutableSet<Point>, v: Int): Int {
        if (best != -1 && v >= best) return -1
        if (cur == end) return setBest(v)

        visited.add(cur)

        val search = mutableListOf(
            Pair(0, Pair(cur.first + 1, cur.second)),
            Pair(0, Pair(cur.first, cur.second + 1)),
            Pair(0, Pair(cur.first - 1, cur.second)),
            Pair(0, Pair(cur.first, cur.second - 1)),
        )

        val straight = Pair(cur.first + (cur.first - prev.first), cur.second + (cur.second - prev.second))
        search.remove(Pair(0, straight))
        if (count < 2) search.add(Pair(count + 1, straight))

        val dist = search.filterNot { (_, pos) ->
            outBounds(pos) || visited.contains(pos)
        }.map { (count, next) ->
            val weight = map[next.first][next.second] - '0'
            solve(next, cur, count, visited, v + weight)
        }.filter { it >= 0 }

        visited.remove(cur)

        return if (dist.isEmpty()) -1 else dist.min()
    }

    fun partOne() {
        val sol = solve(Pair(0, 0), Pair(0, 1), 0, mutableSetOf(), 0)
        println(sol)
    }

    partOne()
}
