#!/bin/bash
#analyze.sh

#Input calculated.txt
#Output movie of 2d histogram from above

#This file takes 3 command line arguments
#Input file
#Output folder name
#Name of root analysis script

#Ex:
# ./exec/analyze.sh $SCRATCH/droplet/results/50A/atom1/calculated.txt 50A/atom7 polarScatter

echo "Starting analyze.sh"
date +"%a. %b. %d, %Y - %I:%M:%S %p"

echo "Creating Directory"
out_folder="$SCRATCH/droplet/results/$2"
mkdir -p $out_folder/img/dipole $out_folder/img/hist
echo "Made directory: $out_folder"
cd $out_folder

#Run root analysis
echo "Running $3"
echo $SCRATCH/droplet/root/$3.out $1 $out_folder $4
$SCRATCH/droplet/root/$3.out $1 $out_folder $4

#Record that the *StepData.txt file have not been adjusted to correct flux
echo "Unadjusting"
rm -f adjusted.bool

echo "Analyze Done!"
date +"%a. %b. %d, %Y - %I:%M:%S %p"

#echo "Creating movie"
#Create movies
#/home/oge1/software/bin/ffmpeg -framerate 10 -i img/hist/%*.png -vcodec mpeg4 -b 800k -r 30  hist.avi -y
#/home/oge1/software/bin/ffmpeg -framerate 10 -i img/quiver/%*.png -vcodec mpeg4 -b 800k -r 30  quiver.avi -y

