#!/bin/bash
#cyl_sub_when_done.sh

#submit cyl_analysis.sh once I have no jobs left on bethel
#n_jobs=$(qnode | grep -Ec 'oge1.*bethel')
n_jobs=$(qnode | grep -Ec 'oge1')

while [ $n_jobs -gt 0 ]
do
	n_jobs=$(qnode | grep -Ec 'oge1')
	echo "n_jobs = $n_jobs - sleeping"
	sleep 30
done

echo "n_jobs = 0 - submitting!"
./cyl_subAll.sh
echo "Done!"
