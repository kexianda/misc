import java.io._
val w = new PrintWriter(new java.io.File("SkewedJoinInput_small.txt" ))

for (i <- 0 to 5) {
    w.write("300\tstrawberry\tsss" + (3*i) + "\n")
    w.write("200\torange0001\tooo" + (3*i+1) + "\n")
    w.write("300\tstrawberry\tsss" + (3*i+2) + "\n")
}
w.close()


// -------TestSkewedJoin UT data --------------
val u1 = new PrintWriter(new java.io.File("SkewedJoinUT1.txt" ))
var k = 0
for (i <- 0 to 120) {
    u1.write("100\tapple1\taaa" + k + "\n")
	k = k + 1
    u1.write("200\torange1\tbbb" + k + "\n")
	k = k + 1
    u1.write("300\tstrawberry\tccc" + k + "\n")
    k = k + 1
}
u1.close()

val u2 = new PrintWriter(new java.io.File("SkewedJoinUT2.txt" ))
u2.write("100\tapple1\n")
u2.write("100\tapple2\n")
u2.write("100\tapple2\n")
u2.write("200\torange1\n")
u2.write("200\torange2\n")
u2.write("300\tstrawberry\n")
u2.write("400\tpear\n")
u2.close()
// -----------------------------------------------


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
wEmptyL.println("1\tleft_one")
wEmptyL.println("2\tleft_two")
wEmptyL.println("3\tleft_three")
wEmptyL.println("5\tleft_five")
wEmptyL.close()
wEmptyR.println("1\tright_one")
wEmptyR.println("2\tright_two")
wEmptyR.println("3\tright_three")
wEmptyR.println("6\tright_six")
wEmptyR.close()
val empty = new PrintWriter(new java.io.File("testSkewedJoinEmptyInput.pig"))
empty.println("a = load 'SkewedJoinLeft.dat' as (nums:chararray,text:chararray);")
empty.println("b = load 'SkewedJoinRight.dat' as (number:chararray,text:chararray);")
empty.println("c = filter a by nums == '7';")
empty.println("d = join c by nums LEFT OUTER, b by number USING 'skewed';")
empty.println("dump d;")
empty.close()
