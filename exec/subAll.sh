#!/bin/bash

p=$SCRATCH/droplet/exec/parallel.sh
node1=n014
node2=n017
node3=n018

#Compile
echo "Compiling..."
$SCRATCH/droplet/root/compilePS.sh

#Submit jobs
echo "Submitting jobs..."
#$p $SCRATCH/droplet/exec/analyze.sh ps_20A_atom{} $node $SCRATCH/droplet/results/20A/atom{}/calculated.txt 20A/atom{} polarScatter 2 3
#$p $SCRATCH/droplet/exec/analyze.sh ps_30A_atom{} $node $SCRATCH/droplet/results/30A/atom{}/calculated.txt 30A/atom{} polarScatter 1 6
#$p $SCRATCH/droplet/exec/analyze.sh ps_40A_atom{} $node $SCRATCH/droplet/results/40A/atom{}/calculated.txt 40A/atom{} polarScatter 11 12
#$p $SCRATCH/droplet/exec/analyze.sh ps_50A_atom{} $node $SCRATCH/droplet/results/50A/atom{}/calculated.txt 50A/atom{} polarScatter 1 12
$p $SCRATCH/droplet/exec/analyze.sh ps_60A_atom{} $node1 $SCRATCH/droplet/results/60A/atom{}/calculated.txt 60A/atom{} polarScatter 1 24
$p $SCRATCH/droplet/exec/analyze.sh ps_60A_atom{} $node2 $SCRATCH/droplet/results/60A/atom{}/calculated.txt 60A/atom{} polarScatter 1 24
$p $SCRATCH/droplet/exec/analyze.sh ps_60A_atom{} $node3 $SCRATCH/droplet/results/60A/atom{}/calculated.txt 60A/atom{} polarScatter 1 24

# Hemisphere
#$p $SCRATCH/droplet/exec/analyze.sh ps_Hemi50A_atom{} $node $SCRATCH/droplet/results/Hemi50A/atom{}/calculated.txt Hemi50A/atom{} polarScatter 6 9
echo "Done submitting jobs!"
