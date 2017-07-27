#/bin/bash

#2 arguments
#Input file & output folder in sapphire/analysis

if [[ $1 == "compile" ]]
then
	#Compile C++ files
	echo "Compiling source files"
	g++ $SCRATCH/droplet/src/sortWaters.cpp -o $SCRATCH/droplet/exec/sortWaters.out
	g++ $SCRATCH/droplet/src/calculations.cpp -o $SCRATCH/droplet/exec/calculations.out

elif [[ $# == 2 ]]
then 
	#Source
	source_file=$1
	#Destination
	out_folder=$2
	
	echo "Destination: $out_folder"

	#Make destination
	mkdir -p $out_folder

	#Extract water atoms from large file
	echo "Not extracting water atoms"
	#$SCRATCH/droplet/src/removeSubstrate.awk $source_file $out_folder/waters.txt

	#Sort water atoms by id
	echo "Sorting atoms"
	$SCRATCH/droplet/exec/sortWaters.out $out_folder/waters.txt $out_folder/waters_sorted.txt

	#Calculate dipole angles and velocities
	echo "Calculating dipole angles and velocities"
	$SCRATCH/droplet/exec/calculations.out $out_folder/waters_sorted.txt $out_folder/calculated.txt

	#:)
	echo "Done!"

else
	echo "2 arguments required! - Input file & output folder name in sapphire/analysis"
	echo "$# arguments given"

fi
