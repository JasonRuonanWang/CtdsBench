import sys
import subprocess

nodes = [1, 2, 4, 8]

def sub(stman, node):
    cmd = "qsub -A csc303 -l walltime=02:00:00 -l nodes={0} {1}.sh".format(node, stman)
    subprocess.Popen(cmd, stdout = subprocess.PIPE, stderr=subprocess.STDOUT, shell=True)
    print cmd

for n in nodes:
    sub("AdiosStMan", n)
    sub("Adios2StMan", n)
    sub("Hdf5StMan", n)

