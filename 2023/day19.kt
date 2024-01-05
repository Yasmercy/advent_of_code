import kotlin.io.path.Path
import kotlin.io.path.readLines

fun main() {
    data class Cond(val category: String, val compare: String, val value: Int)
    data class Workflow(val name: String, val rules: List<Pair<Cond, String>>)
    data class Part(val data: List<Int>)

    fun readWorkflow(workflow: String): Workflow {
        val name = "[^{]*".toRegex().find(workflow)!!.value
        val ruleStrings = "\\{(.*)}".toRegex().find(workflow)!!.groupValues[1].split(',')
        val rules = ruleStrings.map { rule ->
            val values = "([xmas])([<>])(\\d+):([a-zA-Z]+)".toRegex().find(rule)
            if (values != null) {
                val groups = values.groupValues
                Pair(Cond(groups[1], groups[2], groups[3].toInt()), groups[4])
            } else Pair(Cond("a", ">", -1), rule)
        }
        return Workflow(name, rules)
    }

    fun readPart(part: String): Part {
        val digits = "\\d+".toRegex().findAll(part).toList().map { it.value.toInt() }
        return Part(digits)
    }

    fun readWorkflows(workflows: String) = workflows.split('\n').map(::readWorkflow)

    fun readParts(parts: String) = parts.split('\n').map(::readPart)

    fun matchCond(cond: Cond, part: Part): Boolean {
        val selector = "xmas".indexOf(cond.category)
        return if (cond.compare == ">") part.data[selector] > cond.value
        else part.data[selector] < cond.value
    }

    fun getDest(workflow: Workflow, part: Part): String {
        for (rule in workflow.rules)
            if (matchCond(rule.first, part)) return rule.second
        println("unreachable")
        return ""
    }

    val input = Path("day19.in").readLines().joinToString("\n").split("\n\n")
    val workflows = readWorkflows(input[0])
    val parts = readParts(input[1])

    fun partOne() {
        val accepted = mutableListOf<Part>()
        val rejected = mutableListOf<Part>()

        val queue = workflows.associate {
            it.name to mutableListOf<Part>()
        }.toMutableMap()
        queue["in"] = parts.toMutableList()

        while (!queue.values.all { it.isEmpty() }) {
            for (workflow in workflows) {
                for (part in queue[workflow.name]!!) {
                    when (val dest = getDest(workflow, part)) {
                        "A" -> accepted.add(part)
                        "R" -> rejected.add(part)
                        else -> queue[dest]!!.add(part)
                    }
                }
                queue[workflow.name] = mutableListOf()
            }
        }

        val sol = accepted.sumOf {it.data.sum()}
        println(sol)
    }

    fun partTwo() {

    }

    partOne()

}