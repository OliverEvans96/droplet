#!/bin/bash
d="$SCRATCH/droplet/root"
g++ `root-config --glibs --cflags` -lMathMore -lMinuit $d/cyl_analysis.cpp $d/CircleFitClass.cpp $d/Quiver/Quiver.cpp -o $d/cyl_analysis.out
