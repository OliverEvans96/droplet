#!/bin/bash
#zipfile.sh

#Zip one file

#Arg 1: file

bzip2 -vz $1 $1.bz2
