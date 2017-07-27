#!/bin/bash
#parseAndAnalyze.sh

#One argument - sim/part (e.g. 50A/atom5)
#File is assumed to be located in $SCRATCH/droplet/data
data_file=$1

echo "Parsing File"
#Parse
$SCRATCH/droplet/exec/parseFromSort.sh $SCRATCH/droplet/data/${data_file} $SCRATCH/droplet/results/${data_file} > $SCRATCH/droplet/logs/pnA${data_file}.txt

wait

echo "2d Density Analysis"
#2D density
$SCRATCH/droplet/exec/analyze.sh $SCRATCH/droplet/${data_file}/calculated.txt ${data_file} topDensity

echo "3d Scatter Analysis"
#3D scatter
$SCRATCH/droplet/exec/analyze.sh $SCRATCH/droplet/${data_file}/calculated.txt ${data_file} scatter3d
