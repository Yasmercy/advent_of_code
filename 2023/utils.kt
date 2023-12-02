import kotlin.io.path.Path
import kotlin.io.path.readLines

fun readInputs(name: String) = Path(name).readLines()
fun Boolean.toInt() = if (this) 1 else 0