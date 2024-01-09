import kotlin.math.max
import kotlin.math.min

fun main() {
    data class Instruction(val dir: Int, val n: Int)
    data class Point(val row: Int, val col: Int)
    data class Line(val start: Point, val end: Point, val dir: Int, val orient: Int)

    fun Pair<Int, Int>.min(): Int {
        return min(this.first, this.second)
    }

    fun Pair<Int, Int>.max(): Int {
        return max(this.first, this.second)
    }

    fun toDir(c: Char) = when (c) {
        'R' -> 0
        'D' -> 1
        'L' -> 2
        else -> 3
    }

    fun getInstructions1(inputs: List<String>) = inputs.map {
        val split = it.split(' ')
        Instruction(toDir(split[0][0]), split[1].toInt())
    }

    fun fromHex(s: String): Int {
        var x = 0
        var place = 1
        var digits = listOf('0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f')
        for (i in 1..<s.length) {
            x += digits.indexOf(s[s.length - i]) * place
            place *= 16
        }
        return x
    }

    fun getInstructions2(inputs: List<String>) = inputs.map {
        val hex = it.split(' ')[2]
        val dir = hex[hex.length - 2] - '0'
        val n = fromHex(hex.dropLast(2).drop(1))
        Instruction(dir, n)
    }

    fun createLine(start: Point, orient: Int, i1: Instruction, i2: Instruction): Line {
        val directions = listOf(Point(0, 1), Point(1, 0), Point(0, -1), Point(-1, 0))
        val dir = directions[i1.dir]
        val end = Point(start.row + i1.n * dir.row, start.col + i1.n * dir.col)
        return Line(start, end, i1.dir, orient)
    }

    fun getLines(instructions: List<Instruction>): List<Line> {
        var cur = Point(0, 0)
        var orient = instructions[0].dir + 1
        var out = instructions.windowed(2).map { (i1, i2) ->
            val line = createLine(cur, orient, i1, i2)
            cur = line.end
            orient = (orient + i2.dir - i1.dir + 4) % 4
            line
        }
        val last = createLine(cur, orient, instructions.last(), instructions.first())
        return out + listOf(last)
    }

    fun rectangleCorners(top: Line, bot: Line, left: Line, right: Line): List<Int> {
        val r1 = top.start.row
        val r2 = bot.start.row
        val c1 = left.start.col
        val c2 = right.start.col
        return listOf(r1, c1, r2, c2)
    }

    fun contains(smaller: Pair<Int, Int>, larger: Pair<Int, Int>): Boolean {
        return larger.min() <= smaller.min() && larger.max() >= smaller.max()
    }

    fun orientedInside(top: Line, bot: Line, left: Line, right: Line): Boolean {
        // [1, 3, 0, 2] are inward orientations
        val (r1, c1, r2, c2) = rectangleCorners(top, bot, left, right)

        val topIn = top.orient == 1 || !contains(Pair(c1, c2), Pair(top.start.col, top.end.col))
        val botIn = bot.orient == 3 || !contains(Pair(c1, c2), Pair(bot.start.col, bot.end.col))
        val leftIn = left.orient == 0 || !contains(Pair(r1, r2), Pair(left.start.row, left.end.row))
        val rightIn = right.orient == 2 || !contains(Pair(r1, r2), Pair(right.start.row, right.end.row))

        return topIn && botIn && leftIn && rightIn
    }

    fun solve(instructions: List<Instruction>): Int {
        val lines = getLines(instructions)
        val hor = lines.filter { line -> (line.dir % 2) == 0 }.sortedBy { it.start.row }
        val ver = lines.filter { line -> (line.dir % 2) == 1 }.sortedBy { it.start.col }

        // for each rectangle
        // if the orientation is correct, add the area of the rectangle
        val rectangles = hor.windowed(2).flatMap { (top, bot) ->
            ver.windowed(2).filter { (left, right) ->
                orientedInside(top, bot, left, right)
            }.map { (left, right) ->
                listOf(top, bot, left, right)
            }
        }.map { (a, b, c, d) -> rectangleCorners(a, b, c, d) }.filter { (r1, c1, r2, c2) -> r1 != r2 && c1 != c2 }

        val sides = mutableMapOf<List<Int>, Int>()
        var area = 0
        for ((r1, c1, r2, c2) in rectangles) {
            sides[listOf(r1, c1, r1, c2)] = (sides.get(listOf(r1, c1, r1, c2)) ?: 0) + 1
            sides[listOf(r2, c1, r2, c2)] = (sides.get(listOf(r2, c1, r2, c2)) ?: 0) + 1
            sides[listOf(r1, c1, r2, c1)] = (sides.get(listOf(r1, c1, r2, c1)) ?: 0) + 1
            sides[listOf(r1, c2, r2, c2)] = (sides.get(listOf(r1, c2, r2, c2)) ?: 0) + 1
            area += (c2 - c1 + 1) * (r2 - r1 + 1)
        }

        for ((side, count) in sides) {
            val (r1, c1, r2, c2) = side
            area -= (count - 1) * (r2 + c2 - r1 - c1 + 1)
        }

        return area
    }

    val inputs = readInputs("day18.in")

    fun partOne() {
        val instructions = getInstructions1(inputs)
        val sol = solve(instructions)
        println(sol)
    }

    partOne()
    // partTwo()
}