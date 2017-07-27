#!/bin/bash
#prewet_isolate_new.sh

#Isolate new waters for one part

#Argument: dir with waters_sorted.txt
dir="$1"
cd $dir

#Isolate new water molecules from waters_sorted.txt
cat $dir/waters_sorted.txt | awk '!($2<=5&&NF==8){print}' > $dir/waters_new.txt

#Run calculations_centered
$SCRATCH/droplet/exec/calculations_centered.out $dir/waters_new.txt $dir/calculated.txt

