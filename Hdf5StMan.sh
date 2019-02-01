#!/bin/bash
cd $PBS_O_WORKDIR
while true
do
    filename=/lustre/atlas/scratch/wangj/csc303/data/$RANDOM
    aprun -B ./write Hdf5StMan $filename
    rm -rf $filename
done
