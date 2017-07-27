#!/bin/bash

#Unzip Sapphire files

sim=$1 #e.g. 50A
part=$2 #e.g. atom1
dir=$SCRATCH/droplet/data/Bob/Prewet/${sim}

mkdir -p $dir
cd $dir

#echo "Copying Files"
cp /home/mtsige/Bob/Sub3/${sim}/${part}.bz2 .

echo "Decompressing Files"
bunzip2 -dv ${part}.bz2

