import kotlin.math.max
import kotlin.math.min

fun main() {
    data class Instruction(val dir: Int, val n: Long)
    data class Point(val row: Long, val col: Long)
    data class Line(val start: Point, val end: Point, val dir: Int, val orient: Int)

    fun Pair<Long, Long>.min(): Long {
        return min(this.first, this.second)
    }

    fun Pair<Long, Long>.max(): Long {
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
        Instruction(toDir(split[0][0]), split[1].toLong())
    }

    fun fromHex(s: String): Long {
        var x = 0L
        var place = 1L
        var digits = listOf('0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f')
        for (i in 1..<s.length) {
            x += digits.indexOf(s[s.length - i]).toLong() * place
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

    fun createLine(start: Point, orient: Int, i1: Instruction): Line {
        val directions = listOf(Point(0L, 1), Point(1, 0L), Point(0L, -1), Point(-1, 0L))
        val dir = directions[i1.dir]
        val end = Point(start.row + i1.n * dir.row, start.col + i1.n * dir.col)
        return Line(start, end, i1.dir, orient)
    }

    fun getLines(instructions: List<Instruction>): List<Line> {
        var cur = Point(0L, 0L)
        var orient = instructions[0].dir + 1
        var out = instructions.windowed(2).map { (i1, i2) ->
            val line = createLine(cur, orient, i1)
            cur = line.end
            orient = (orient + i2.dir - i1.dir + 4) % 4
            line
        }
        val last = createLine(cur, orient, instructions.last())
        return out + listOf(last)
    }

    fun rectangleCorners(top: Line, bot: Line, left: Line, right: Line): List<Long> {
        val r1 = top.start.row
        val r2 = bot.start.row
        val c1 = left.start.col
        val c2 = right.start.col
        return listOf(r1, c1, r2, c2)
    }

    fun contains(smaller: Pair<Long, Long>, larger: Pair<Long, Long>): Boolean {
        return larger.min() <= smaller.min() && larger.max() >= smaller.max()
    }

    fun orientedInside(top: Line, bot: Line, left: Line, right: Line, hor: List<Line>, ver: List<Line>): Boolean {
        val (r1, c1, r2, c2) = rectangleCorners(top, bot, left, right)
        val north = hor.filter { line ->
            line.start.row <= r1 && contains(Pair(c1, c2), Pair(line.start.col, line.end.col))
        }.lastOrNull() ?: return false
        val south = hor.filter { line ->
            line.start.row >= r2 && contains(Pair(c1, c2), Pair(line.start.col, line.end.col))
        }.firstOrNull() ?: return false
        val west = ver.filter { line ->
            line.start.col <= c1 && contains(Pair(r1, r2), Pair(line.start.row, line.end.row))
        }.lastOrNull() ?: return false
        val east = ver.filter { line ->
            line.start.col >= c2 && contains(Pair(r1, r2), Pair(line.start.row, line.end.row))
        }.firstOrNull() ?: return false

        return listOf(west.orient, north.orient, east.orient, south.orient) == listOf(0, 1, 2, 3)
    }

    fun getRectangles(lines: List<Line>): List<List<Long>> {
        val hor = lines.filter { line -> (line.dir % 2) == 0 }.sortedBy { it.start.row }
        val ver = lines.filter { line -> (line.dir % 2) == 1 }.sortedBy { it.start.col }

        // for each rectangle
        // if the orientation and parity are orrect, add the area of the rectangle
        val rectangles = hor.windowed(2).flatMap { (top, bot) ->
            ver.windowed(2).filter { (left, right) ->
                orientedInside(top, bot, left, right, hor, ver)
            }.map { (left, right) ->
                rectangleCorners(top, bot, left, right)
            }.filter { (r1, c1, r2, c2) -> r1 != r2 && c1 != c2 }
        }

        return rectangles
    }

    fun getAreaRectangles(rectangles: List<List<Long>>): Long {
        val sides = mutableMapOf<List<Long>, Long>()
        val points = mutableMapOf<Pair<Long, Long>, Long>()
        var area = 0L

        // add all areas
        for ((r1, c1, r2, c2) in rectangles) {
            points[Pair(r1, c1)] = (points.get(Pair(r1, c1)) ?: 0L) + 1L
            points[Pair(r2, c2)] = (points.get(Pair(r2, c2)) ?: 0L) + 1L
            points[Pair(r1, c2)] = (points.get(Pair(r1, c2)) ?: 0L) + 1L
            points[Pair(r2, c1)] = (points.get(Pair(r2, c1)) ?: 0L) + 1L
            sides[listOf(r1, c1, r1, c2)] = (sides.get(listOf(r1, c1, r1, c2)) ?: 0L) + 1L
            sides[listOf(r2, c1, r2, c2)] = (sides.get(listOf(r2, c1, r2, c2)) ?: 0L) + 1L
            sides[listOf(r1, c1, r2, c1)] = (sides.get(listOf(r1, c1, r2, c1)) ?: 0L) + 1L
            sides[listOf(r1, c2, r2, c2)] = (sides.get(listOf(r1, c2, r2, c2)) ?: 0L) + 1L
            area += (c2 - c1 + 1L) * (r2 - r1 + 1L)
        }

        // exclude all duplicate sides
        for ((side, count) in sides) {
            val (r1, c1, r2, c2) = side
            area -= (count - 1) * (r2 + c2 - r1 - c1 + 1L)
        }

        // include all overcounted points
        for (count in points.values) {
            if (count == 4L) area += 1L
        }

        return area
    }

    fun solve(instructions: List<Instruction>): Long {
        val lines = getLines(instructions)
        val rectangles = getRectangles(lines)
        val area = getAreaRectangles(rectangles)
        return area
    }

    val inputs = readInputs("day18.in")

    fun partOne() {
        val instructions = getInstructions1(inputs)
        val sol = solve(instructions)
        println(sol)
    }

    fun partTwo() {
        val instructions = getInstructions2(inputs)
        val sol = solve(instructions)
        println(sol)
    }

    partOne()
    partTwo()
}