fun main() {
    fun solve(i: Int, k: Int, record: List<Char>, groups: List<Int>): Int {
        // solving: amount of ways to fill the group
        // with the kth group starting at element i

        // base case: index validation
        if (i >= record.size || k == groups.size) return 0

        // base case: record substring not match
        val size = groups[k]
        if (record.size - i < size || record.slice(i..<i + size).any { it == '.' }) return 0

        // summing up amount of ways to fill the remaining groups
        // requires remaining groups to be left
        var sum = 0
        if (k < groups.size - 1) {
            for (start in i + size + 1..<record.size) {
                // ensure not skipping over any hashes
                if (record.slice(i + size..<start).any { it == '#' }) break
                sum += solve(start, k + 1, record, groups)
            }
        }

        if (k == groups.size - 1 && record.slice(i + size..<record.size).all { it != '#' })
            return 1 + sum
        return sum
    }

    fun solution(record: List<Char>, groups: List<Int>): Int {
        var lastStart = record.indexOf('#')
        if (lastStart == -1)
            lastStart = record.size - 1

        val counts = (0..lastStart).map { solve(it, 0, record, groups) }
        return counts.sum()
    }

    val inputs = readInputs("day12.in")
    val records = inputs.map { line -> line.split(' ')[0].toList() }
    val groups = inputs.map { line -> getAllNums(line.split(' ')[1]).map { it.toInt() } }

    fun partOne() {
        val sol = records.zip(groups).sumOf { (r, g) -> solution(r, g) }
        println(sol)
    }

    partOne()

}