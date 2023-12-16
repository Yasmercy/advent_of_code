import kotlin.math.min

fun main() {
    fun isPalindrome(s: String) = s.reversed() == s

    fun splitBetweenIndex(s: String, index: Int): String {
        // 012345, (1) 1.5 -> 0123
        // 012345, (3) 3.5 -> 2345
        val leftDistance = index
        val rightDistance = s.length - index - 2
        val dist = min(leftDistance, rightDistance)
        return s.slice(index - dist..index + dist + 1)
    }

    fun symmetricHor(inputs: List<String>): Int {
        for (i in 0..inputs[0].length)
            if (inputs.all { isPalindrome(splitBetweenIndex(it, i)) })
                return i
        return -1
    }

    fun symmetricVert(inputs: List<String>): Int {
        val length = inputs[0].length
        for (i in 0..inputs.size) {
            val strings = (0..<length).map { c ->
                inputs.indices.map { r ->
                    inputs[r][c]
                }.joinToString("")
            }
            if (strings.all { isPalindrome(splitBetweenIndex(it, i)) })
                return i
        }
        return -1
    }

    fun createGroups(inputs: List<String>): List<List<List<String>>> {
        val out = mutableListOf<List<List<List<String>>>>()
        val splits = inputs.withIndex().filter { it.value.isEmpty() }.map { it.index }
        val divisions = listOf(-1) + splits + listOf(inputs.size)

        return divisions.windowed(2).map {
            inputs.slice(it[0] + 1..<it[1])
        }.chunked(2)
    }

    val inputs = readInputs("day13.in")
    val groups = createGroups(inputs)
    println(groups[1])


    fun partOne() {
        val first = inputs.slice(0..6)
        val second = inputs.slice(8..<inputs.size)

        val c = symmetricHor(first) + 1
        val r = symmetricVert(second) + 1
        val sol = 100 * r + c
        println(sol)
    }

    // partOne()
}