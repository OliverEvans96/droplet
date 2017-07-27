#!/bin/bash

p=$SCRATCH/droplet/exec/parallel.sh

if [ $# = 1 ]
then
	node=$1
else
	node=1
fi

#Submit jobs
echo "Submitting jobs..."
#$p $SCRATCH/droplet/src/isolateSubstrate.awk is_20A_atom{} $node $SCRATCH/droplet/data/20A/atom{} $SCRATCH/droplet/results/20A/atom{}/substrate.txt "" 2 3
#$p $SCRATCH/droplet/src/isolateSubstrate.awk is_30A_atom{} $node $SCRATCH/droplet/data/30A/atom{} $SCRATCH/droplet/results/30A/atom{}/substrate.txt "" 1 2
$p $SCRATCH/droplet/src/isolateSubstrate.awk is_40A_atom{} $node $SCRATCH/droplet/data/40A/atom{} $SCRATCH/droplet/results/40A/atom{}/substrate.txt "" 10 10
#$p $SCRATCH/droplet/src/isolateSubstrate.awk is_50A_atom{} $node $SCRATCH/droplet/data/50A/atom{} $SCRATCH/droplet/results/50A/atom{}/substrate.txt "" 1 3
#$p $SCRATCH/droplet/src/isolateSubstrate.awk is_60A_atom{} $node $SCRATCH/droplet/data/60A/atom{} $SCRATCH/droplet/results/60A/atom{}/substrate.txt "" 1 5
echo "Done submitting jobs!"
