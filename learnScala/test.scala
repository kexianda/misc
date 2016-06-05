
// scalac test.scala
// javap -c -p test$.class test.class

object MyFun1 extends Function1[Int, Int] {
	def apply(x: Int): Int = x * x
}


// def f(s:String) = {"f(" + s +")"}
// def g(s:String) = {"g(" + s +")"}
  	
class Covariant[+A]


object Test {
  def main(args: Array[String]): Unit = {

  	//function
  	//MyFun1(3)

  	//val fComposeG = f _ compose g _
    //val gf = f _ compose g _
    //gf("sss")

    // local function
    def locFun1(a: Int) = {
      def locFun11(a:Int) = a+1
      locFun11(a)
    }
    def locFun2(a: Int) = a*2

    locFun1(locFun2(2))

    val one: PartialFunction[Int, String] = {case 1 => "one"}
    one(1);

    val cv: Covariant[AnyRef] = new Covariant[String]
  }
}

