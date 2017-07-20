#!/bin/bash

#Count the number of timesteps in each part of each sim.

for sim in $(ls | grep 0A)
do
	for part in $(ls $sim | grep atom)
	do
		n=$(mawk -f nTimesteps.awk "$sim/$part/calculated.txt")
		echo "$sim/$part: $n timesteps"
	done
	echo
done
