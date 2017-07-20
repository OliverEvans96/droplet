#!/bin/bash

for i in `ls`
do
	for j in `ls $i | grep atom`
	do
		rm $i/$j/img -rf
		rm $i/img -rf
	done
done
