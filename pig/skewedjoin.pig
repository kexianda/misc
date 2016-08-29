 A = load 'SkewedJoinInput_small.txt' as (id:int, name, n);
 B = load 'SkewedJoinInput2.txt' as (id:int, name, tinfo);
 C = join A by (id, name), B by (id, name)  using 'skewed' parallel 7;
 dump C;

-- plain join
-- A = load 'joinleft.txt' as (id:int, name);
-- B = load 'joinright.txt' as (id:int, name);
-- C = join A by id, B by id;
-- dump C;
