#!/bin/bash
cp cmake-build-debug/6lab tests/solution
# shellcheck disable=SC2164
cd tests
for (( t = 1; t <= 20; t++ ));
do
	# shellcheck disable=SC2006
	a=`cat "$t".in`
	if [[ ("$a" = "") ]];
	then
		continue
	fi
	touch out.txt
	./solution < "$t.in" > out.txt
	# shellcheck disable=SC2006
	a=`diff "$t.out" -q out.txt`
	if [[ ("$a" = "") ]];
	then
		echo "Test $t OK"
	else
		echo "Test $t WA"
	fi
	rm -rf out.txt
done