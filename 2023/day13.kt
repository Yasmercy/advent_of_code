import kotlin.math.min

fun main() {
    fun isPalindrome(s: String) = s.reversed() == s

    fun splitBetweenIndex(s: String, index: Int): String {
        // 012345, (1) 1.5 -> 0123
        // 012345, (3) 3.5 -> 2345
        // 01234, (1) 1.5 -> 0123
        // 01234, (3) 3.5 -> 2345
        val leftDistance = index
        val rightDistance = s.length - index - 2
        val dist = min(leftDistance, rightDistance)
        return s.slice(index - dist..index + dist + 1)
    }

    fun symmetricHor(inputs: List<String>): Int {
        for (i in 0..<inputs[0].length - 1) if (inputs.all { isPalindrome(splitBetweenIndex(it, i)) }) return i
        return -1
    }

    fun symmetricVert(inputs: List<String>): Int {
        val length = inputs[0].length
        for (i in 0..<inputs.size - 1) {
            val strings = (0..<length).map { c ->
                inputs.indices.map { r ->
                    inputs[r][c]
                }.joinToString("")
            }
            if (strings.all { isPalindrome(splitBetweenIndex(it, i)) }) return i
        }
        return -1
    }

    fun createGroups(inputs: List<String>): List<List<List<String>>> {
        // should do this with reading file and splitting by '\n\n'
        val splits = inputs.withIndex().filter { it.value.isEmpty() }.map { it.index }
        val divisions = listOf(-1) + splits + listOf(inputs.size)
        return divisions.windowed(2).map {
            inputs.slice(it[0] + 1..<it[1])
        }.chunked(2)
    }

    fun getValues(groups: List<List<List<String>>>) = groups.map { group ->
        val c1 = symmetricHor(group[0]) + 1
        val r1 = symmetricVert(group[0]) + 1
        val c2 = symmetricHor(group[1]) + 1
        val r2 = symmetricVert(group[1]) + 1
        100 * (r1 + r2) + c1 + c2
    }

    val inputs = readInputs("day13.in")
    val groups = createGroups(inputs)

    fun partOne() {
        val values = getValues(groups)
        val sol = values.sum()
        println(sol)
    }

    partOne()
}