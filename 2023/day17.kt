import java.util.PriorityQueue
import kotlin.math.max

fun main() {
    val map = readInputs("day17.in").map { it.toList() }
    val width = map[0].size
    val height = map.size
    val end = Pair(height - 1, width - 1)

    fun heuristic(cur: Point) = max((end.first - cur.first), (end.second - cur.second))
    data class Item(val dist: Int, val loc: Point, val prev: Point, val straight: Int) : Comparable<Item> {
        override fun compareTo(other: Item) = compareValuesBy(this, other) { it.dist + heuristic(it.loc) }
    }

    data class HashItem(val loc: Point, val prev: Point, val straight: Int)

    fun outBounds(cur: Point) = !inBounds(cur.first, 0, height - 1) || !inBounds(cur.second, 0, width - 1)

    fun getFront(cur: Item) =
        Point(cur.loc.first + (cur.loc.first - cur.prev.first), cur.loc.second + (cur.loc.second - cur.prev.second))

    fun getBack(cur: Item) =
        Point(cur.loc.first - (cur.loc.first - cur.prev.first), cur.loc.second - (cur.loc.second - cur.prev.second))

    fun solve(min: Int, max: Int): Int {
        val start = Item(0, Pair(0, 0), Pair(0, 0), min)
        val queue = PriorityQueue<Item>()
        val seen = mutableSetOf<HashItem>()
        val queued = mutableSetOf(HashItem(start.loc, start.prev, start.straight))
        queue.add(start)

        while (!queue.isEmpty()) {
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
                if (outBounds(next) || next == back || (next == front && cur.straight == max) || (next != front && cur.straight < min)) continue
                val weight = map[next.first][next.second] - '0'

                val nextFront = Item(cur.dist + weight, next, cur.loc, cur.straight + 1)
                val nextSide = Item(cur.dist + weight, next, cur.loc, 1)
                val hashFront = HashItem(next, cur.loc, cur.straight + 1)
                val hashSide = HashItem(next, cur.loc, 1)
                if (next == front && !queued.contains(hashFront)) {
                    queue.add(nextFront)
                    queued.add(hashFront)
                } else if (!queued.contains(hashSide)) {
                    queue.add(nextSide)
                    queued.add(hashSide)
                }
            }
        }

        return -1
    }

    fun partOne() {
        val sol = solve(1, 3)
        println(sol)
    }

    fun partTwo() {
        val sol = solve(4, 10)
        println(sol)
    }

    partOne()
    partTwo()
}
