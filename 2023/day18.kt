fun main() {
    data class Instruction(val dir: Int, val n: Int)
    data class Point(val row: Int, val col: Int)
    data class Line(val start: Point, val end: Point, val dir: Int, val orient: Int)

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

    fun createLine(start: Point, i1: Instruction, i2: Instruction): Line {
        val directions = listOf(Point(0, 1), Point(1, 0), Point(0, -1), Point(-1, 0))
        val dir = directions[i1.dir]
        val end = Point(start.row + i1.n * dir.row, start.col + i1.n * dir.col)
        return Line(start, end, i1.dir, i2.dir)
    }

    fun getLines(instructions: List<Instruction>): List<Line> {
        var cur = Point(0, 0)
        var out = instructions.windowed(2).map {
            val line = createLine(cur, it[0], it[1])
            cur = line.end
            line
        }
        val last = createLine(cur, instructions.last(), instructions.first())
        return out + listOf(last)
    }

    fun orientedInside(h1: Line, h2: Line, v1: Line, v2: Line): Boolean {
        return false
    }

    fun solve(instructions: List<Instruction>): Int {
        val lines = getLines(instructions)
        val hor = lines.filter {line -> (line.dir % 2) == 0}.sortedBy { it.start.col }
        val ver = lines.filter {line -> (line.dir % 2) == 1}.sortedBy { it.start.row }

        // for each rectangle
        // if the orientation is correct, add the area of the rectangle
        val area = hor.windowed(2).flatMap { (h1, h2) ->
            ver.windowed(2).map { (v1, v2) ->
                0
            }
        }

        return area.sum()
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