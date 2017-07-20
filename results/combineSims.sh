#!/bin/bash

#combineSims.sh
#Combine extracted data

#This script is to be excuted by extract.sh

#Create directory if it doesn't exist
mkdir -p finalResults/inst
mkdir -p finalResults/avg

#Combine 

echo "AVG:"
i=2;

#Read fields from file
avgFields=$(head -n1 20A/atom1/avgStepData.txt)
#Remove any initial spaces before '#'
avgFields=$(echo "$avgFields" | sed 's/.*#/#/')
#Remove '#time' (first word) from fields
avgFields=${avgFields#* }

for field in $avgFields
do
	echo $field
	#Format i
	printf -v i "%02.0f" $i
	paste -d"\t\t" templates/avgTime.txt *0A/results/avg$i.txt > finalResults/avg/$field.txt
	#Unformat i
	printf -v i "%.0f" $i
	((i++))
done

echo
echo "INST:"

#Read fields from file
instFields=$(head -n1 20A/atom1/instStepData.txt)
#Remove any initial spaces before '#'
instFields=$(echo "$instFields" | sed 's/.*#/#/')
#Remove '#time' (first word) from fields
instFields=${instFields#* }

i=2;
for field in $instFields
do
	echo $field
	#Format i
	printf -v i "%02.0f" $i
	paste -d"\t\t" templates/instTime.txt *0A/results/inst$i.txt > finalResults/inst/$field.txt
	#Unformat i
	printf -v i "%.0f" $i
	((i++))
done

echo
echo "Done combining sims!"

