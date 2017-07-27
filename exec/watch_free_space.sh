#!/bin/bash
#watch_free_space.sh

#Make sure that bethel doesn't run out of HD space
#Kill all of my jobs if it gets too low

free=$(df -k /home | tail -n1 | awk '{print $3}')
#Limit = 0.5TB
limit=500000000

#Run as long as I have some jobs running
n_jobs=$(qstat | grep oge1 -c)
while [ "$n_jobs" -gt "0" ]
do
	date +"%M-%d-%Y %H:%m"
	echo "$n_jobs jobs"
	echo "$free free"
	if [ "$free" -lt "$limit" ]
	then
		echo "Too low! killing!"
		#Identify my jobs
		my_jobs=($(qstat | grep oge1 | awk -F'.' '{print$1}'))
		
		#Kill jobs
		qdel "${my_jobs[@]}"
	fi
	echo "OK"
	echo

	#Check every 10 minutes
	sleep 600
done
