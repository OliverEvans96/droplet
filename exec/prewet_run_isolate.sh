#!/bin/bash
#prewet_run_isolate.sh

#Run prewet_isolate_new.sh and prewet_isolate_old.sh for all parts

#Parallel script
parallel='$SCRATCH/droplet/exec/parallel.sh'

#Argument
script_prefix='$SCRATCH/droplet/exec/prewet_isolate'
name=''
node='bethel'
arg1=''
arg2=''
arg3=''
low='1'
high='1'

dir="$SCRATCH/droplet/results/Bob/Prewet"

#Compile
echo "Compiling"
$SCRATCH/droplet/exec/parse.sh compile
$SCRATCH/droplet/root/compilePS.sh

#Isolate new
echo "Isolating New"
files=($(find "$dir/New" -maxdepth 2 -type d | egrep 'atom'))
for file in "${files[@]}"
do
	name="Isolate_Prewet/${file##*/Prewet/}"
	arg1=$file
	$parallel "${script_prefix}_new.sh" "$name" "$node" "$arg1" "$arg2" "$arg3" "$low" "$high"
done

echo

#Isolate old
echo "Isolating Old"
files=($(find "$dir/Old" -maxdepth 2 -type d | egrep 'atom'))
for file in "${files[@]}"
do
	name="Isolate_Prewet/${file##*/Prewet/}"
	arg1=$file
	$parallel "${script_prefix}_old.sh" "$name" "$node" "$arg1" "$arg2" "$arg3" "$low" "$high"
done

echo 'Done!'
