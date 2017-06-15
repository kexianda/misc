export SPARK_SUBMIT_OPTS=" -agentlib:jdwp=transport=dt_socket,server=y,suspend=y,address=5005"
$SPARK_HOME/bin/spark-submit ./target/scala-2.11/playspark_2.11-1.0.jar

