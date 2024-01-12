import kotlin.math.max

fun main() {
    data class Location(val r: Int, val c: Int)

    fun getNeighbors(cur: Location, map: List<List<Char>>): List<Location> {
        val (r, c) = cur
        val neighbors = listOf(Location(r - 1, c), Location(r, c + 1), Location(r + 1, c), Location(r, c - 1))

        // val slopes = "^>v<".toList()
        // return neighbors.zip(slopes)
        //     .filter { (p, i) -> (p.r in map.indices && p.c in map[0].indices) && (map[p.r][p.c] == '.' || map[p.r][p.c] == i) }
        //     .map { it.first }

        return neighbors.filter { p -> (p.r in map.indices && p.c in map[0].indices) && (map[p.r][p.c] != '#') }
    }

    val map = readInputs("day23.in").map { it.toList() }
    val start = Location(0, map[0].indexOf('.'))

    fun solve(cur: Location, seen: MutableSet<Location>, map: List<List<Char>>): Int {
        if (cur.r == map.size - 1) return 100_000

        var out = -1
        for (neighbor in getNeighbors(cur, map)) {
            if (seen.contains(neighbor)) continue
            seen.add(neighbor)

            val dist = solve(neighbor, seen, map) + 1
            out = max(out, dist)
            seen.remove(neighbor)
        }

        return out
    }

    fun partOne() {
        val sol = solve(start, mutableSetOf(), map) - 100_000
        println(sol)
    }

    partOne()
}