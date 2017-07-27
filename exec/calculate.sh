#!/bin/bash

for folder in */
do
	echo "Performing calculations for $folder"
	../exec/calculations.out "${folder}waters_sorted.txt" "${folder}calculated.txt"
	echo
done

echo "Done!"
