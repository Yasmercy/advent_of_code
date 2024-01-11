import kotlin.math.min

fun main() {
    data class Range(val start: Long, val end: Long)
    data class Rock(val x: Range, val y: Range, val z: Range)

    operator fun <T> List<T>.component6() = this[5]

    fun Range.min(): Long {
        return min(this.start, this.end)
    }

    fun parseRock(rock: String): Rock {
        val (x1, y1, z1, x2, y2, z2) = getAllNums(rock)
        return Rock(Range(x1, x2), Range(y1, y2), Range(z1, z2))
    }

    fun rangeIntersect(a: Range, b: Range) =
        (b.start <= a.start && a.start <= b.end) || (b.start <= a.end && a.end <= b.end) || (a.start <= b.start && b.start <= a.end) || (a.start <= b.end && b.end <= a.end)

    fun rockIntersect(a: Rock, b: Rock) =
        rangeIntersect(a.x, b.x) && rangeIntersect(a.y, b.y) && rangeIntersect(a.z, b.z)

    fun moveDown(r: Rock) = Rock(r.x, r.y, Range(r.z.start - 1, r.z.end - 1))

    fun canMoveDown(r: Rock, floor: Long, rocks: List<Rock>): Boolean {
        return (r.z.min() > floor) && !rocks.any { it != r && rockIntersect(it, moveDown(r)) }
    }

    fun updateRocks(rocks: List<Rock>, floor: Long) = rocks.map { r ->
        if (canMoveDown(r, floor, rocks)) moveDown(r)
        else r
    }

    fun dropRocks(rocks: List<Rock>, floor: Long): List<Rock> {
        var out = rocks.sortedBy { it.z.min() }
        while (true) {
            val newRocks = updateRocks(out, floor).sortedBy { it.z.min() }
            if (newRocks == out) break
            out = newRocks
        }
        return out
    }

    fun supportedBy(r: Rock, rocks: List<Rock>): List<Rock> {
        val down = moveDown(r)
        return rocks.filter { it != r && rockIntersect(down, it) }
    }

    fun supporting(r: Rock, rocks: List<Rock>) = rocks.filter {
        it != r && rockIntersect(moveDown(it), r)
    }

    fun movedRocks(r: Rock, supporting: Map<Rock, Set<Rock>>, supported: Map<Rock, Set<Rock>>, cur: MutableSet<Rock>) {
        cur.add(r)
        val add = supporting[r]!!.filter { rock -> !cur.contains(rock) && supported[rock]!!.all { cur.contains(it) } }
        cur += add
        for (newMoved in add) {
            movedRocks(newMoved, supporting, supported, cur)
        }
    }

    val inputs = readInputs("day22.in")
    val rocks = inputs.map(::parseRock)
    val floor = rocks.minOf { rock -> rock.z.min() }
    val droppedRocks = dropRocks(rocks, floor)

    fun partOne() {
        val supported = droppedRocks.map { supportedBy(it, droppedRocks) }
        val cantDelete = supported.filter { it.size == 1 }.toSet()
        val sol = rocks.size - cantDelete.size
        println(sol)
    }

    fun partTwo() {
        val supported = droppedRocks.associateWith { supportedBy(it, droppedRocks).toSet() }
        val supporting = droppedRocks.associateWith { supporting(it, droppedRocks).toSet() }
        val allMoved = droppedRocks.map {
            val moved = mutableSetOf<Rock>()
            movedRocks(it, supporting, supported, moved)
            moved
        }

        val sol = allMoved.sumOf { it.size - 1 }
        println(sol)
    }

    partOne()
    partTwo()
}
