#!/bin/bash

#movie.sh
#Create a movie from files in separate directories

#arg1 - sim
#arg2 - name (of directory and movie)

#e.g. ./movie.sh 50A vr

#Arguments
sim=$1
name=$2

#:)
folder=movie_files_${name}
cd $SCRATCH/droplet/results/$sim

#Create directories
echo "Creating directories"
mkdir $folder
mkdir -p movies

#Copy images to one folder
echo "Copying images"
cp atom*/img/$name/*.ppm $folder

#Create movie
echo "Creating movie"
/home/oge1/.local/bin/ffmpeg -framerate 10 -i ${folder}/%*.ppm -vcodec mpeg4 -b 800k -r 10  movies/${name}.mp4 -y

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

