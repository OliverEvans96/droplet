#!/bin/bash
#ps_status.sh
#Watch ps logs, return most recently printed frame number for each

cd $SCRATCH/droplet/logs

for sim in `seq 20 10 60`
do
	for part in `ls -v ps_${sim}A_atom*`
	do
		frame=$(cat $part | grep FRAME | tail -n1)
		tmp=${part##*_} #isolate part number
		echo "${sim}/${tmp%.txt}: ${frame}"
	done
	echo
done
