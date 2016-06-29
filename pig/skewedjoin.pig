A = load 'SkewedJoinInput1.txt' as (id, name, n);
B = load 'SkewedJoinInput2.txt' as (id, name);
C = join A by id, B by id using 'skewed' parallel 7;
dump C;
