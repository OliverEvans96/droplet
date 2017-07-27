#!/bin/bash
d="$SCRATCH/droplet/root"
g++ `root-config --glibs --cflags` -lMathMore -lMinuit $d/polarScatter.cpp $d/CircleFitClass.cpp $d/Quiver/Quiver.cpp $d/../src/FieldViz/FieldViz.cpp -o $d/polarScatter.out
