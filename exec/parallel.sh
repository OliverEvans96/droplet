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
low=$7
high=$8

#Print arguments received
echo "parallel.sh:"
echo "script=$script"
echo "name=$name"
echo "node=$node"
echo "arg1=$arg1"
echo "arg2=$arg2"
echo "arg3=$arg3"
echo "low=$low"
echo "high=$high"
echo

i=1
for arg in "$@"
do
	#echo "$i: $arg"
	i=$((i+1))
done

for i in $(seq "$low" "$high")
do
	arg1_replaced=${arg1/'{}'/$i}
	arg2_replaced=${arg2/'{}'/$i}
	arg3_replaced=${arg3/'{}'/$i}
	name_replaced=${name/'{}'/$i}


	#Write new sub_script
	sed -e "s@\${NODE}@$node@g" -e "s@\${PROC}@1@g" -e "s@\${NAME}@$name_replaced@g" -e "s@\${SCRIPT}@$script@g" -e "s@\${ARG1}@$arg1_replaced@g" -e "s@\${ARG2}@$arg2_replaced@g" -e "s@\${ARG3}@$arg3_replaced@g" $SCRATCH/droplet/sub_scripts/sub_script_template &> $SCRATCH/droplet/sub_scripts/tmp_sub_script

	#Make sure log directory exists
	log_dir="$SCRATCH/droplet/logs/${name_replaced%/*}"
	#Check whether name contains a slash
	if [[ "$name_replaced" =~ "/" ]]
	then
		mkdir -p $log_dir
	fi

	#Submit the sub_script
	qsub $SCRATCH/droplet/sub_scripts/tmp_sub_script
	#cat $SCRATCH/droplet/sub_scripts/tmp_sub_script
	#echo
done
