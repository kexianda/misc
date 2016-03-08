
-- mergejoinright1..4
--A = LOAD 'mergejoinleft1' as (a:int, b:int);
--B = LOAD 'mergejoinright3' as (a:int, b:int);
--C = JOIN A by $0, B by $0 using 'merge';
--DUMP C;

-- mergejoin multiple splits
A2 = LOAD 'mergejoin_multisplits_left*' as (a:int);
B2 = LOAD 'mergejoin_multisplits_right*' as (a:int);
C2 = JOIN A2 BY $0, B2 BY $0 USING 'merge';
DUMP C2;
