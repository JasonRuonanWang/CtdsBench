#!/bin/bash
cd $PBS_O_WORKDIR
while true
do
    aprun -B ./write AdiosStMan
done
