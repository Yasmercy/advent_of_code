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

    fun horParity(line: Line, ver: List<Line>, rectangle: List<Long>): Boolean {
        // odd number of vertical lines between endpoint and rectangle
        val (r1, c1, r2, c2) = rectangle
        val (r, l) = Pair(max(line.start.col, line.end.col), min(line.start.col, line.end.col))
        val (b, t) = Pair(max(line.start.row, line.end.row), min(line.start.row, line.end.row))
        return if (r <= c1) {
            ver.count {it.start.col in (r+1)..<c1 && contains(Pair(r1, r2), Pair(b, t))} % 2 == 1
        } else if (l >= c2) {
            ver.count {it.start.col in (c2 + 1)..<l && contains(Pair(r1, r2), Pair(b, t))} % 2 == 1
        } else {
            println("dying")
            false
        }
    }

    fun verParity(line: Line, hor: List<Line>, rectangle: List<Long>): Boolean {
        // odd number of horizontal lines between endpoint and rectangle
        val (r1, c1, r2, c2) = rectangle
        val (r, l) = Pair(max(line.start.col, line.end.col), min(line.start.col, line.end.col))
        val (b, t) = Pair(max(line.start.row, line.end.row), min(line.start.row, line.end.row))
        return if (r1 >= b) {
            hor.count {it.start.row in (b + 1)..<r1 && contains(Pair(c1, c2), Pair(r, l))} % 2 == 1
        } else if (t >= r2) {
            hor.count {it.start.col in (r2 + 1)..<t && contains(Pair(c1, c2), Pair(r, l))} % 2 == 1
        } else {
            println("dying")
            false
        }
    }

    fun validHorExtension(line: Line, hor: List<Line>, rectangle: List<Long>): Boolean {
        val (_, c1, _, c2) = rectangle
        if (!contains(Pair(c1, c2), Pair(line.start.col, line.end.col)) && !horParity(line, hor, rectangle)) {
            println("$line $rectangle")
        }
        return !contains(Pair(c1, c2), Pair(line.start.col, line.end.col)) && horParity(line, hor, rectangle)
    }

    fun validVerExtension(line: Line, ver: List<Line>, rectangle: List<Long>): Boolean {
        val (r1, _, r2, _) = rectangle
        return !contains(Pair(r1, r2), Pair(line.start.row, line.end.row)) && verParity(line, ver, rectangle)
    }

    fun orientedInside(top: Line, bot: Line, left: Line, right: Line, hor: List<Line>, ver: List<Line>): Boolean {
        // [1, 3, 0, 2] are inward orientations
        val rectangle = rectangleCorners(top, bot, left, right)

        val topIn = top.orient == 1 || validHorExtension(top, ver, rectangle)
        val botIn = bot.orient == 3 || validHorExtension(bot, ver, rectangle)
        val leftIn = left.orient == 0 || validVerExtension(left, hor, rectangle)
        val rightIn = right.orient == 2 || validVerExtension(right, hor, rectangle)

        return topIn && botIn && leftIn && rightIn
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

        // brute force area
        val rMin = hor.first().start.row.toInt()
        val rMax = hor.last().start.row.toInt()
        val cMin = ver.first().start.col.toInt()
        val cMax = ver.last().start.col.toInt()

        val grid = MutableList(rMax - rMin + 1) { MutableList(cMax - cMin + 1) { false } }
        var count = 0
        for ((r1, c1, r2, c2) in rectangles) {
            val r1 = r1.toInt()
            val r2 = r2.toInt()
            val c1 = c1.toInt()
            val c2 = c2.toInt()
            for (r in r1..r2) {
                for (c in c1..c2) {
                    count += 1 - grid[r - rMin][c - cMin].toInt()
                    grid[r - rMin][c - cMin] = true
                }
            }
        }
        println("Real area = $count")
        // println(grid.map {it.map {if (it) "#" else "."}.joinToString("")}.joinToString("\n"))

        return rectangles
    }

    fun getAreaRectangles(rectangles: List<List<Long>>): Long {
        val sides = mutableMapOf<List<Long>, Long>()
        val points = mutableMapOf<Pair<Long, Long>, Long>()
        var area = 0L

        val rMin = rectangles.minOf { (r1, c1, r2, c2) -> min(r1, r2) }
        val cMin = rectangles.minOf { (r1, c1, r2, c2) -> min(c1, c2) }

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
            // if (count > 1) println("$side ${(r2 + c2 - r1 - c1 + 1L)}")
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

        // println(rectangles.joinToString("\n"))
        // println(grid.map {it.map {if (it) "#" else "."}.joinToString("")}.joinToString("\n"))

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
    // partTwo()
}
