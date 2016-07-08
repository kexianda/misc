trait Trait1 {
	val name = "Trait1"
	def hi() {
		println("I am a trait, my name is " + name)
	}	
}

trait Trait2 {
	def say() {
		println("I am a trait, my name is Trait2.")
	}
}

class ClassExtendsTrait2 extends Trait2 with Trait1{
	override def toString = {
		println(super.toString)
		"ClassExtendsTrait2"
	}
}

// --------------------------------------------------
// Test stackable modification
abstract class IntQueue {
	def get(): Int
	def put(x: Int)
}
import scala.collection.mutable.ArrayBuffer
class BasicIntQueue extends IntQueue {
	private val buf = new ArrayBuffer[Int]
	def get() = buf.remove(0)
	def put(x:Int) {buf += x}
}
trait Doubling extends IntQueue {
	abstract override def put(x:Int) { super.put(2*x) }
}
trait Increasing  extends IntQueue {
	abstract override def put(x:Int) { super.put(x+1) }
}
class MyQueue extends BasicIntQueue with Doubling with Increasing {

}
// trait  {
	
// }
// --------------------------------------------------
object Ch12 {
	def main(args: Array[String]): Unit = {
		val c = new ClassExtendsTrait2
		c.say()
		c.hi()
		c.toString
		println(c.name)

		stackableModificate
	}

	def stackableModificate:Unit = {
		val myQ = new MyQueue;
		myQ.put(10)
		println(myQ.get())
	}
}
/*

*/


// Ch12.class ---
// Ch12.class: like a java interface
// javap -c Ch12.class
/*
Compiled from "Ch12.scala"
public final class Ch12 {
  public static void main(java.lang.String[]);
    Code:
       0: getstatic     #16                 // Field Ch12$.MODULE$:LCh12$;
       3: aload_0
       4: invokevirtual #18                 // Method Ch12$.main:([Ljava/lang/String;)V
       7: return
}
*/
/*
Compiled from "Ch12.scala"
public final class Ch12$ {
  public static final Ch12$ MODULE$;

  public static {};
    Code:
       0: new           #2                  // class Ch12$
       3: invokespecial #12                 // Method "<init>":()V
       6: return

  public void main(java.lang.String[]);
    Code:
       0: new           #16                 // class ClassExtendsTrait2
       3: dup
       4: invokespecial #17                 // Method ClassExtendsTrait2."<init>":()V
       7: astore_2
       8: aload_2
       9: invokevirtual #20                 // Method ClassExtendsTrait2.say:()V
      12: return
}
*/
//--- bytecode for trait ---
/*
---javap -c Trait2.class:---
Compiled from "Ch12.scala"
public abstract class Trait2$class {
  public static void say(Trait2);
    Code:
       0: getstatic     #13                 // Field scala/Predef$.MODULE$:Lscala/Predef$;
       3: ldc           #15                 // String I am a trait, my name is Trait2.
       5: invokevirtual #19                 // Method scala/Predef$.println:(Ljava/lang/Object;)V
       8: return

  public static void $init$(Trait2);
    Code:
       0: return
}
*/
//-----
/*
---javap -c Trait2$class.class:---

Compiled from "Ch12.scala"
public abstract class Trait2$class {
  public static void say(Trait2);
    Code:
       0: getstatic     #13                 // Field scala/Predef$.MODULE$:Lscala/Predef$;
       3: ldc           #15                 // String I am a trait, my name is Trait2.
       5: invokevirtual #19                 // Method scala/Predef$.println:(Ljava/lang/Object;)V
       8: return

  public static void $init$(Trait2);
    Code:
       0: return
}
*/

/*
--- javap -c ClassExtendsTrait2.class ---
Compiled from "Ch12.scala"
public class ClassExtendsTrait2 implements Trait2,Trait1 {
  public java.lang.String name();
    Code:
       0: aload_0
       1: getfield      #17                 // Field name:Ljava/lang/String;
       4: areturn

  public void Trait1$_setter_$name_$eq(java.lang.String);
    Code:
       0: aload_0
       1: aload_1
       2: putfield      #17                 // Field name:Ljava/lang/String;
       5: return

  public void hi();
    Code:
       0: aload_0
       1: invokestatic  #29                 // Method Trait1$class.hi:(LTrait1;)V
       4: return

  public void say();
    Code:
       0: aload_0
       1: invokestatic  #35                 // Method Trait2$class.say:(LTrait2;)V
       4: return

  public java.lang.String toString();
    Code:
       0: ldc           #37                 // String ClassExtendsTrait2
       2: areturn

  public ClassExtendsTrait2();
    Code:
       0: aload_0
       1: invokespecial #40                 // Method java/lang/Object."<init>":()V
       4: aload_0
       5: invokestatic  #43                 // Method Trait2$class.$init$:(LTrait2;)V
       8: aload_0
       9: invokestatic  #45                 // Method Trait1$class.$init$:(LTrait1;)V
      12: return
}

*/
/*
Compiled from "Ch12.scala"
public abstract class Trait1$class {
  public static void hi(Trait1);
    Code:
       0: getstatic     #13                 // Field scala/Predef$.MODULE$:Lscala/Predef$;
       3: new           #15                 // class scala/collection/mutable/StringBuilder
       6: dup
       7: invokespecial #19                 // Method scala/collection/mutable/StringBuilder."<init>":()V
      10: ldc           #21                 // String I am a trait, my name is
      12: invokevirtual #25                 // Method scala/collection/mutable/StringBuilder.append:(Ljava/lang/Object;)Lscala/collection/mutable/StringBuilder;
      15: aload_0
      16: invokeinterface #31,  1           // InterfaceMethod Trait1.name:()Ljava/lang/String;
      21: invokevirtual #25                 // Method scala/collection/mutable/StringBuilder.append:(Ljava/lang/Object;)Lscala/collection/mutable/StringBuilder;
      24: invokevirtual #34                 // Method scala/collection/mutable/StringBuilder.toString:()Ljava/lang/String;
      27: invokevirtual #38                 // Method scala/Predef$.println:(Ljava/lang/Object;)V
      30: return

  public static void $init$(Trait1);
    Code:
       0: aload_0
       1: ldc           #42                 // String Trait1
       3: invokeinterface #46,  2           // InterfaceMethod Trait1.Trait1$_setter_$name_$eq:(Ljava/lang/String;)V
       8: return
}
*/