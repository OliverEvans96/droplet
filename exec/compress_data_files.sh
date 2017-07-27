#!/bin/bash
#compress_data_files.sh

#Compress unused data files using bzip2

clear
#Root directory
dir_root="$SCRATCH/droplet/results"

#Parallel script
parallel='$SCRATCH/droplet/exec/parallel.sh'

#Argument
script='bzip2'
name=''
node='bethel'
arg1=''
arg2=''
arg3=''
low='1'
high='1'

#Locate substrate.txt and waters.txt files
files=($(find $dir_root | egrep '(substrate|waters).txt'))
echo "Compressing:"

#Locate atom* files
#files=($(find $dir_root -not -path "$dir_root/Bob/Prewet/*" -regex '.*/atom[0-9]+'))

#Run script
#Loop through all files that need to be zipped
for file in ${files[@]}
do
	echo "$file"
	fname=${file#/}
	name="Compress/$(echo ${fname##*/data/} | sed 's/\//_/g')"
	arg1="$file"
	$parallel "$script" "$name" "$node" "$arg1" "$arg2" "$arg3" "$low" "$high"
	echo
done

echo 'Done!'
