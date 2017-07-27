#/bin/bash

#2 arguments
#Input file & output folder in sapphire/analysis/results

if [[ $1 == "compile" ]]
then
	#Compile C++ files
	echo "Compiling source files"
	g++ $SCRATCH/droplet/src/sortWaters.cpp -o $SCRATCH/droplet/exec/sortWaters.out
	g++ $SCRATCH/droplet/src/calculations_centered.cpp -o $SCRATCH/droplet/exec/calculations_centered.out
	g++ $SCRATCH/droplet/src/quartzDensity.cpp -o $SCRATCH/droplet/exec/quartzDensity.out

elif [[ $# == 2 ]]
then 
	#Source
	source_file=$1
	#Destination
	out_folder=$SCRATCH/droplet/results/$2
	
	echo "Destination: $out_folder"

	#Make destination
	mkdir -p $out_folder

	#Extract water atoms from large file
	echo "Extracting water atoms"
	$SCRATCH/droplet/src/quartzRemoveSubstrate.awk $source_file $out_folder/waters.txt

	#Sort water atoms by id
	echo "Sorting atoms"
	$SCRATCH/droplet/exec/sortWaters.out $out_folder/waters.txt $out_folder/waters_sorted.txt
	rm $out_folder/waters.txt

	#Extract substrate atoms from large file
	echo "Extracting substrate atoms"
	$SCRATCH/droplet/src/quartzIsolateSubstrate.awk $source_file $out_folder/substrate.txt

	#Substrate density
	echo "Calculating substrate density"
	$SCRATCH/droplet/exec/quartzDensity.out $out_folder/substrate.txt $out_folder/substrate_density_halfA.txt $out_folder/center_of_mass.txt
	rm $out_folder/substrate.txt

	#Calculate dipole angles and velocities
	echo "Calculating dipole angles and velocities"
	$SCRATCH/droplet/exec/calculations_centered.out $out_folder/waters_sorted.txt $out_folder/calculated.txt
	rm $out_folder/waters_sorted.txt

	#:)
	echo "Done!"

else
	echo "2 arguments required! - Input file & output folder name in sapphire/analysis"
	echo "$# arguments given"

fi
