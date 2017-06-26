

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
        config("spark.sql.parquet.compression.codec", "snappy")
        .getOrCreate()

      // uncompressed
      //sc.hadoopConfiguration.setInt( "dfs.blocksize", blockSize)
      //sc.hadoopConfiguration.setInt( "parquet.block.size", blockSize *8)

      val recordsNum = 1024 * 1024 * 5 
      val cal = Calendar.getInstance

      val salesRDD = spark.sparkContext.makeRDD((1 to recordsNum).map { i =>
        cal.set(2017, 6, 1)
        val r = new Random()
        val name = "name" + i % 1000
        val item = "item" + i % 50
        val quantity = r.nextInt(100) + 1
        val price = 1000 + (r.nextDouble() * 100)
        val date = new java.sql.Date(cal.getTimeInMillis)
        Sales(name, item, quantity, price, price * quantity, date, date)
      }, 1)

//      val slices = 10
//      val numPerPartion = 10
//      val rdd = spark.sparkContext.parallelize((1 to slices), slices)
//      val salesRDD = rdd.flatMap { p =>
//        (1 to numPerPartion).map { i =>
//          cal.set(2017, 5, p % 10 + 1)
//          val r = new Random()
//          val name = "name" + i % 1000
//          val item = "item" + i % 50
//          val quantity = r.nextInt(100) + 1
//          val price = 1000 + (r.nextDouble() * 100)
//          val date = new java.sql.Date(cal.getTimeInMillis)
//          Sales(name, item, quantity, price, price * quantity, date, date)
//        }
//      }
//
//      val salesRDD = spark.sparkContext.makeRDD( (1 to recordsNum).map(
//        i => {
//          cal.set(2017, 6, i%10 + 1)
//          val r = new Random()
//          val name = "name" + i % 1000
//          val item = "item" + i % 50
//          val quantity = r.nextInt(100) + 1
//          val price = 1000 + (r.nextDouble() * 100)
//          val date = new java.sql.Date(cal.getTimeInMillis)
//          Sales(name, item, quantity, price, price * quantity, date, date)
//        }), 1)

      import spark.implicits._
      val salesDS = spark.sqlContext.createDataset(salesRDD)
      salesDS.write.partitionBy("doc_date").parquet("testdata.parquet")

      // query some
      val allSales = spark.read.parquet("testdata.parquet")
      allSales.createOrReplaceTempView("sales")
      val q1DF = spark.sql("SELECT * FROM sales WHERE doc_date = '2017-07-01' ")
      q1DF.show()
      //q1DF.write.

//      val q2 = spark.sql("INSERT INTO sales SELECT * FROM sales")
//      q2.write.mode("append").partitionBy("doc_date").parquet("testdata.parquet")
      spark.stop()
    }
    catch {
      case e: Exception => println(e.getMessage)
    }
  }
}

