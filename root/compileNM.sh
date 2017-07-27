#!/bin/bash
d="$SCRATCH/droplet/root"
g++ `root-config --glibs --cflags` -lMathMore $d/NumericalMinimization.cpp -o $d/NumericalMinimization.out
