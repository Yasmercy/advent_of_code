import kotlin.io.path.Path
import kotlin.io.path.readText

fun main() {
    fun getHash(str: String): Int {
        var hash = 0
        for (c in str) {
            val ord = c.code
            hash += ord
            hash *= 17
            hash %= 256
        }
        return hash
    }

    val input = Path("day15.in").readText().split(",")
    val hashes = input.map(::getHash)

    fun partOne() {
        val sol = hashes.sum()
        println(sol)
    }

    fun partTwo() {
        val boxes = MutableList<MutableList<Pair<String, Int>>>(256) { mutableListOf() }
        for (command in input) {
            val name = "[a-zA-Z]+".toRegex().find(command)!!.value
            val hash = getHash(name)
            if (command.contains("=")) {
                val split = command.split("=")
                val value = Pair(split[0], split[1].toInt())
                val key = boxes[hash].find { it.first == name }
                if (key != null) {
                    val index = boxes[hash].indexOf(key)
                    boxes[hash][index] = value
                } else {
                    boxes[hash].add(value)
                }
            } else {
                val key = boxes[hash].find { it.first == name }
                boxes[hash].remove(key)
            }
        }

        var sol = 0
        for ((boxNum, box) in boxes.withIndex())
            for ((slotNum, item) in box.withIndex())
                sol += (boxNum + 1) * (slotNum + 1) * item.second
        println(sol)
    }

    partOne()
    partTwo()
}