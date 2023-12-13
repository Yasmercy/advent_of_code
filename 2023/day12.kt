fun main() {
    fun getSolution(record: List<Char>, groups: List<Int>): Long {
        val dp = MutableList(record.size) { MutableList(groups.size + 1) { 0L } }

        for (k in groups.indices.reversed()) {
            for (i in record.indices) {
                val size = groups[k]
                if (record.size - i < size || record.slice(i..<i + size).any { it == '.' })
                    continue

                for (start in i + size + 1..<record.size) {
                    if (record.slice(i + size..<start).any { it == '#' }) break
                    dp[i][k] += dp[start][k + 1]
                }

                if (k == groups.size - 1 && record.slice(i + size..<record.size).all { it != '#' })
                    dp[i][k] += 1L

            }
        }

        var lastStart = record.indexOf('#')
        if (lastStart == -1)
            lastStart = record.size - 1

        val counts = (0..lastStart).map { dp[it][0] }
        return counts.sum()
    }

    val inputs = readInputs("day12.in")
    val records = inputs.map { line -> line.split(' ')[0].toList() }
    val groups = inputs.map { line -> getAllNums(line.split(' ')[1]).map { it.toInt() } }

    fun partOne() {
        val sol = records.zip(groups).sumOf { (r, g) -> getSolution(r, g) }
        println(sol)
    }

    fun partTwo() {
        val newRecords = records.map { line -> joinList(line, 5, '?') }
        val newGroups = groups.map { line -> line * 5 }
        val counts = newRecords.zip(newGroups).sumOf { (r, g) -> getSolution(r, g) }
        println(counts)
    }

    partOne()
    partTwo()
}