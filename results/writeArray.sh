#!/bin/bash
#writeZeros.sh
#Print a 2d array of numbers

#Required arguments
nRows=$1
nCols=$2

#Optional Arguments
num=$3 
nDigits=$4 

#Default to zero if not specified
if [ -z "$3" ]
then
	num=0
fi

if [ -z "$4" ]
then
	nDigits=0
fi

#Create row array
rowArr=( $(for i in `seq 1 $nCols`; do echo $num; done) )

for i in `seq 1 $nRows`
do
	printf "%.${nDigits}f\t" "${rowArr[@]}"
	printf "\n"
done
