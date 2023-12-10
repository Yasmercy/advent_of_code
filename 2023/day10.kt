import kotlin.math.abs

data class Loc(val row: Int, val col: Int)
data class Trans(val start: Loc, val end: Loc)
data class Directions(val path1: Trans, val path2: Trans)

val tileToDir = mapOf(
    '|' to Directions(Trans(Loc(-1, 0), Loc(-1, 0)), Trans(Loc(1, 0), Loc(1, 0))),
    '-' to Directions(Trans(Loc(0, -1), Loc(0, -1)), Trans(Loc(0, 1), Loc(0, 1))),
    'L' to Directions(Trans(Loc(1, 0), Loc(0, 1)), Trans(Loc(0, -1), Loc(-1, 0))),
    'J' to Directions(Trans(Loc(1, 0), Loc(0, -1)), Trans(Loc(0, 1), Loc(-1, 0))),
    'F' to Directions(Trans(Loc(-1, 0), Loc(0, 1)), Trans(Loc(0, -1), Loc(1, 0))),
    '7' to Directions(Trans(Loc(-1, 0), Loc(0, -1)), Trans(Loc(0, 1), Loc(1, 0))),
)

fun goToNext(start: Loc, from: Loc, map: List<List<Char>>): Loc {
    val diff = Loc(start.row - from.row, start.col - from.col)

    if (!tileToDir.contains(map[start.row][start.col]))
        return start

    val dir = tileToDir[map[start.row][start.col]]!!

    return when (diff) {
        dir.path1.start -> Loc(start.row + dir.path1.end.row, start.col + dir.path1.end.col)
        dir.path2.start -> Loc(start.row + dir.path2.end.row, start.col + dir.path2.end.col)
        else -> start
    }
}

fun getPath(start: Loc, dir: Loc, map: List<List<Char>>): List<Loc> {
    val seen = mutableSetOf<Loc>()
    val out = mutableListOf(start)
    var cur = Loc(start.row + dir.row, start.col + dir.col)
    var from = start

    while (!seen.contains(cur)) {
        seen.add(cur)
        out.add(cur)

        val tmp = cur
        cur = goToNext(cur, from, map)
        from = tmp
        if (cur == tmp)
            return out.toList()
    }
    out.add(cur)
    return out.toList()
}

fun getCycle(start: Loc, map: List<List<Char>>): List<Loc> {
    for (dr in -1..1) {
        for (dc in -1..1) {
            if (abs(dr + dc % 2) != 1)
                continue
            val path = getPath(start, Loc(dr, dc), map)
            if (path.size > 1 && path.first() == path.last())
                return path
        }
    }
    return listOf()
}

fun getStart(map: List<List<Char>>): Loc {
    for (i in map.indices) {
        for (j in map[i].indices) {
            if (map[i][j] == 'S')
                return Loc(i, j)
        }
    }
    return Loc(0, 0)
}

// disjoint set representation by size
// uses the index as row * width +  col
fun init(size: Int) = (0..<size).map { -1 }.toMutableList()

fun find(set: MutableList<Int>, index: Int): Int {
    if (set[index] >= 0) {
        val out = find(set, set[index])
        set[index] = out
        return out
    }
    return index
}

fun union(set: MutableList<Int>, a: Int, b: Int) {
    val r1 = find(set, a)
    val r2 = find(set, b)
    if (r1 == r2) return
    if (set[r1] < set[r2]) {
        set[r1] += set[r2]
        set[r2] = r1
    } else {
        set[r2] += set[r1]
        set[r1] = r2
    }
}

fun connectAll(set: MutableList<Int>, cur: Loc, ignore: List<Int>, w: Int, h: Int) {
    val curIdx = cur.row * w + cur.col
    if (ignore.contains(curIdx)) return

    val dirs = listOf(Loc(-1, 0), Loc(1, 0), Loc(0, 1), Loc(0, -1))
    for (dir in dirs) {
        val neighbor = Loc(cur.row + dir.row, cur.col + dir.col)
        val index = neighbor.row * w + neighbor.col
        if (neighbor.row in 0..<h && neighbor.col in 0..<w && !ignore.contains(index)) {
            union(set, curIdx, index)
        }
    }
}


fun main() {
    val inputs = readInputs("day10.in").map { it.toList() }
    val start = getStart(inputs)
    val cycle = getCycle(start, inputs)

    fun partOne() {
        val sol = cycle.size / 2
        println(sol)
    }

    fun partTwo() {
        val height = inputs.size
        val width = inputs[0].size
        val loop = cycle.map { node ->
            node.row * width + node.col
        }

        val ds = init(width * height)
        for (node in loop)
            union(ds, loop[0], node)
        for (row in 0..<height) {
            for (col in 0..<width) {
                connectAll(ds, Loc(row, col), loop, width, height)
            }
        }

        val sol = width * height - cycle.size + ds[find(ds, 0)] + 1
        println(sol)
    }

    partOne()
    partTwo()
}
