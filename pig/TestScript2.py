#!/usr/bin/python
from org.apache.pig.scripting import *
input = 'simple_table_7'
Pig.fs("rmr simple_out")
Pig.fs("rmr simple_out2")
Pig.fs("rmr simple_out3")
output1 = 'simple_out'
output2 = 'simple_out2'
output3 = 'simple_out3'
P = Pig.compile("mypipeline", """a = load '$input';
b = foreach a generate $0, $0;
store b into '$output';""")
Q = P.bind([{'input':input, 'output':output1}, {'input':input, 'output':output2}, {'input':input, 'output':output3}])
stats = Q.run()
