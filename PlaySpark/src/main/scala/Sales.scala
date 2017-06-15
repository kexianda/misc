import org.apache.spark.sql.types.DateType

/**
  * Created by xianda on 17-6-15.
  */
case class Sales (
  cust_name:String,
  item_name:String,
  item_quantity:Int,
  item_price:Double,
  total: Double,
  doc_date: java.sql.Date,
  due_date: java.sql.Date
                 )

