// Ch32.scala
// scala.actor

import scala.actors._

object SillyActor extends Actor {
	def act() {
		println("I'm acting!")
		Thread.sleep(1000)
	}
}
SillyActor.start