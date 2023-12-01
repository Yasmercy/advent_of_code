import kotlin.io.path.Path
import kotlin.io.path.readLines

fun read_input(name: String) = Path(name).readLines()

fun filter_digits(data: List<String>): List<List<Int>> {
    return data.map { 
        val digits = it.filter { it.isDigit() }
        listOf(digits.getOrElse(0, { '0' }) - '0', digits.getOrElse(digits.lastIndex, { '0' }) - '0')
    }
}

fun process_digits(data: List<List<Int>>): List<Int> {
    return data.map {
        it[0] * 10 + it[1]
    }
}

fun part_one() {
    val input = read_input("day1.in")
    val digits = filter_digits(input)
    val processed = process_digits(digits)
    val sum = processed.sumOf { it }
    println(sum)

}

fun get_number(num: String, start: Int): String {
    val numbers = listOf("1", "2", "3", "4", "5", "6", "7", "8", "9", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine")
    for (n in numbers) {
        if (start + n.length <= num.length && num.substring(start, start + n.length) == n) {
            return n
        }
    }
    return ""
}

fun get_numbers(data: String): List<String> {
    return (0..data.length).map {
        get_number(data, it)
    }.filter {
        it != ""
    }
}

fun num_to_int(num: String) : Int {
    if (num[0].isDigit()) {
        return num[0] - '0'
    }
    val numbers = listOf("zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine")
    return numbers.indexOf(num)
}

fun part_two() {
    val input = read_input("day1.in")
    val sequence = input.map {
        val numbers = get_numbers(it)
        num_to_int(numbers.first()) * 10 + num_to_int(numbers.last())
    }
    val sum = sequence.sumOf {it}
    println(sum)
}

fun main() {
    part_one()
    part_two()
}
