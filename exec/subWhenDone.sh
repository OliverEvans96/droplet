#!/bin/bash
#subWhenDone.sh

#Both should be executable files that submit jobs using parallel.sh which take node as only argument
script1=$1
script2=$2
node=$3

#Call first script and capture job list
list=$(seq 199670 199683) # | awk 'BEGIN{FS="."}/bethel/{print $1}')
list=$(printf "'";printf "%s|" $list;printf "'")

num=1

while (( num>0 ))
do
	num=$(qstat | grep -cE $list)
	sleep 30
	echo $num
done

echo "Previous jobs are done! submitting next!"

/bin/bash $2 $node

echo "All jobs done now!"

