fun isZero(arr: List<Long>) = arr.all { it == 0L }

fun getDifferences(arr: List<Long>): List<Long> {
    return arr.windowed(2).map { (a, b) -> b - a }
}

fun makePattern(arr: List<Long>): List<List<Long>> {
    return if (isZero(arr))
        listOf(arr)
    else listOf(arr) + makePattern(getDifferences(arr))
}

fun extendPattern(pattern: List<List<Long>>): Long {
    val backs = pattern.map { it.last() }
    return backs.reduceRight { a, b -> a + b }
}

fun dextendPattern(pattern: List<List<Long>>): Long {
    val backs = pattern.map { it.first() }
    return backs.reduceRight { a, b -> a - b }
}

fun main() {
    val inputs = readInputs("day9.in")
    val nums = inputs.map(::getAllNums)

    fun partOne() {
        val patterns = nums.map(::makePattern)
        val extended = patterns.map(::extendPattern)
        val sol = extended.sum()
        println(sol)
    }

    fun partTwo() {
        val patterns = nums.map(::makePattern)
        val extended = patterns.map(::dextendPattern)
        val sol = extended.sum()
        println(sol)
    }

    partOne()
    partTwo()
}

