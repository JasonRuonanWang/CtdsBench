import os

nodes = [1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048]
nodes = [1]

for n in nodes:
    cmd = "qsub -A csc303 -l walltime=02:00:00 -l nodes={0} /lustre/atlas/scratch/wangj/csc303/CtdsBench/Adios2StMan.sh".format(n)
    print cmd
    os.system(cmd)

    cmd = "qsub -A csc303 -l walltime=02:00:00 -l nodes={0} /lustre/atlas/scratch/wangj/csc303/CtdsBench/AdiosStMan.sh".format(n)
    print cmd
    os.system(cmd)

    cmd = "qsub -A csc303 -l walltime=02:00:00 -l nodes={0} /lustre/atlas/scratch/wangj/csc303/CtdsBench/Hdf5StMan.sh".format(n)
    print cmd
    os.system(cmd)
