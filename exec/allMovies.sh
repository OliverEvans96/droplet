#!/bin/bash

# Create all movies for all sims

movies=('hist' 'densityHalfA' 'density' 'mono') # 'quiver' 'vr' 'all' 'dipole')

script=$SCRATCH/droplet/exec/movie.sh


echo "Making movies"

for movie in ${movies[@]}
do
	echo
	echo $movie
	$SCRATCH/droplet/exec/parallel.sh $script movie_{}0A_$movie 1 {}0A $movie "" 4 4
	#$SCRATCH/droplet/exec/parallel.sh $script movie_{}0A_$movie 1 {}0A $movie "" 6 6
done

echo "Jobs submitted!"
