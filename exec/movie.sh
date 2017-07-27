#!/bin/bash

#movie.sh
#Create a movie from files in separate directories

#arg1 - sim
#arg2 - name (of directory and movie)

#e.g. ./movie.sh 50A vr

#Arguments
sim=$1
name=$2

ext=png

#:)
folder=movie_files_${name}
cd $SCRATCH/droplet/results/$sim

#Create directories
echo "Creating directories"
mkdir -p $folder
mkdir -p movies

#Copy images to one folder
echo "Copying images"
cp atom*/img/$name/*.$ext $folder

#Create movie
echo "Creating movie"
/home/oge1/.local/bin/ffmpeg -r 10 -i ${folder}/%*.$ext -c:v mpeg4 -qscale:v 3 -pix_fmt yuv420p -y movies/${name}.mp4
#/home/oge1/.local/bin/ffmpeg -i ${folder}/%*.$ext -pix_fmt yuv420p -r 10 -framerate 10 -y movies/${name}.mp4
#/home/oge1/.local/bin/ffmpeg -i ${folder}/%*.$ext -c:v libx264 -crf 24 -pix_fmt yuv420p -r 10 -framerate 10 -y movies/${name}.mp4

#Delete temporary files & directory
echo "Deleting temporary files"
rm -rf $folder

#Copy to movies directory
echo "Copying to movies directory"
moviename=$SCRATCH/droplet/results/movies/${name}_${sim}
mkdir -p ${moviename%/*}
cp movies/${name}.mp4 $moviename.mp4

#Done! Good job :D
echo "Done!"

