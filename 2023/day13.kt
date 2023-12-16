import kotlin.io.path.Path
import kotlin.io.path.readLines
import kotlin.math.min

fun main() {
    fun isPalindrome(s: String) = s.reversed() == s

    fun splitBetweenIndex(s: String, index: Int): String {
        val rightDistance = s.length - index - 2
        val dist = min(index, rightDistance)
        return s.slice(index - dist..index + dist + 1)
    }

    fun symmetricHor(inputs: List<String>, exclude: Set<Int>): Int {
        for (i in 0..<inputs[0].length - 1)
            if (!exclude.contains(i) && inputs.all { isPalindrome(splitBetweenIndex(it, i)) })
                return i
        return -1
    }

    fun symmetricVert(inputs: List<String>, exclude: Set<Int>): Int {
        val length = inputs[0].length
        for (i in 0..<inputs.size - 1) {
            val strings = (0..<length).map { c ->
                inputs.indices.map { r ->
                    inputs[r][c]
                }.joinToString("")
            }
            if (!exclude.contains(i) && strings.all { isPalindrome(splitBetweenIndex(it, i)) }) return i
        }
        return -1
    }

    fun getValue(group: List<String>, excludeHor: Set<Int>, excludeVer: Set<Int>): Int {
        val c = symmetricHor(group, excludeHor) + 1
        val r = symmetricVert(group, excludeVer) + 1
        return 100 * r + c
    }

    fun getValues(groups: List<List<String>>) = groups.map { getValue(it, setOf(), setOf()) }

    fun swap(c: Char) = if (c == '#') '.' else '#'

    fun mutateList(group: List<String>): List<List<String>> {
        val arr = group.map { it.toMutableList() }.toMutableList()
        val out = mutableListOf<List<String>>()
        for (r in group.indices) {
            for (c in group[0].indices) {
                arr[r][c] = swap(arr[r][c])
                out.add(arr.map { it.joinToString("") })
                arr[r][c] = swap(arr[r][c])
            }
        }
        return out
    }

    fun getMutValues(groups: List<List<String>>) = groups.map { group ->
        val excludeHor = setOf(symmetricHor(group, setOf()))
        val excludeVer = setOf(symmetricVert(group, setOf()))
        mutateList(group).maxOf { getValue(it, excludeHor, excludeVer) }
    }

    val input = Path("day13.in").readLines().joinToString("\n").split("\n\n")
    val groups = input.map { it.split('\n') }

    fun partOne() {
        val values = getValues(groups)
        val sol = values.sum()
        println(sol)
    }

    fun partTwo() {
        val values = getMutValues(groups)
        val sol = values.sum()
        println(sol)
    }

    partOne()
    partTwo()
}