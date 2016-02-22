#!/usr/bin/env bash

# if dfs is not started up, start it.
cnt=`jps | grep -c -E "DataNode| NameNode|SecondaryNameNode"`
if [ $cnt != 3 ]; then 
	echo "Starting dfs..."
	bash $HADOOP_HOME/sbin/start-dfs.sh
	sleep 10
fi 
cnt=`jps | grep -c -E "ResourceManager|NodeManager"`
if [ $cnt != 2 ]; then
	echo "Starting yarn..."
	bash $HADOOP_HOME/sbin/start-yarn.sh
	sleep 5
fi


#hadoop fs -rm -r /user/xiandake/input
#hadoop fs -copyFromLocal input /user/xiandake/input

#hadoop fs -rm -r /user/xiandake/input
#hadoop fs -rm -r /user/xiandake/1.txt


#hadoop fs -mkdir /user/xiandake/tmp
#sleep 5
