#!/bin/bash
#run_parseAll_cyl.sh

node1=n014
node2=n017
node3=n018

# Spherical
#./parseAll.sh 20A 1 3 $node
#./parseAll.sh 30A 5 6 $node
#./parseAll.sh 40A 11 12 $node
#./parseAll.sh 50A 1 12 $node
./parseAll.sh 60A 1 8 $node1
./parseAll.sh 60A 9 16 $node2
./parseAll.sh 60A 17 24 $node3
#
# Hemispherical
#./parseAll.sh Hemi50A 6 9 $node
