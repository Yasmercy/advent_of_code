import kotlin.math.abs

fun main() {
    class Rational(top: Long, bot: Long) {
        val num = makeNum(top, bot)
        val denom = makeDenom(top, bot)

        override fun toString(): String {
            return "Rational(num=$num, denom=$denom)"
        }

        override fun equals(other: Any?) = (other is Rational) && this.num == other.num && this.denom == other.denom

        private fun makeNum(top: Long, bot: Long) = abs(top) / gcd(abs(top), abs(bot))
        private fun makeDenom(top: Long, bot: Long) = bot * (if (top < 0) -1 else 1) / gcd(abs(top), abs(bot))
    }

    operator fun Rational.compareTo(other: Long) = this.num.compareTo(other * denom) * (if (denom < 0) -1 else 1)
    operator fun Long.compareTo(other: Rational) = -other.compareTo(this)
    operator fun Rational.times(other: Long) = Rational(this.num * other, this.denom)
    operator fun Rational.plus(other: Long) = Rational(this.num + this.denom * other, this.denom)

    data class Vector2D(val x: Long, val y: Long)
    data class Line2D(val start: Vector2D, val dir: Vector2D)
    data class Vector3D(val x: Long, val y: Long, val z: Long)
    data class Line3D(val start: Vector3D, val dir: Vector3D)

    operator fun <T> List<T>.component6() = this[5]

    fun createLine(nums: List<Long>): Line3D {
        val (x, y, z, dx, dy, dz) = nums
        return Line3D(Vector3D(x, y, z), Vector3D(dx, dy, dz))
    }

    fun aliasLines(v1: Line2D, v2: Line2D): List<Long> {
        val a = v1.dir.x
        val b = v1.dir.y
        val c = -v2.dir.x
        val d = -v2.dir.y
        val x = v2.start.x - v1.start.x
        val y = v2.start.y - v1.start.y
        return listOf(a, b, c, d, x, y)
    }

    fun infIntersections(v1: Line2D, v2: Line2D): Boolean {
        val (a, b, c, d, x, y) = aliasLines(v1, v2)

        if ((a * d - b * c) != 0L) return false
        if (x == 0L && y == 0L) return true
        if (x == 0L || y == 0L) return false
        if (Rational(b, a) == Rational(d, c)) return Rational(b, a) == Rational(y, x)
        if (Rational(c, a) == Rational(d, b)) return Rational(b, a) == Rational(y, x)
        return false
    }

    fun noIntersections(v1: Line2D, v2: Line2D): Boolean {
        val (a, b, c, d, _, _) = aliasLines(v1, v2)
        return ((a * d - b * c) == 0L) && !infIntersections(v1, v2)
    }

    fun intersection(v1: Line2D, v2: Line2D): Pair<Rational, Rational> {
        val (a, b, c, d, x, y) = aliasLines(v1, v2)
        val determinant = a * d - b * c
        val t = Rational(d * x - c * y, determinant)
        val s = Rational(a * y - b * x, determinant)

        return Pair(t, s)
    }

    fun getVector(line: Line2D, t: Rational): Pair<Rational, Rational> {
        val px = t * line.dir.x + line.start.x
        val py = t * line.dir.y + line.start.y
        return Pair(px, py)
    }

    fun intersectLines(a: Line3D, b: Line3D): List<Rational>? {
        val pairs = listOf(
            Pair(
                Line2D(Vector2D(a.start.x, a.start.y), Vector2D(a.dir.x, a.dir.y)),
                Line2D(Vector2D(b.start.x, b.start.y), Vector2D(b.dir.x, b.dir.y))
            ), Pair(
                Line2D(Vector2D(a.start.x, a.start.z), Vector2D(a.dir.x, a.dir.z)),
                Line2D(Vector2D(b.start.x, b.start.z), Vector2D(b.dir.x, b.dir.z))
            ), Pair(
                Line2D(Vector2D(a.start.y, a.start.z), Vector2D(a.dir.y, a.dir.z)),
                Line2D(Vector2D(b.start.y, b.start.z), Vector2D(b.dir.y, b.dir.z))
            )
        )

        if (pairs.any { (v1, v2) -> noIntersections(v1, v2) }) return null
        if (pairs.all { (v1, v2) -> infIntersections(v1, v2) }) {
            println("dying")
            return null
        }

        val intersections =
            pairs.filterNot { (v1, v2) -> infIntersections(v1, v2) }.map { (v1, v2) -> intersection(v1, v2) }

        if (intersections.any { parametric -> parametric != intersections[0] }) return null

        val (t, s) = intersections[0]
        val p1 = listOf(t * a.dir.x + a.start.x, t * a.dir.y + a.start.y, t * a.dir.z + a.start.z)
        val p2 = listOf(s * b.dir.x + b.start.x, s * b.dir.y + b.start.y, s * b.dir.z + b.start.z)
        if (p1 != p2) {
            println("dying")
        }
        return p1
    }

    val inputs = readInputs("day24.in")
    val lines = inputs.map { createLine(getAllNums(it)) }

    fun partOne() {
        val lines2D = lines.map { line ->
            Line2D(Vector2D(line.start.x, line.start.y), Vector2D(line.dir.x, line.dir.y))
        }
        // val lines2D = listOf(
        //     Line2D(Vector2D(19, 13), Vector2D(-2, 1)),
        //     Line2D(Vector2D(18, 19), Vector2D(-1, -1)),
        // )

        val min = 200000000000000L
        val max = 400000000000000L
        var sol = 0

        for (i in lines2D.indices) {
            for (j in (i + 1)..<lines2D.size) {
                val (v1, v2) = Pair(lines2D[i], lines2D[j])
                if (v1 == v2 || noIntersections(v1, v2)) continue
                val (t, s) = intersection(v1, v2)
                val (x1, y1) = getVector(v1, t)
                val (x2, y2) = getVector(v2, s)

                if (x1 != x2 || y1 != y2) {
                    // println("$v1 $v2 $t $s")
                    // println("$x1 $x2 $y1 $y2 ${x1 != x2} ${y1 != y2}")
                    println("BAD")
                    continue
                }

                // if (t >= 0 && s >= 0) println("$x1 $y1")

                if (t < 0 || s < 0) continue

                if (t >= 0 && s >= 0 && min <= x1 && x1 <= max && min <= y1 && y1 <= max) ++sol
            }
        }
        println(sol)
    }

    fun partTwo() {
        val min = 7L
        val max = 27L
        var sol = 0

        for (v1 in lines) {
            for (v2 in lines) {
                if (v1 == v2) continue
                val intersection = intersectLines(v1, v2) ?: continue
                println(intersection)
                if (intersection.all { min <= it && it <= max }) ++sol
            }
        }
        println(sol)
    }

    partOne()
}

