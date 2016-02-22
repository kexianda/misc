--TestCollectedGroup
--REGISTER 'hdfs://myudfs.jar';
REGISTER myudfs.jar;
A = LOAD 'mjtest.txt' using myudfs.DummyCollectableLoader() as (id, name, grade);
B = LOAD 'mjtest.txt' using myudfs.DummyCollectableLoader() as (id, name, grade);
C = join A by id, B by id using 'merge';
D = group C by A::id using 'collected';
dump D;


--testGetHadoopCounters
--A = load 'input' as (a0:int, a1:int, a2:int);
--B = filter A by a0 >= 4;
--C = filter A by a0 < 4;
--D = group C by a0;
--E = foreach D generate group, COUNT(C);
--store B into 'output_0';
--store E into 'output_1';

--tesatDisablePigCounters2
--A = load 'input' as (a0:int, a1:int, a2:int);
--B = filter A by a0 > 3;
--store A into 'output';
--store B into 'tmp/output';


--simpleTest
--A = load 'input' as (a0:int, a1:int,a2:int);
--B = group A by a0;
--C = foreach B generate group, COUNT(A);
--store C into 'output';

--testEmtypFileCounter2  multiquery
--A = load 'input' as (a0:int, a1:int, a2:int);
--B = filter A by a0 <  0;
--store A into 'output';
--store B into 'output2';

--TestPigRunner.returnCodeTest
--A = load 'nonexist-input' as (a0:int, a1:int, a2:int);
--B = filter A by a0 > 0;
--C = group B by $0;
--D = join C by $0, B by $0;
--store D into 'output';

--Testing Spark Counter
-- multi job
--a = load 'input' using PigStorage('\t') as (a0:int, a1:int, a2:int);
--store a into 'output';

--b = load '1.txt' using PigStorage('\t') as (a0:int, a1:int, a2:int);
--store b into 'output2';
--c = load 'input' using PigStorage('\t') as (a0:int, a1:int, a2:int);
--d = join b by a0, c by a0;
--store d into 'output3';



--testStopOnFailure
--A1 = load 'input' using PigStorage('\t');
--B1 = load 'nonexist' using PigStorage('\t');
--C1 = load 'input' using PigStorage('\t');
--A2 = distinct A1;
--B2 = distinct B1;
--C2 = distinct C1;
--ret = union A2, B2, C2;
--store ret into 'tmp/output';



--Testing Spark Counter output
--case1 multi job
--a = load 'input' using PigStorage('\t') as (a0:int, a1:int, a2:int);
--store a into 'output';
--b = load '1.txt' using PigStorage('\t') as (a0:int, a1:int, a2:int);

--store b into 'output2';
--c = load 'input' using PigStorage('\t') as (a0:int, a1:int, a2:int);
--d = join b by a0, c by a0;
--store d into 'tmp/output';

--case 2: multi query. 
--a = load 'input' using PigStorage('\t') as (a0:int, a1:int, a2:int);
--store a into 'output';
--b = filter a by a0 > 2;
--store b into 'output2';

--Test Input Record Counter
--a = load 'input' using PigStorage('\t') as (a0:int, a1:int, a2:int);
--store a into 'output';
--b = load '1.txt' using PigStorage('\t') as (a0:int, a1:int, a2:int);
--store b into 'output2';
--c = join a by a0, b by a0;
--store c into 'tmp/output';


-- for testing
--A = LOAD 'input' USING PigStorage(',') AS (a0:int, a1:int, a2:int);
--B = FILTER A BY a0 > 3;
--dump B;

--a = load 'input' using PigStorage('\t') as (a0:int, a1:int, a2:int);
--b = filter a by a0 < 5;
--c = filter a by a0 >= 5;
--d = join b by a0, c by a0;
--dump d;

--TestPigRunner.testEmptyFileCounter()  --spark accumulator
--a = load 'input' using PigStorage('\t') as (a0:int, a1:int, a2:int);
--b = load '1.txt' using PigStorage('\t') as (a0:int, a1:int, a2:int);
--c = join a by a0, b by a0;
--d = filter a by a0 < 5;
--store c into 'output';
--store c into 'output2';

--test Counter & PigStatusReporter
--a = load 'input' using PigStorage('\t') as (a0:int, a1:int, a2:int);
--b = FOREACH a GENERATE a0, a1;
--store b into 'output';

----TestPigRunner.simpleTest()
--A = load 'input' using PigStorage('\t') as (a0:int, a1:int, a2:int);
--B = group A by a0;
--C = foreach B generate group, COUNT(A);
--store C into 'output';

--returnCodeTest
--A = load 'input' using PigStorage('\t') as (a0:int, a1:int, a2:int);
--B = filter A by a0 > 0;
--C = group B by $0;
--D = join C by  $0, B by $0;
--store D into 'output';
--store C into 'output2';
