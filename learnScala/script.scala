//programming in scala

//------- ch6: Functional Objects ---------
class Rational(n:Int, d:Int) {
	require(d != 0)
	
	private val g = gcd(n.abs, d.abs)

	val numer = n/g
	val denom = d/g

	def this(n:Int) = this(n,1)

	override def toString = numer + "/" + denom
	def add(that:Rational):Rational = new Rational(
			numer * that.denom + denom * that.numer, 
			denom * that.denom
		)
	def multply(that:Rational):Rational = new Rational(
			numer * that.numer, denom * that.denom
		)
	def +(that:Rational): Rational = add(that)
	def *(that:Rational): Rational = multply(that)

	private def gcd(a:Int, b:Int):Int = 
		if (b==0) a else gcd(b, a%b)
}

// implicit conversions
//implicit def intToRational(i:Int) = new Rational(i)

val half = new Rational(1,2)
val twoThirds = new Rational(2,3)
val two = new Rational(2)
System.out.println(half add twoThirds + two*twoThirds)

//------- ch7: Control structures ---------
val filesHere = (new java.io.File(".")).listFiles
def linesOfFile(file: java.io.File) = scala.io.Source.fromFile(file).getLines.toList
def grep(pattern: String) = for {
	file <- filesHere
	if file.getName.endsWith(".scala")
	line <- linesOfFile(file)
	if line.trim.matches(pattern)
} yield line
//{
	//println(file)
	//file.getName + ": " + line.trim
	//println(line)
//	line
//}

(grep(".*gcd.*")).mkString

//
