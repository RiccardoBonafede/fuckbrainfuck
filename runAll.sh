#!/bin/env bash

testdir="tests/"

echo "[*]starting tests...this may take few seconds..."

testcases=`ls ${testdir}*.b`
input=`ls ${testdir}*.in` 
results=`ls ${testdir}*.out`

for testcase in `ls ${testdir}*.b`;
	 do
	 	echo "[*]Running $testcase"
		if [ -e "${testcase:0:2}.in" ]; then
			output=`$(cat ${testcase::-2}.in) | ./bf $testcase`;
		else
			output=`./bf $testcase`;
		fi

		if [ "$output" == "$(cat ${testcase::-2}.out)" ]; then
			echo "[*]$testcase succeed"
		else
			echo "[!]$testcase failed. Expeted `cat ${testcase::-2}.out` find $output"
		fi
	done;
