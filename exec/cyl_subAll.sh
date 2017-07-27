#!/bin/bash

p=$SCRATCH/droplet/exec/parallel.sh
node=n010

#Compile
echo "Compiling..."
$SCRATCH/droplet/root/compileCYL.sh

#Submit jobs
echo "Submitting jobs..."

#Sub951By50 New
#echo "Sub951By50 New"
$p $SCRATCH/droplet/exec/analyze.sh cyl_Bob/Sub951By50/Cyl60A/New_atom{} $node $SCRATCH/droplet/results/Bob/Sub951By50/Cyl60A/New/atom{}/calculated.txt Bob/Sub951By50/Cyl60A/New/atom{} cyl_analysis 3 5
#$p $SCRATCH/droplet/exec/analyze.sh cyl_Bob/Sub951By50/Cyl100A/New_atom{} $node $SCRATCH/droplet/results/Bob/Sub951By50/Cyl100A/New/atom{}/calculated.txt Bob/Sub951By50/Cyl100A/New/atom{} cyl_analysis 1 2

#Sub951By100 New
echo "Sub951By100 New"
$p $SCRATCH/droplet/exec/analyze.sh cyl_Bob/Sub951By100/Cyl100A/New_atom{} $node $SCRATCH/droplet/results/Bob/Sub951By100/Cyl100A/New/atom{}/calculated.txt Bob/Sub951By100/Cyl100A/New/atom{} cyl_analysis 18 21
#$p $SCRATCH/droplet/exec/analyze.sh cyl_Bob/Sub951By100/Cyl50A_atom{} $node $SCRATCH/droplet/results/Bob/Sub951By100/Cyl50A/atom{}/calculated.txt Bob/Sub951By100/Cyl50A/atom{} cyl_analysis 1 3

#Sub951By50
#echo "Sub951By50"
#$p $SCRATCH/droplet/exec/analyze.sh cyl_Bob/Sub951By50/Cyl20A_atom{} $node $SCRATCH/droplet/results/Bob/Sub951By50/Cyl20A/atom{}/calculated.txt Bob/Sub951By50/Cyl20A/atom{} cyl_analysis 1 5
#$p $SCRATCH/droplet/exec/analyze.sh cyl_Bob/Sub951By50/Cyl30A_atom{} $node $SCRATCH/droplet/results/Bob/Sub951By50/Cyl30A/atom{}/calculated.txt Bob/Sub951By50/Cyl30A/atom{} cyl_analysis 1 5
#$p $SCRATCH/droplet/exec/analyze.sh cyl_Bob/Sub951By50/Cyl40A_atom{} $node $SCRATCH/droplet/results/Bob/Sub951By50/Cyl40A/atom{}/calculated.txt Bob/Sub951By50/Cyl40A/atom{} cyl_analysis 1 5
#$p $SCRATCH/droplet/exec/analyze.sh cyl_Bob/Sub951By50/Cyl50A_atom{} $node $SCRATCH/droplet/results/Bob/Sub951By50/Cyl50A/atom{}/calculated.txt Bob/Sub951By50/Cyl50A/atom{} cyl_analysis 1 5
#$p $SCRATCH/droplet/exec/analyze.sh cyl_Bob/Sub951By50/Cyl60A_atom{} $node $SCRATCH/droplet/results/Bob/Sub951By50/Cyl60A/atom{}/calculated.txt Bob/Sub951By50/Cyl60A/atom{} cyl_analysis 1 5
#$p $SCRATCH/droplet/exec/analyze.sh cyl_Bob/Sub951By50/Cyl100A_atom{} $node $SCRATCH/droplet/results/Bob/Sub951By50/Cyl100A/atom{}/calculated.txt Bob/Sub951By50/Cyl100A/atom{} cyl_analysis 1 5
#
#$p $SCRATCH/droplet/exec/analyze.sh cyl_Bob/Sub951By50/FullCyl20A_atom{} $node $SCRATCH/droplet/results/Bob/Sub951By50/FullCyl20A/atom{}/calculated.txt Bob/Sub951By50/FullCyl20A/atom{} cyl_analysis 1 3
#$p $SCRATCH/droplet/exec/analyze.sh cyl_Bob/Sub951By50/FullCyl30A_atom{} $node $SCRATCH/droplet/results/Bob/Sub951By50/FullCyl30A/atom{}/calculated.txt Bob/Sub951By50/FullCyl30A/atom{} cyl_analysis 1 3
##
##Sub951By100
#echo
#echo "Sub951By100"
##$p $SCRATCH/droplet/exec/analyze.sh cyl_Bob/Sub951By100/Cyl20A_atom{} $node $SCRATCH/droplet/results/Bob/Sub951By100/Cyl20A/atom{}/calculated.txt Bob/Sub951By100/Cyl20A/atom{} cyl_analysis 1 2
#$p $SCRATCH/droplet/exec/analyze.sh cyl_Bob/Sub951By100/Cyl50A_atom{} $node $SCRATCH/droplet/results/Bob/Sub951By100/Cyl50A/atom{}/calculated.txt Bob/Sub951By100/Cyl50A/atom{} cyl_analysis 1 3

echo "Done submitting jobs!"
