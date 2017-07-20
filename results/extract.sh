#!/bin/bash

#Combine parts for all sims, then all sims

cd /home/oge1/lammps/sapphire/analysis/results

echo "Combining Parts..."

#Run combineParts.sh for each sim
for sim in *0A
do
	cd $sim
	echo
	echo "Simulation: $sim"
	echo

	#Adjust flux at the beginning of each part
	echo "Adjusting Flux"

	#Reset adjustment counters
	nMonoTotal=0
	monoRTotal=0

	#Count number of fields in this sim file
	nAvgFields=$(head -n1 avgStepData.txt | awk '{print NF}') 
	nInstFields=$(head -n1 instStepData.txt | awk '{print NF}') 

	#Figure out which field is flux
	#Read 1st line | separate to multiple lines | add line numbers | ignore case, match 'flux', return line number (word number)
	avgFluxField=$(head -n1 avgStepData.txt | xargs printf "%s\n" | cat -n | awk 'tolower($2) ~ /flux/ {print $1}')
	instFluxField=$(head -n1 instStepData.txt | xargs printf "%s\n" | cat -n | awk 'tolower($2) ~ /flux/ {print $1}')
	avgMonoRField=$(head -n1 avgStepData.txt | xargs printf "%s\n" | cat -n | awk 'tolower($2) ~ /dme/ {print $1}')

	#Iterate through all but last part
	partNames=(atom*)
	echo "PWD: `pwd`"
	echo "Loop through ${partNames[@]}."
	echo $(seq $((${#partNames[@]}-1)))

	#Only adjust once
	for i in $(seq 2 $((${#partNames[@]})))
	do
		if [[ ! -e "atom$i/adjusted.bool" ]]
		then
			echo
			echo "Adjusting $sim/atom$i"
			#Count # of atoms added to monolayer for this file (sum of flux)
			#This should be the same for avg and inst, so only count once
			nMonoThisPart=$(tail -n +2 atom$((i-1))/avgStepData.txt | awk '{n+=$'"$avgFluxField"'}END{print n}')

			#Count change in monolayer this part
			monoRThisPart=$(tail -n +2 atom$((i-1))/avgStepData.txt | awk '{n+=$'"$avgMonoRField"'}END{print n}')

			#Count total # of atoms in monolayer & monolayer change
			((nMonoTotal+=nMonoThisPart))
			#Bash can't do floating point arithmetic
			monoRTotal=$(python -c "print $monoRTotal + $monoRThisPart")

			echo
			echo "nMonoThisPart = $nMonoThisPart"
			echo "nMonoTotal = $nMonoTotal"
			echo "avgFluxField = $avgFluxField"

			echo
			echo "monoRThisPart = $monoRThisPart"
			echo "monoRTotal = $monoRTotal"
			echo "avgMonoRField = $avgMonoRField"

			#Subtract total number of monolayer this time from first flux value in next part
			echo "Writing to atom$i/avgStepData.txt"
			#cat "atom$i/avgStepData.txt" | awk 'BEGIN{OFS="\t"}NR==2{print "avg: "$'$avgFluxField'"-'$nMonoTotal', and "$'"$avgDMeField"'"-'"$monoR"'"; $'"$avgFluxField"'-='"$nMonoTotal"';$'"$avgDMeField"'-='"$monoR"'}{print > "atom'$i'/avgStepData.txt"}' 
			cat "atom$i/avgStepData.txt" | awk '
			BEGIN{OFS="\t"}
			NR==2{
				print "avgFlux: "$'"$avgFluxField"'"-'"$nMonoTotal"'"
				print "avgMono: "$'"$avgMonoRField"'"-'"$monoRTotal"'";
			   	$'"$avgFluxField"'-='"$nMonoTotal"';
			   	$'"$avgMonoRField"'-='"$monoRTotal"'}
			{print > "atom'$i'/avgStepData.txt"}' 

			cat "atom$i/instStepData.txt" | awk 'BEGIN{OFS="\t"}NR==2{print "inst: "$'$instFluxField'"-'$nMonoTotal'"; $'"$instFluxField"'-='"$nMonoTotal"'}{print > "atom'$i'/instStepData.txt"}' 
			#part has now been adjusted.
			touch atom$i/adjusted.bool
		else
			echo "Not adjusting $sim/atom$i"
		fi
	done

	echo "combineParts:"
	#Combine data from each part
	../combineParts.sh
	cd ..

	echo
	echo "---------------"
	echo
done

#Count # of lines in longest inst and avg files & save file name
longestAvg=($(wc -l *0A/avgStepData.txt | head -n -1 | awk '$1>n{n=$1;f=$2}END{print n,f}'))
longestInst=($(wc -l *0A/instStepData.txt | head -n -1 | awk '$1>n{n=$1;f=$2}END{print n,f}'))

#Separate number of lines & file name
longestAvgN=${longestAvg[0]}
longestAvgF=${longestAvg[1]}
longestInstN=${longestInst[0]}
longestInstF=${longestInst[1]}

echo "Longest avg file is $longestAvgF with $longestAvgN lines"
echo "Longest inst file is $longestInstF with $longestInstN lines"

#Create time templates
echo "Creating time templates..."
tail -n +2 $longestAvgF | awk '{print $1}' > templates/avgTime.txt
tail -n +2 $longestInstF | awk '{print $1}' > templates/instTime.txt


for sim in *0A
do
	cd $sim

	echo
	echo "Searching for outliers"
	cat "avgStepData.txt" | awk '
	BEGIN{OFS="\t"}
	NR>1{
		for(i=2;i<=NF;i++) {
			if(sqrt($i^2)>1e5) {
				if($i!="nan")
				{
					print "Found avg outlier!",$0;
					print $i" = -";
				}
				$i="-";
			}
		};
	}
	{print > "avgNoOutliers.txt"}'
	cat avgNoOutliers.txt > avgStepData.txt

	cat "instStepData.txt" | awk '
	BEGIN{OFS="\t"}
	NR>1{
		for(i=2;i<=NF;i++) {
			if(sqrt($i^2)>1e5) {
				if($i!="nan")
				{
					print "Found inst outlier!",$0;
					print $i" = -";
				}
				$i="-";
			}
		};
	}
	{print > "instNoOutliers.txt"}'
	cat instNoOutliers.txt > instStepData.txt


	#Add dashes to ends of files before combining

	#Count number of fields in this sim file
	nAvgFields=$(head -n1 avgStepData.txt | awk '{print NF}') 
	nInstFields=$(head -n1 instStepData.txt | awk '{print NF}') 

	#Count number of lines in this sim file
	nAvg=`wc -l avgStepData.txt | awk '{print $1}'`
	nInst=`wc -l instStepData.txt | awk '{print $1}'`
	echo "$sim: nAvg=$nAvg nInst=$nInst"

	#Number of dash rows to add = number required - current number
	nDashesAvg=$(($longestAvgN - $nAvg))
	nDashesInst=$(($longestInstN - $nInst))

	#Add dashes
	../writeDashes.sh $nDashesAvg $nAvgFields >> avgStepData.txt
	echo "Added $nDashesAvg lines to avgStepData.txt"

	../writeDashes.sh $nDashesInst $nInstFields >> instStepData.txt
	echo "Added $nDashesInst lines to instStepData.txt"

	#Separate sim files into column files
	echo "Separating sim files into columns"
	for group in avg inst
	do
		nCols=`head -n1 ${group}StepData.txt | awk '{print NF}'` 
		for i in `seq -f "%02.0f" 1 $nCols`
		do
			awk 'NR>1{print $'"$i"'>"results/'"${group}$i"'.txt"}' ${group}StepData.txt
		done
	done

	cd ..
done

#Now, combine sim column files into overall files for each field
echo
echo "Combining Sims..."
./combineSims.sh

echo
echo "Plotting..."
cd finalResults
gnuplot plotAll.gpi
cd ..

echo
echo "Done! Woohoo!"
