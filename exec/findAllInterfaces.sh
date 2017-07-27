#!/bin/bash
#findAllInterfaces.sh
#Find mono-bulk interface for each sim

p=$SCRATCH/droplet/exec/parallel4.sh

#Compile
echo "Compiling..."
$SCRATCH/droplet/root/compilePS.sh

#Submit jobs
echo "Submitting jobs..."
$p $SCRATCH/droplet/exec/analyze.sh fi_{}0A 1 $SCRATCH/droplet/results/{}0A/atom2/calculated.txt {}0A/atom2 polarScatter mono 2 6
echo "Done submitting jobs!"
