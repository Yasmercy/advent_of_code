fun main() {
    data class Beam(val dir: Int, val loc: Pair<Int, Int>)

    val map = readInputs("day16.in").map { it.toList() }
    val directions = listOf(Pair(0, 1), Pair(1, 0), Pair(0, -1), Pair(-1, 0)) // E S W N

    fun inBoundBeam(beam: Beam) =
        inBounds(beam.loc.first, 0, map.size - 1) && inBounds(beam.loc.second, 0, map[0].size - 1)

    fun iterateBeam(cur: Beam, beams: MutableList<Beam>, seen: MutableSet<Beam>, pos: MutableSet<Pair<Int, Int>>) {
        var beam = cur
        while (!seen.contains(beam)) {
            seen.add(beam)
            pos.add(beam.loc)
            val dir = directions[beam.dir]
            val newPos = Pair(beam.loc.first + dir.first, beam.loc.second + dir.second)
            if (!inBoundBeam(Beam(0, newPos))) break

            if (map[newPos.first][newPos.second] != '.') pos.add(newPos)

            when (map[newPos.first][newPos.second]) {
                '/' -> beam = if (beam.dir % 2 == 0) Beam((beam.dir + 3) % 4, newPos)
                else Beam((beam.dir + 1) % 4, newPos)

                '\\' -> beam = if (beam.dir % 2 == 0) Beam((beam.dir + 1) % 4, newPos)
                else Beam((beam.dir + 3) % 4, newPos)

                '|' -> beam = if (beam.dir % 2 == 1) Beam(beam.dir, newPos)
                else {
                    beams.add(Beam(3, newPos))
                    Beam(1, newPos)
                }

                '-' -> beam = if (beam.dir % 2 == 0) Beam(beam.dir, newPos)
                else {
                    beams.add(Beam(2, newPos))
                    Beam(0, newPos)
                }

                else -> beam = Beam(beam.dir, newPos)
            }
        }
    }

    fun partOne() {
        val seen = mutableSetOf<Beam>()
        val beams = mutableListOf(Beam(0, Pair(0, -1)))
        val pos = mutableSetOf<Pair<Int, Int>>()

        while (beams.isNotEmpty()) {
            iterateBeam(beams.removeLast(), beams, seen, pos)
        }

        // for (i in map.indices) {
        //     for (j in map[0].indices) {
        //         if (pos.contains(Pair(i, j))) print('#')
        //         else print('.')
        //     }
        //     println()
        // }

        val sol = pos.size - 1
        println(sol)
    }

    partOne()
}