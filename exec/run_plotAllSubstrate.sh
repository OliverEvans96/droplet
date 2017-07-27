#!/bin/bash
#run_plotAllSubstrate.sh

node=n023

#Sub951By50
./plotAllSubstrate.sh Bob/Sub951By50/Cyl20A 1 1 $node
./plotAllSubstrate.sh Bob/Sub951By50/Cyl30A 1 5 $node
./plotAllSubstrate.sh Bob/Sub951By50/Cyl40A 1 5 $node
./plotAllSubstrate.sh Bob/Sub951By50/Cyl50A 1 5 $node
./plotAllSubstrate.sh Bob/Sub951By50/Cyl60A 2 5 $node
./plotAllSubstrate.sh Bob/Sub951By50/Cyl100A 1 2 $node

./plotAllSubstrate.sh Bob/Sub951By50/FullCyl20A 1 3 $node
./plotAllSubstrate.sh Bob/Sub951By50/FullCyl30A 1 3 $node
./plotAllSubstrate.sh Bob/Sub951By50/FullCyl100A 1 1 $node

#Sub951By100
./plotAllSubstrate.sh Bob/Sub951By100/Cyl20A 1 2 $node
./plotAllSubstrate.sh Bob/Sub951By100/Cyl50A 1 3 $node
./plotAllSubstrate.sh Bob/Sub951By100/Cyl100A 1 1 $node
