import java.io._
val w = new PrintWriter(new java.io.File("SkewedJoinInput1.txt" ))

for (i <- 0 to 100) {
    w.write("300\tstrawberry\tsss" + (3*i) + "\n")
    w.write("200\torange0001\tooo" + (3*i+1) + "\n")
    w.write("300\tstrawberry\tsss" + (3*i+2) + "\n")
}
w.close()

val w2 = new PrintWriter(new java.io.File("SkewedJoinInput2.txt" ))
w2.write("100\tapple00001\n")
w2.write("100\tapple00002\n")
w2.write("100\tapple00002\n")
w2.write("200\torange0001\n")
w2.write("200\torange0002\n")
w2.write("300\tstrawberry\n")
w2.write("400\tpear000000\n")
w2.close()

// for sampling
val w3= new PrintWriter(new java.io.File("SkewedJoinInput_1000.txt" ))
val r = scala.util.Random

for ( i <- 0 to 1000) {
	(r.nextInt(100) % 3) match {
	case 0 => w3.write("200\torange0001\tooo" + i + "\n")
	case _ => w3.write("300\tstrawberry\tsss" + i + "\n")
	}
}
w3.close()
