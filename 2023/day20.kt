import java.util.ArrayDeque

fun main() {
    data class Signal(val from: String, val to: String, val low: Boolean)
    data class Module(
        val type: String,
        val out: List<String>,
        var on: Boolean = false,
        val memory: MutableMap<String, Boolean> = mutableMapOf()
    )

    fun parseModule(item: String): Pair<String, Module> {
        val names = "\\w+".toRegex().findAll(item).map { it.value }.toMutableList()
        val name = names.removeFirst()
        val type = "[&%]".toRegex().find(item)?.value
        return Pair(name, Module(type ?: "", names))
    }

    fun readModules(items: List<String>): MutableMap<String, Module> {
        val out = items.associate(::parseModule).toMutableMap()
        for ((k, v) in out) {
            for (name in v.out) {
                if (out.contains(name)) out[name]!!.memory[k] = true
            }
        }
        return out
    }

    fun processSignal(signal: Signal, modules: Map<String, Module>): List<Signal> {
        // println("${signal.from} -${if (signal.low) "low" else "high"}-> ${signal.to}")
        val to = modules[signal.to] ?: return listOf()
        return if (to.type == "%" && signal.low) {
            to.on = !to.on
            to.out.map { Signal(signal.to, it, !to.on) }
        } else if (to.type == "&") {
            to.memory[signal.from] = signal.low
            val low = to.memory.values.all { !it }
            to.out.map { Signal(signal.to, it, low) }
        } else if (to.type == "") {
            to.out.map { Signal(signal.to, it, signal.low) }
        } else {
            listOf()
        }
    }

    val inputs = readInputs("day20.in")

    fun partOne() {
        var low = 0
        var high = 0
        val queue = ArrayDeque<Signal>()
        val modules = readModules(inputs)

        for (i in 1..1000) {
            queue.add(Signal("button", "broadcaster", true))
            while (!queue.isEmpty()) {
                val item = queue.poll()
                if (item.low) ++low else ++high

                val signals = processSignal(item, modules)
                for (signal in signals) {
                    queue.add(signal)
                }
            }
        }

        val sol = low * high
        println("$sol")
    }

    fun partTwo() {
        val queue = ArrayDeque<Signal>()
        val modules = readModules(inputs)

        var i = 0
        var sol = 0
        while (++i > 0 && sol == 0) {
            queue.add(Signal("button", "broadcaster", true))
            while (!queue.isEmpty()) {
                val item = queue.poll()
                if (item.to == "rx" && item.low) sol = i
                val signals = processSignal(item, modules)
                for (signal in signals) {
                    queue.add(signal)
                }
            }
        }

        println("$sol")
    }

    partOne()
    partTwo()
}