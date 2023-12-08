data class Graph(val leftEdges: Map<String, String>, val rightEdges: Map<String, String>)

fun readSymbols(line: String) = "\\w\\w\\w".toRegex().findAll(line).map { it.value }.toList()

fun readGraph(input: List<String>): Graph {
    val leftEdges = input.map { line ->
        val symbols = readSymbols(line)
        symbols[0] to symbols[1]
    }.associate { it }
    val rightEdges = input.map { line ->
        val symbols = readSymbols(line)
        symbols[0] to symbols[2]
    }.associate { it }

    return Graph(leftEdges, rightEdges)
}

fun iterateForward(start: String, graph: Graph, commands: List<Char>, ends: Set<String>): Long {
    var cur = start
    var index = 0L
    while (!ends.contains(cur)) {
        cur = if (commands[(index % commands.size).toInt()] == 'L') {
            graph.leftEdges[cur]!!
        } else {
            graph.rightEdges[cur]!!
        }
        index += 1
    }
    return index
}

fun gcd(a: Long, b: Long): Long {
    return if (a == 0L || b == 0L) (a + b) else gcd(b, a - b * (a / b))
}

fun lcm(a: Long, b: Long) = (a * b) / gcd(a, b)

fun main() {
    val inputs = readInputs("day8.in")
    val graph = readGraph(inputs.slice(2..<inputs.size))
    val commands = inputs[0].toList()

    fun partOne() {
        val sol = iterateForward("AAA", graph, commands, setOf("ZZZ"))
        println(sol)
    }

    fun partTwo() {
        val edges = inputs.slice(2..<inputs.size).flatMap(::readSymbols)
        val starts = edges.filter { it.last() == 'A' }.toSet()
        val ends = edges.filter { it.last() == 'Z' }.toSet()

        val times = starts.map { s ->
            iterateForward(s, graph, commands, ends)
        }
        println(times)
        val sol = times.reduce(::lcm)
        println(sol)
    }

    // partOne()
    partTwo()
}