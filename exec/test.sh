#!/bin/bash
#test.sh

#Parallel script
parallel='$SCRATCH/droplet/exec/parallel.sh'

#Argument
script='"echo"'
name='test_{}'
node='bethel'
arg1='20A/20AonMonolayer test 123 456'
arg2='atom{}'
arg3=''
low='1'
high='6'

#Run script
$parallel $script $name $node $arg1 $arg2 $arg3 $low $high

