#!/bin/bash
cd $PBS_O_WORKDIR
while true
do
    filename=/lustre/atlas/scratch/wangj/csc303/data/$RANDOM
    aprun -B ./write_concat $filename
    rm -rf $filename.*
done
