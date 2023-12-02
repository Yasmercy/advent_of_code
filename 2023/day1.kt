fun filterDigits(data: List<String>): List<List<Int>> {
    return data.map {
        val digits = it.filter { d -> d.isDigit() }
        listOf(digits.getOrElse(0) { '0' } - '0', digits.getOrElse(digits.lastIndex) { '0' } - '0')
    }
}

fun processDigits(data: List<List<Int>>): List<Int> {
    return data.map {
        it[0] * 10 + it[1]
    }
}

fun getNumber(num: String, start: Int): String {
    val numbers = listOf(
        "1",
        "2",
        "3",
        "4",
        "5",
        "6",
        "7",
        "8",
        "9",
        "one",
        "two",
        "three",
        "four",
        "five",
        "six",
        "seven",
        "eight",
        "nine"
    )
    for (n in numbers) {
        if (start + n.length <= num.length && num.substring(start, start + n.length) == n) {
            return n
        }
    }
    return ""
}

fun getNumbers(data: String): List<String> {
    return (0..data.length).map {
        getNumber(data, it)
    }.filter {
        it != ""
    }
}

fun numToInt(num: String): Int {
    if (num[0].isDigit()) {
        return num[0] - '0'
    }
    val numbers = listOf("zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine")
    return numbers.indexOf(num)
}


fun main() {
    fun partOne() {
        val input = readInputs("day1.in")
        val digits = filterDigits(input)
        val processed = processDigits(digits)
        val sum = processed.sumOf { it }
        println(sum)

    }

    fun partTwo() {
        val input = readInputs("day1.in")
        val sequence = input.map {
            val numbers = getNumbers(it)
            numToInt(numbers.first()) * 10 + numToInt(numbers.last())
        }
        val sum = sequence.sumOf { it }
        println(sum)
    }

    partOne()
    partTwo()
}
