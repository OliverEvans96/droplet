#!/bin/bash

p=$SCRATCH/droplet/exec/parallel.sh

if [ $# = 1 ]
then
	node=$1
else
	node=1
fi

#Compile
echo "Compiling..."
if g++ $SCRATCH/droplet/src/substrateDensity.cpp -o $SCRATCH/droplet/exec/substrateDensity.out

#Submit jobs
then
	echo "Submitting jobs..."
	$p $SCRATCH/droplet/exec/substrateDensity.out sd_20A_atom{} $node $SCRATCH/droplet/results/20A/atom{}/substrate.txt $SCRATCH/droplet/results/20A/atom{}/substrate_density_halfA.txt $SCRATCH/droplet/results/20A/atom{}/center_of_mass.txt 1 3
	#$p $SCRATCH/droplet/exec/substrateDensity.out sd_30A_atom{} $node $SCRATCH/droplet/results/30A/atom{}/substrate.txt $SCRATCH/droplet/results/30A/atom{}/substrate_density_halfA.txt $SCRATCH/droplet/results/30A/atom{}/center_of_mass.txt 2 2
	#$p $SCRATCH/droplet/exec/substrateDensity.out sd_40A_atom{} $node $SCRATCH/droplet/results/40A/atom{}/substrate.txt $SCRATCH/droplet/results/40A/atom{}/substrate_density_halfA.txt $SCRATCH/droplet/results/40A/atom{}/center_of_mass.txt 3 10
	#$p $SCRATCH/droplet/exec/substrateDensity.out sd_50A_atom{} $node $SCRATCH/droplet/results/50A/atom{}/substrate.txt $SCRATCH/droplet/results/50A/atom{}/substrate_density_halfA.txt $SCRATCH/droplet/results/50A/atom{}/center_of_mass.txt 1 12
	#$p $SCRATCH/droplet/exec/substrateDensity.out sd_60A_atom{} $node $SCRATCH/droplet/results/60A/atom{}/substrate.txt $SCRATCH/droplet/results/60A/atom{}/substrate_density_halfA.txt $SCRATCH/droplet/results/60A/atom{}/center_of_mass.txt 1 5
	echo "Done submitting jobs!"
else
	echo "Compilation error - terminating"
fi
