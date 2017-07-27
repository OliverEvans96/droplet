#!/bin/bash

#parallel.sh

#Run a script with 50A/atom$x

#Arguments:
#Script
#Node
#Arguments ({} replaced by number)
#Low
#High

script=$1
name=$2
node=$3
arg1=$4
arg2=$5
arg3=$6
arg4=$7
low=$8
high=$9

i=1
for arg in "$@"
do
	#echo "$i: $arg"
	i=$((i+1))
done

for i in $(seq $low $high)
do
	arg1_replaced=${arg1/'{}'/$i}
	arg2_replaced=${arg2/'{}'/$i}
	arg3_replaced=${arg3/'{}'/$i}
	arg4_replaced=${arg4/'{}'/$i}
	name_replaced=${name/'{}'/$i}

	#Write new sub_script
	sed -e "s@\${NODE}@$node@g" -e "s@\${PROC}@1@g" -e "s@\${NAME}@$name_replaced@g" -e "s@\${SCRIPT}@$script@g" -e "s@\${ARG1}@$arg1_replaced@g" -e "s@\${ARG2}@$arg2_replaced@g" -e "s@\${ARG3}@$arg3_replaced@g" -e "s@\${ARG4}@$arg4_replaced@g" $SCRATCH/droplet/sub_scripts/sub_script_template4 &> $SCRATCH/droplet/sub_scripts/tmp_sub_script

	#Submit the sub_script
	qsub $SCRATCH/droplet/sub_scripts/tmp_sub_script
	#cat $SCRATCH/droplet/sub_scripts/tmp_sub_script
	#echo
done
