#!/bin/bash
#decompress_prewet.sh

#Copy and unzip all prewet files to ~/lammps/sapphire/analysis/data/Bob/Prewet/30AonMonolayer/

#Parallel script
parallel='$SCRATCH/droplet/exec/parallel.sh'

#Argument
script='$SCRATCH/droplet/exec/decompress.sh'
name='Decompress_Prewet_20A_20onMono_atom{}'
node='bethel'
arg1='20A/20AonMonolayer'
arg2='atom{}'
arg3=''
low='1'
high='6'

#Run script
$parallel $script $name $node $arg1 $arg2 $arg3 $low $high

