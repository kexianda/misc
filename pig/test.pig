--TestCollectedGroup
--REGISTER 'hdfs://myudfs.jar';
REGISTER myudfs.jar;
A = LOAD 'collectedgroup1' USING myudfs.DummyCollectableLoader() AS (id);
B = GROUP A by $0 USING 'collected';
C = GROUP B by $0 USING 'collected';
DUMP C;

--watching endOfAllInput in POForEach
--A = LOAD 'collectedgroup1' USING myudfs.DummyCollectableLoader() AS (id);
--B = FOREACH A GENERATE $0;
--C = GROUP B by $0 USING 'collected';
--DUMP C;

--A = LOAD 'mjtest.txt' USING myudfs.DummyCollectableLoader() AS (id, name, grade);
--B = LOAD 'mjright.txt' USING myudfs.DummyCollectableLoader() AS (id, name, grade);
--C = JOIN A BY id, B BY id  USING 'merge';  --merge join
--D = GROUP C BY A::id USING 'collected';
--DUMP D;
--merge join test case 2
--A = LOAD 'mcgleft.txt' using myudfs.DummyCollectableLoader() as (c1:chararray, c2:int);
--B = LOAD 'mcgright.txt' using myudfs.DummyIndexableLoader()  as (c1:chararray, c2:int);
--C = JOIN A BY c1, B BY c1 using 'merge';
--dump C;

--merge cogroup---------
--A = LOAD 'mcgleft.txt' using myudfs.DummyCollectableLoader() as (c1:chararray, c2:int);
--B = LOAD 'mcgright.txt' using myudfs.DummyIndexableLoader()  as (c1:chararray, c2:int);
--C = COGROUP A BY c1, B BY c1 using 'merge';
--dump C;


--FRJoin
--A = LOAD 'mjtest.txt' as (id, name , grade);
--B = LOAD 'mjright.txt' as (id, name, grade);
--C = JOIN A by (id, name), B by (id, name) using 'replicated';
--dump C;

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
