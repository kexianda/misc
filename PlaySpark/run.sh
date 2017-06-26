
hdfs dfs -rm -r testdata.parquet

$SPARK_HOME/bin/spark-submit --executor-memory 8g --driver-memory 8g ./target/scala-2.11/playspark_2.11-1.0.jar

