import java.io._
val w = new PrintWriter(new java.io.File("SkewedJoinInput_small.txt" ))

for (i <- 0 to 5) {
    w.write("300\tstrawberry\tsss" + (3*i) + "\n")
    w.write("200\torange0001\tooo" + (3*i+1) + "\n")
    w.write("300\tstrawberry\tsss" + (3*i+2) + "\n")
}
w.close()

val w2 = new PrintWriter(new java.io.File("SkewedJoinInput2.txt" ))
w2.write("100\tapple00001\ta\n")
w2.write("100\tapple00002\ta\n")
w2.write("100\tapple00002\ta\n")
w2.write("200\torange0001\to\n")
w2.write("200\torange0002\to\n")
w2.write("300\tstrawberry\ts\n")
w2.write("400\tpear000000\tp\n")
w2.close()

// for sampling
val w1000= new PrintWriter(new java.io.File("SkewedJoinInput_1000.txt" ))
val r = scala.util.Random

for ( i <- 0 to 1000) {
	(r.nextInt(100) % 3) match {
	case 0 => w1000.write("200\torange0001\tooo" + i + "\n")
	case _ => w1000.write("300\tstrawberry\tsss" + i + "\n")
	}
}
w1000.close()

//---- TestSkewedJoin.testSkewedJoinOneValue()----
// SkewedJoinInput3.txt
val w3 = new PrintWriter(new java.io.File("SkewedJoinInput3.txt"))
w3.println("100\tapple1")
w3.println("100\tapple2")
w3.println("200\torange1")
w3.println("200\torange2")
w3.println("300\tstrawberry")
w3.println("300\tstrawberry2")
w3.println("400\tpear")
w3.close()
val oneValue = new PrintWriter(new java.io.File("testSkewedjoinOneValue.pig"))
oneValue.println("A = LOAD 'SkewedJoinInput3.txt' as (id, name);")
oneValue.println("B = LOAD 'SkewedJoinInput3.txt' as (id, name);")
oneValue.println("C = FILTER A by id == 400;")
oneValue.println("D = FILTER B by id == 400;")
oneValue.println("E = JOIN C by id, D BY id USING 'skewed';")
oneValue.println("DUMP E;")
oneValue.close()

//---- TestSkewedJoin.testSkewedJoinEmptyInput() ----
val wEmptyL = new PrintWriter(new java.io.File("SkewedJoinLeft.dat"))
val wEmptyR = new PrintWriter(new java.io.File("SkewedJoinRight.dat"))
wEmptyL.println("1")
wEmptyL.println("2")
wEmptyL.println("3")
wEmptyL.println("5")
wEmptyL.close()
wEmptyR.println("1\tone")
wEmptyR.println("2\ttwo")
wEmptyR.println("3\tthree")
wEmptyR.close()
val empty = new PrintWriter(new java.io.File("testSkewedJoinEmptyInput.pig"))
empty.println("a = load 'SkewedJoinLeft.dat' as (nums:chararray);")
empty.println("b = load 'SkewedJoinRight.dat' as (number:chararray,text:chararray);")
empty.println("c = filter a by nums == '7';")
empty.println("d = join c by nums LEFT OUTER, b by number USING 'skewed';")
empty.println("dump d;")
empty.close()
