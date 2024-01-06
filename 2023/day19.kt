import kotlin.io.path.Path
import kotlin.io.path.readLines

fun main() {
    data class Cond(val category: String, val compare: String, val value: Long)
    data class Workflow(val name: String, val rules: List<Pair<Cond, String>>)
    data class Part(val data: List<Long>)

    fun notCond(cond: Cond) = if (cond.compare == ">") Cond(cond.category, "<", cond.value + 1)
    else Cond(cond.category, ">", cond.value - 1)

    fun readWorkflow(workflow: String): Workflow {
        val name = "[^{]*".toRegex().find(workflow)!!.value
        val ruleStrings = "\\{(.*)}".toRegex().find(workflow)!!.groupValues[1].split(',')
        val rules = ruleStrings.map { rule ->
            val values = "([xmas])([<>])(\\d+):([a-zA-Z]+)".toRegex().find(rule)
            if (values != null) {
                val groups = values.groupValues
                Pair(Cond(groups[1], groups[2], groups[3].toLong()), groups[4])
            } else Pair(Cond("a", ">", -1), rule)
        }
        return Workflow(name, rules)
    }

    fun readPart(part: String): Part {
        val digits = "\\d+".toRegex().findAll(part).toList().map { it.value.toLong() }
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
        for (rule in workflow.rules) if (matchCond(rule.first, part)) return rule.second
        println("unreachable")
        return ""
    }

    fun accept(conds: List<Cond>, cur: String, rules: Map<String, List<Pair<Cond, String>>>): List<List<Cond>> {
        // for each condition in workflow:
        // add the condition to conditions (cloned)
        // if its accept -> add to output
        // if its reject -> continue
        // otherwise add the recursive called

        val out = mutableListOf<List<Cond>>()
        val not = mutableListOf<Cond>()
        for ((cond, dest) in rules[cur]!!) {
            val joined = conds + not + listOf(cond)
            when (dest) {
                "R" -> {}
                "A" -> out.add(joined)
                else -> out += accept(joined, dest, rules)
            }
            not.add(notCond(cond))
        }
        return out
    }

    fun numParts(ranges: List<Cond>): Long {
        val groups = ranges.groupBy { it.category }
        val mins = groups.values.map { rules ->
            rules.filter { rule -> rule.compare == ">" }.maxOf { rule -> rule.value }
        }
        val maxs = groups.values.map { rules ->
            rules.filter { rule -> rule.compare == "<" }.minOf { rule -> rule.value }
        }

        return mins.zip(maxs).map { (min, max) ->
            max - min - 1
        }.reduce { a, b -> a * b }
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

        val sol = accepted.sumOf { it.data.sum() }
        println(sol)
    }

    fun partTwo() {
        val accepted = accept(listOf(), "in", workflows.associate { it.name to it.rules })
        val range = "xmas".flatMap { c ->
            listOf(Cond(c.toString(), ">", 0), Cond(c.toString(), "<", 4001))
        }
        val acceptRange = accepted.map {
            it + range
        }

        val sol = acceptRange.sumOf(::numParts)
        println(sol)
    }

    partOne()
    partTwo()

}