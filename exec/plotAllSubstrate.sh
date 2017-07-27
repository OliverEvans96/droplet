#!/bin/bash

#Plot substrate for all parts in a sim in parallel

#Arg 1: sim
#Arg 2: part min
#Arg 3: part max
#Arg 4: node

#ex: ./plotAllSubstrate.sh 50A 1 12 n004

sim=$1
min=$2
max=$3
node=$4

if [ $# == 4 ]; then
	script=/home/oge1/python/miniconda/bin/python
	name=plotAll_${sim}_atom{}
	arg1='-u'
	arg2=$SCRATCH/droplet/exec/plotSubstrate2D.py
	arg3=${sim}/atom{}  

	$SCRATCH/droplet/exec/parallel.sh $script $name $node $arg1 $arg2 $arg3 $min $max
else
	echo "Please use 4 command line arguments: sim, min, max, node"
fi
