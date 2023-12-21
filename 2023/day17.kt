import java.util.PriorityQueue

typealias Point = Pair<Int, Int>

fun main() {
    data class Item(val dist: Int, val loc: Point, val prev: Point, val straight: Int) : Comparable<Item> {
        override fun compareTo(other: Item) = compareValuesBy(this, other) { it.dist }
    }

    data class HashItem(val loc: Point, val prev: Point, val straight: Int)

    val map = readInputs("day17.in").map { it.toList() }
    val width = map[0].size
    val height = map.size
    val end = Pair(height - 1, width - 1)

    fun outBounds(cur: Point) = !inBounds(cur.first, 0, height - 1) || !inBounds(cur.second, 0, width - 1)

    fun getFront(cur: Item) =
        Point(cur.loc.first + (cur.loc.first - cur.prev.first), cur.loc.second + (cur.loc.second - cur.prev.second))

    fun getBack(cur: Item) =
        Point(cur.loc.first - (cur.loc.first - cur.prev.first), cur.loc.second - (cur.loc.second - cur.prev.second))

    fun solve(): Int {
        val start = Item(0, Pair(0, 0), Pair(0, 0), 0)
        val queue = PriorityQueue<Item>()
        val seen = mutableSetOf<HashItem>()
        val queued = mutableSetOf(HashItem(start.loc, start.prev, start.straight))
        queue.add(start)

        var iter = 0

        while (!queue.isEmpty()) {
            if (++iter % 1_000_000 == 0) {
                println("${queue.size} ${queued.size} ${seen.size}")
            }

            val cur = queue.poll()
            if (cur.loc == end) {
                return cur.dist
            }

            if (seen.contains(HashItem(cur.loc, cur.prev, cur.straight))) continue
            seen.add(HashItem(cur.loc, cur.prev, cur.straight))

            val front = getFront(cur)
            val back = getBack(cur)
            val search = listOf(
                Point(cur.loc.first + 1, cur.loc.second),
                Point(cur.loc.first, cur.loc.second + 1),
                Point(cur.loc.first - 1, cur.loc.second),
                Point(cur.loc.first, cur.loc.second - 1),
            )

            for (next in search) {
                if (outBounds(next) || next == back || (next == front && cur.straight == 2)) continue
                val weight = map[next.first][next.second] - '0'

                val nextFront = Item(cur.dist + weight, next, cur.loc, cur.straight + 1)
                val nextSide = Item(cur.dist + weight, next, cur.loc, 0)
                if (next == front && !queued.contains(HashItem(next, cur.loc, cur.straight + 1))) {
                    queue.add(nextFront)
                    queued.add(HashItem(next, cur.loc, cur.straight + 1))
                }
                else if (!queued.contains(HashItem(next, cur.loc, 0))) {
                    queue.add(nextSide)
                    queued.add(HashItem(next, cur.loc, 0))
                }
            }
        }

        return -1
    }

    // fun partOne() {
    //     val sol = solveDfs(Pair(0, 0), Pair(0, 1), 0, mutableSetOf(), 0)
    //     println(sol)
    // }

    // partOne()
    val sol = solve()
    println(sol)
}
