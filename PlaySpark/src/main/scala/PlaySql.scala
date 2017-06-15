

import org.apache.spark.sql.SparkSession
import java.util.Calendar

import scala.util.Random
/**
  * Created by xianda on 17-6-14.
  */
object PlaySql {
  def main(args:Array[String]): Unit = {
    try {
      val spark = SparkSession
        .builder().
        appName("investigating index").
        config("spark.sql.parquet.compression.codec", "uncompressed")
        .getOrCreate()

      //sc.hadoopConfiguration.setInt( "dfs.blocksize", blockSize)
      //sc.hadoopConfiguration.setInt( "parquet.block.size", blockSize *8)

      val recordsSize = 1024 * 1024
      val cal = Calendar.getInstance

      val salesRDD = spark.sparkContext.makeRDD( (1 to recordsSize).map(
        i => {
          cal.set(2017, 6, i%10 + 1)
          val r = new Random()
          val name = "name" + i % 1000
          val item = "item" + i % 50
          val quantity = r.nextInt(100) + 1
          val price = 1000 + (r.nextDouble() * 100)
          val date = new java.sql.Date(cal.getTimeInMillis)
          Sales(name, item, quantity, price, price * quantity, date, date)
        }))

      import spark.implicits._
      val salesDS = spark.sqlContext.createDataset(salesRDD)
      //salesDS.show()

      salesDS.write.partitionBy("doc_date").parquet("testdata.parquet")
    }
    catch {
      case e: Exception => println(e.getMessage)
    }
  }
}

