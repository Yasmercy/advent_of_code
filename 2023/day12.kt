fun main() {
    fun solve(i: Int, k: Int, record: List<Char>, groups: List<Int>): Int {
        // base case: index validation
        // if there is no more input, return whether finished
        if (i >= record.size)
            return 0

        // base case: no groups
        if (k == groups.size)
            return 0

        // base case: record substring too small
        val size = groups[k]
        if (record.size - i < size)
            return 0

        // summing up amount of ways to create the substring
        var sum = 0
        for (start in i..<record.size - size) {
            // ensure everything in substring is # or ?
            if (record.slice(start..<start + size).any { it == '.' })
                continue

            // ensure spacer after (if more groups)
            if (record[start + size] != '#')
                sum += solve(start + size + 1, k + 1, record, groups)
        }

        // edge case where ending group is used
        if (k == groups.size - 1 && (i..record.size - size).any { start ->
                record.slice(start..<start + size).all { it != '.' } &&
                        record.slice(start + size..<record.size).all { it != '#' }
            }
        ) {
            sum += 1
        }

        return sum
    }

    val inputs = readInputs("day12.in")
    val records = inputs.map { line -> line.split(' ')[0].toList() }
    val groups = inputs.map { line -> getAllNums(line.split(' ')[1]).map { it.toInt() } }

    fun partOne() {
        val counts = records.zip(groups).map { (r, g) ->
            solve(0, 0, r, g)
        }
        val sol = counts.sum()
        println(sol)
    }

    partOne()
}