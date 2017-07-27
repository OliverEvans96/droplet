#!/bin/bash
#subSub.sh
#submit subWhenDone.sh using parallel.sh

script1=$1
script2=$2
node=$3
dir="$SCRATCH/droplet/exec"

$SCRATCH/droplet/exec/parallel.sh subWhenDone.sh "SWD_${script1:0:5}_${script2:0:5}" bethel $dir/$script1 $dir/$script2 $node 0 0
