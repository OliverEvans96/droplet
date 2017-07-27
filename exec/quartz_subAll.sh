#!/bin/bash

p=$SCRATCH/droplet/exec/parallel.sh
node=bethel

#Compile
echo "Compiling..."
$SCRATCH/droplet/root/compileCYL.sh

#Quartz/mit jobs
echo "Submitting jobs..."

#120By120
echo "120By120"
#$p $SCRATCH/droplet/exec/analyze.sh ps_Bob/Quartz/120By120/20A_atom{} $node $SCRATCH/droplet/results/Bob/Quartz/120By120/20A/atom{}/calculated.txt Bob/Quartz/120By120/20A/atom{} polarScatter 1 3
$p $SCRATCH/droplet/exec/analyze.sh ps_Bob/Quartz/120By120/25A_atom{} $node $SCRATCH/droplet/results/Bob/Quartz/120By120/25A/atom{}/calculated.txt Bob/Quartz/120By120/25A/atom{} polarScatter 1 3

#240By240
echo
echo "240By240"
#$p $SCRATCH/droplet/exec/analyze.sh ps_Bob/Quartz/240By240/30A_atom{} $node $SCRATCH/droplet/results/Bob/Quartz/240By240/30A/atom{}/calculated.txt Bob/Quartz/240By240/30A/atom{} polarScatter 1 4
#$p $SCRATCH/droplet/exec/analyze.sh ps_Bob/Quartz/240By240/35A_atom{} $node $SCRATCH/droplet/results/Bob/Quartz/240By240/35A/atom{}/calculated.txt Bob/Quartz/240By240/35A/atom{} polarScatter 1 4
#$p $SCRATCH/droplet/exec/analyze.sh ps_Bob/Quartz/240By240/40A_atom{} $node $SCRATCH/droplet/results/Bob/Quartz/240By240/40A/atom{}/calculated.txt Bob/Quartz/240By240/40A/atom{} polarScatter 1 4
#$p $SCRATCH/droplet/exec/analyze.sh ps_Bob/Quartz/240By240/45A_atom{} $node $SCRATCH/droplet/results/Bob/Quartz/240By240/45A/atom{}/calculated.txt Bob/Quartz/240By240/45A/atom{} polarScatter 1 4
#$p $SCRATCH/droplet/exec/analyze.sh ps_Bob/Quartz/240By240/50A_atom{} $node $SCRATCH/droplet/results/Bob/Quartz/240By240/50A/atom{}/calculated.txt Bob/Quartz/240By240/50A/atom{} polarScatter 1 4

#403By51
echo
echo "403By51"
#$p $SCRATCH/droplet/exec/analyze.sh ps_Bob/Quartz/403By51/30A_atom{} $node $SCRATCH/droplet/results/Bob/Quartz/403By51/30A/atom{}/calculated.txt Bob/Quartz/403By51/30A/atom{} polarScatter 1 5

echo "Done submitting jobs!"
