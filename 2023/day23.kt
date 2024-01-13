import kotlin.math.max

fun main() {
    // adjacency list representation, pair(to, dist)
    data class Graph(val edges: List<List<Pair<Int, Int>>>)
    data class Location(val r: Int, val c: Int)

    fun getNeighbors(cur: Location, map: List<List<Char>>): List<Location> {
        val (r, c) = cur
        val neighbors = listOf(Location(r - 1, c), Location(r, c + 1), Location(r + 1, c), Location(r, c - 1))

        val slopes = "^>v<".toList()
        return neighbors.filterIndexed { i, p ->
            (p.r in map.indices && p.c in map[0].indices) && (map[p.r][p.c] == '.' || map[p.r][p.c] == slopes[i])
        }
    }

    fun getAdjacent(cur: Location, map: List<List<Char>>): List<Location> {
        val (r, c) = cur
        val neighbors = listOf(Location(r - 1, c), Location(r, c + 1), Location(r + 1, c), Location(r, c - 1))
        return neighbors.filter { p -> (p.r in map.indices && p.c in map[0].indices) && (map[p.r][p.c] != '#') }
    }

    fun distance(from: Location, to: Location, map: List<List<Char>>): Int {
        if (from == to) return 0

        val neighbors = getNeighbors(from, map)
        var out = 0
        for (neighbor in neighbors) {
            var cur = neighbor
            var prev = from
            var dist = 0
            while (++dist > 0 && cur != to) {
                val next = getNeighbors(cur, map).filter { it != prev }
                if (next.size != 1) break
                prev = cur
                cur = next[0]
            }
            if (cur == to) out = max(out, dist)
        }
        return out
    }

    fun createGraph(map: List<List<Char>>): Graph {
        val start = Location(0, map[0].indexOf('.'))
        val end = Location(map.size - 1, map.last().indexOf('.'))
        val height = map.size
        val width = map[0].size

        val nodes = (0..<height).flatMap { r ->
            (0..<width).filter { c ->
                val cur = Location(r, c)
                cur == start || cur == end || getAdjacent(cur, map).size > 2
            }.map { c -> Location(r, c) }
        }

        val adjList = MutableList<MutableList<Pair<Int, Int>>>(nodes.size) { mutableListOf() }
        for (i in nodes.indices) {
            val from = nodes[i]
            for (j in nodes.indices) {
                val to = nodes[j]
                val dist = distance(from, to, map)
                if (dist > 0) adjList[i].add(Pair(j, dist))
            }
        }

        return Graph(adjList)
    }

    val infinity = 100_000

    fun longestPath(cur: Int, graph: Graph, seen: MutableList<Boolean>): Int {
        // returns the longest path between nodes cur and end
        // filters paths by whether you can still reach the end (not implemented)

        if (cur == graph.edges.size - 1) return infinity

        var best = 0
        for ((next, dist) in graph.edges[cur]) {
            if (seen[next]) continue
            seen[next] = true
            best = max(best, dist + longestPath(next, graph, seen))
            seen[next] = false
        }
        return best
    }

    fun partOne() {
        val map = readInputs("day23.in").map { it.toList() }
        val graph = createGraph(map)

        val sol = longestPath(0, graph, MutableList(graph.edges.size) { false }) - infinity
        println(sol)
    }

    fun partTwo() {
        val map = readInputs("day23.in").map { it.map { c -> if (c == '#') '#' else '.' } }
        val graph = createGraph(map)

        val sol = longestPath(0, graph, MutableList(graph.edges.size) { false }) - infinity
        println(sol)
    }

    partOne()
    partTwo()
}
