 
 -- testSkewedJoinWithGroup
 A = load 'SkewedJoinUT1.txt' as (id, name, n);
 B = load 'SkewedJoinUT2.txt' as (id, name);
 C = group A by id;
 D = group B by id;
 E = join C by group, D by group using 'skewed' parallel 5;
 dump E;

--A = load 'SkewedJoinInput_small.txt' as (id:int, name, n);
-- B = load 'SkewedJoinInput2.txt' as (id:int, name, tinfo);
-- C = join A by (id, name), B by (id, name) using 'skewed' parallel 7;
--C = join A by id, B by id using 'skewed' parallel 7;
-- dump C;

-- plain join
-- A = load 'joinleft.txt' as (id:int, name);
-- B = load 'joinright.txt' as (id:int, name);
-- C = join A by id, B by id;
-- dump C;
