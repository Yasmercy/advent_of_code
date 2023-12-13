import kotlin.io.path.Path
import kotlin.io.path.readLines

fun readInputs(name: String) = Path(name).readLines()
fun Boolean.toInt() = if (this) 1 else 0
fun inBounds(x: Int, lower: Int, higher: Int) =
    x in lower..higher

fun getAllNums(line: String) = "-?\\d+".toRegex().findAll(line).map { it.value.toLong() }.toList()

fun <T> joinList(arr: List<T>, n: Int, sep: T): List<T> {
    val out = mutableListOf<T>()

    for (i in 0..<n - 1) {
        out += arr
        out += sep
    }
    out += arr
    return out
}

operator fun <T> List<T>.times(index: Int): List<T> {
    val out = mutableListOf<T>()
    for (i in 0..<index) {
        out += this
    }
    return out.toList()
}