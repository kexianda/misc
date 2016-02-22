cp=`dirname ~/git/pig/build/ivy/lib/Pig/hadoop-common-2.5.0.jar`/hadoop-common-2.5.0.jar
cp=${cp}:`dirname ~/git/pig/build/pig-0.15.0-SNAPSHOT.jar`/pig-0.15.0-SNAPSHOT.jar
echo ${cp}

javac -cp ${cp} ./myudfs/DummyCollectableLoader.java

jar -cf myudfs.jar myudfs
