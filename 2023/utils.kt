import kotlin.io.path.Path
import kotlin.io.path.readLines

fun readInputs(name: String) = Path(name).readLines()
fun Boolean.toInt() = if (this) 1 else 0
fun inBounds(x: Int, lower: Int, higher: Int) =
    x in lower..higher

fun getAllNums(line: String) = "-?\\d+".toRegex().findAll(line).map { it.value.toLong() }.toList()
