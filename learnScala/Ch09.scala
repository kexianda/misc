object FileMatcher {
	private val files = (new java.io.File(".")).listFiles

	def fileMatching(q:String, 
					matcher:(String,String)=>Boolean) = {
		for(file <- files if matcher(file.getName, q)) 
			yield file.getName
	}

	def fileEndingWith(q:String) = fileMatching(q, _ endsWith _)
	def fileContains(q:String) = fileMatching(q, _ contains _)
}

val files = FileMatcher.fileMatching(".scala", _ endsWith _ )
val files2 = FileMatcher.fileContains("scala")
files2.foreach(println)