A = load 'SkewedJoinInput_1000.txt' as (id:int, name, n);
B = load 'SkewedJoinInput2.txt' as (id:int, name);
C = join A by (id, name), B by ((50*2 + id -100), name) using 'skewed' parallel 7;
dump C;
