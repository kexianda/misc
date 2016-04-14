register myudfs.jar;

A = load 'endInput.txt' using myudfs.DummyCollectableLoader() as (c1:chararray, c2:chararray);
B = load 'endInput.txt' using myudfs.DummyIndexableLoader() as (c1:chararray, c2:chararray);
C = cogroup A by (c1,c2), B by(c1, c2) using 'merge';
D = group C by $0 using 'collected';
dump D;
--E = stream C through ` awk '{ print $0 }'`;
--dump E;
