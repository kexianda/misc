#!/usr/bin/env bash
rm pig_*.log
hadoop fs -rm -r /user/xiandake/output
#hadoop fs -rm -r /user/xiandake/output2
#hadoop fs -rm -r /user/xiandake/output3
#hadoop fs -rm -r /user/xiandake/tmp
#hadoop fs -rm -r /user/xiandake/output_0
#hadoop fs -rm -r /user/xiandake/output_1
#hadoop fs -mkdir /user/xiandake/tmp/
#rm -f pig*.log
#sleep 5s
echo "teardown...  finished."
