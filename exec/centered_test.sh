#/bin/bash

#2 arguments
#Input file & output folder in sapphire/analysis/results

if [[ $1 == "compile" ]]
then
	#Compile C++ files
	echo "Compiling source files"
	g++ $SCRATCH/droplet/src/calculations.cpp -o $SCRATCH/droplet/exec/calculations.out
	g++ $SCRATCH/droplet/src/calculations_centered.cpp -o $SCRATCH/droplet/exec/calculations_centered.out

else
	#Source
	source_file=$SCRATCH/droplet/src/test.dat
	#Destination
	out_file=$SCRATCH/droplet/src/results.dat
	
	echo "Destination: $out_file"

	#Calculate dipole angles and velocities
	echo "Calculating dipole angles and velocities"
	$SCRATCH/droplet/exec/calculations.out $source_file $out_file
	echo
	$SCRATCH/droplet/exec/calculations_centered.out $source_file ${out_file%.*}_centered.dat

	#:)
	echo "Done!"
fi

