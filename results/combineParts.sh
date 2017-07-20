#!/bin/bash

#combineParts.sh

#Combine output data from parts of a simulation
#I chose this method of iterating from first to last and ignoring missing parts to ensure that parts were processed in sequential order.
#Using something like "for file in atom*", the parts were processed in the order: atom1,atom10,atom11,atom2,.. because of the lack of preceeding zeros in part names.

#This script is to be excuted by extract.sh

#Remove files if they exist
rm -f avgStepData.txt instStepData.txt

#Make a list of all part numbers
i=0
for num in atom*
do
	partNums[$((i++))]=${num#atom}
done

#Find the first and last part numbers
echo "partNums=${partNums[@]}"
first=`printf "%d\n" "${partNums[@]}" | sort -rn | tail -1`
echo "First: $first"
last=`printf "%d\n" "${partNums[@]}" | sort -rn | head -1`
echo "Last: $last"

#Count number of fields
nAvgFields=$(head -n1 atom$first/avgStepData.txt | awk '{print NF}') 
nInstFields=$(head -n1 atom$first/instStepData.txt | awk '{print NF}') 

#Part number counter
pn=1

#Iterate from last to first
for part in `seq $first $last`
do
	#Move on if part doesn't exist
	if [ -d atom$part ]
	then
		#Only include first line comment for first part
		if [[ $pn == 1 ]]
		then
			cat atom$part/avgStepData.txt >> avgStepData.txt
			cat atom$part/instStepData.txt >> instStepData.txt
		else	
			tail -n +2 atom$part/avgStepData.txt >> avgStepData.txt
			tail -n +2 atom$part/instStepData.txt >> instStepData.txt
		fi

		#Increment pn
		((pn++))

		echo "Added atom$part"
	else
		#Fill in file with array of dashes if file is missing
		#Hope that none are missing.
		#Missing pieces might not necessarily be 100 & 500 lines long respectively.
		../writeDashes.sh 100 $nAvgFields >> avgStepData.txt
		../writeDashes.sh 500 $nInstFields  >> instStepData.txt
		echo "Missing atom$part - Inserted dashes"
	fi
done
