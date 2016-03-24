

REGISTER myudfs.jar;
A = LOAD 'collectedgroup1' USING myudfs.DummyCollectableLoader() AS (id);
B = GROUP A by $0 USING 'collected';
--C = FOREACH B GENERATE $0, $0;
DUMP B;
D = STREAM B THROUGH ` awk '{
	print $1"\tdd";
}'`;

DUMP D;
