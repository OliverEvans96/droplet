#!/bin/bash
#subWhenDone.sh

#Both should be executable files that submit jobs using parallel.sh which take node as only argument
script=$1
node=$2

#Call first script and capture job list
num=1

while (( num>0 ))
do
	num=$(qstat | grep -c oge1)
	sleep 30
	echo $num
done

echo "Previous jobs are done! submitting next!"

/bin/bash $2 $node

echo "All jobs done now!"

