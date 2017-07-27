#!/bin/bash
#prewet_isolate_old.sh

#Isolate old waters for one part

#Argument: dir with waters_sorted.txt
dir="$1"

#Isolate old water molecules from waters_sorted.txt
cat $dir/waters_sorted.txt | awk '!($2>5&&NF==8){print}' > $dir/waters_old.txt

#Run calculations_centered
$SCRATCH/droplet/exec/calculations_centered.out $dir/waters_old.txt $dir/calculated.txt

