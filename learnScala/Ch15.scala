// case class 
abstract class Expr
case class Var(name: String) extends Expr 
case class Number(num:Double) extends Expr
case class UnOp(operator:String, arg:Expr) extends Expr
case class BinOp(operator:String, left:Expr, right:Expr) extends Expr

object Ch15 {
	def main(args:Array[String]) {
		val v = Var("x")
		val n0 = Number(0)
		val n1 = Number(1)
		val unop1 = UnOp("-", n1)
		val unop2= UnOp("-", unop1)
		val binop = BinOp("+", v, n0)
		simplifyTop(binop)
	}

	def simplifyTop(expr:Expr): Expr = expr match {
		case UnOp("-", UnOp("-", e)) => e
		case BinOp("+", e, Number(0)) => e
		case BinOp("*", e, Number(1)) => e
		case _ => expr
	}
}