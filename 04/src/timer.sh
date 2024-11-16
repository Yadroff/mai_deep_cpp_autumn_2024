#!/bin/bash
cp cmake-build-debug/6lab tests/solution
# shellcheck disable=SC2164
cd tests
for (( t = 1; t <= 1; t++ ));
do
	# shellcheck disable=SC2006
	a=`cat "$t".in`
	if [[ ("$a" = "") ]];
	then
		continue
	fi
	touch out.txt
	T="$(date +%s%N)"
	./solution < "$t.in" > out.txt
	T="$(($(date +%s%N)-T))"
	M="$((T/1000000))"
  echo $M
	rm -rf out.txt
done