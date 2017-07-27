#!/bin/bash
#kill_my_jobs.sh

#Kill all jobs

my_jobs=($(qstat | grep oge1 | awk -F'.' '{print$1}'))
qdel "${my_jobs[@]}"

