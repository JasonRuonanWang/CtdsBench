import os
import sys

nodes = [1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048]

stman = sys.argv[1]

cmd = "echo no stman specified"

for n in nodes:
    if stman == "Adios2StMan":
        cmd = "qsub -A csc303 -l walltime=02:00:00 -l nodes={0} /lustre/atlas/scratch/wangj/csc303/h5tests/Adios2StMan.sh".format(n)
        os.system(cmd)
        print cmd
    elif stman == "AdiosStMan":
        cmd = "qsub -A csc303 -l walltime=02:00:00 -l nodes={0} /lustre/atlas/scratch/wangj/csc303/h5tests/AdiosStMan.sh".format(n)
        os.system(cmd)
        print cmd
    elif stman == "Hdf5StMan":
        cmd = "qsub -A csc303 -l walltime=02:00:00 -l nodes={0} /lustre/atlas/scratch/wangj/csc303/h5tests/Hdf5StMan.sh".format(n)
        os.system(cmd)
        print cmd

