import java.math.BigInteger

fun main() {
    class Rational(top: BigInteger, bot: BigInteger) {
        val num = makeNum(top, bot)
        val denom = makeDenom(top, bot)

        override fun toString(): String {
            return "Rational(num=$num, denom=$denom)"
        }

        override fun equals(other: Any?) = (other is Rational) && this.num == other.num && this.denom == other.denom

        fun sign(x: BigInteger): BigInteger {
            if (x < BigInteger("0")) return BigInteger("-1")
            if (x > BigInteger("0")) return BigInteger("1")
            return BigInteger("0")
        }

        private fun makeNum(top: BigInteger, bot: BigInteger) = top.abs() / gcd(top.abs(), bot.abs())
        private fun makeDenom(top: BigInteger, bot: BigInteger) = bot * sign(top) / gcd(top.abs(), bot.abs())
    }


    operator fun Rational.compareTo(other: BigInteger) = this.num.compareTo(other * denom) * sign(denom).toInt()
    operator fun BigInteger.compareTo(other: Rational) = -other.compareTo(this)
    operator fun Rational.times(other: BigInteger) = Rational(this.num * other, this.denom)
    operator fun Rational.plus(other: BigInteger) = Rational(this.num + this.denom * other, this.denom)

    data class Vector2D(val x: BigInteger, val y: BigInteger)
    data class Line2D(val start: Vector2D, val dir: Vector2D)
    data class Vector3D(val x: BigInteger, val y: BigInteger, val z: BigInteger)
    data class Line3D(val start: Vector3D, val dir: Vector3D)

    operator fun <T> List<T>.component6() = this[5]

    fun createLine(nums: List<BigInteger>): Line3D {
        val (x, y, z, dx, dy, dz) = nums
        return Line3D(Vector3D(x, y, z), Vector3D(dx, dy, dz))
    }

    fun aliasLines(v1: Line2D, v2: Line2D): List<BigInteger> {
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
        if ((a * d - b * c) != BigInteger("0")) return false
        if (x == BigInteger("0") && y == BigInteger("0")) return true
        if (x == BigInteger("0") || y == BigInteger("0")) return false
        if (Rational(b, a) == Rational(d, c)) return Rational(b, a) == Rational(y, x)
        if (Rational(c, a) == Rational(d, b)) return Rational(b, a) == Rational(y, x)
        return false
    }

    fun noIntersections(v1: Line2D, v2: Line2D): Boolean {
        val (a, b, c, d) = aliasLines(v1, v2)
        return ((a * d - b * c) == BigInteger("0")) && !infIntersections(v1, v2)
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

    val inputs = readInputs("day24.in")
    val lines = inputs.map { createLine(getAllNums(it).map { x -> x.toBigInteger() }) }

    fun partOne() {
        val lines2D = lines.map { line ->
            Line2D(Vector2D(line.start.x, line.start.y), Vector2D(line.dir.x, line.dir.y))
        }

        val min = 200000000000000L.toBigInteger()
        val max = 400000000000000L.toBigInteger()
        var sol = 0

        for (i in lines2D.indices) {
            for (j in (i + 1)..<lines2D.size) {
                val (v1, v2) = Pair(lines2D[i], lines2D[j])
                if (v1 == v2 || noIntersections(v1, v2)) continue
                val (t, s) = intersection(v1, v2)
                val (x1, y1) = getVector(v1, t)

                if (t < 0.toBigInteger() || s < 0.toBigInteger()) continue
                if (min <= x1 && x1 <= max && min <= y1 && y1 <= max) ++sol
            }
        }
        println(sol)
    }

    partOne()
}