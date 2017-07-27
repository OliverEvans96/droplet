#!/bin/bash

p=$SCRATCH/droplet/exec/parallel.sh
node=bethel

#Compile
echo "Compiling..."
$SCRATCH/droplet/root/compilePS.sh

#Submit jobs
echo "Submitting jobs..."

##Both
#echo "Both"
#$p $SCRATCH/droplet/exec/analyze.sh Prewet/Both/20A_20AonMonolayer/atom{} $node $SCRATCH/droplet/results/Bob/Prewet/Both/20A_20AonMonolayer/atom{}/calculated.txt Bob/Prewet/Both/20A_20AonMonolayer/atom{}  polarScatter 1 3
#$p $SCRATCH/droplet/exec/analyze.sh Prewet/Both/30A_20AonMonolayer/atom{} $node $SCRATCH/droplet/results/Bob/Prewet/Both/30A_20AonMonolayer/atom{}/calculated.txt Bob/Prewet/Both/30A_20AonMonolayer/atom{} polarScatter 1 3
#$p $SCRATCH/droplet/exec/analyze.sh Prewet/Both/30A_30AonMonolayer/atom{} $node $SCRATCH/droplet/results/Bob/Prewet/Both/30A_30AonMonolayer/atom{}/calculated.txt Bob/Prewet/Both/30A_30AonMonolayer/atom{} polarScatter 1 6
#

#New
echo "New"
$p $SCRATCH/droplet/exec/analyze.sh Prewet/New/20A_20AonMonolayer/atom{} $node $SCRATCH/droplet/results/Bob/Prewet/New/20A_20AonMonolayer/atom{}/calculated.txt Bob/Prewet/New/20A_20AonMonolayer/atom{}  polarScatter 1 3
$p $SCRATCH/droplet/exec/analyze.sh Prewet/New/30A_20AonMonolayer/atom{} $node $SCRATCH/droplet/results/Bob/Prewet/New/30A_20AonMonolayer/atom{}/calculated.txt Bob/Prewet/New/30A_20AonMonolayer/atom{} polarScatter 1 3
$p $SCRATCH/droplet/exec/analyze.sh Prewet/New/30A_30AonMonolayer/atom{} $node $SCRATCH/droplet/results/Bob/Prewet/New/30A_30AonMonolayer/atom{}/calculated.txt Bob/Prewet/New/30A_30AonMonolayer/atom{} polarScatter 1 6

#Old
echo "Old"
$p $SCRATCH/droplet/exec/analyze.sh Prewet/Old/20A_20AonMonolayer/atom{} $node $SCRATCH/droplet/results/Bob/Prewet/Old/20A_20AonMonolayer/atom{}/calculated.txt Bob/Prewet/Old/20A_20AonMonolayer/atom{}  polarScatter 1 3
$p $SCRATCH/droplet/exec/analyze.sh Prewet/Old/30A_20AonMonolayer/atom{} $node $SCRATCH/droplet/results/Bob/Prewet/Old/30A_20AonMonolayer/atom{}/calculated.txt Bob/Prewet/Old/30A_20AonMonolayer/atom{} polarScatter 1 3
$p $SCRATCH/droplet/exec/analyze.sh Prewet/Old/30A_30AonMonolayer/atom{} $node $SCRATCH/droplet/results/Bob/Prewet/Old/30A_30AonMonolayer/atom{}/calculated.txt Bob/Prewet/Old/30A_30AonMonolayer/atom{} polarScatter 1 6
echo "Done submitting jobs!"
