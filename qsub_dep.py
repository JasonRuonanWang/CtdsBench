import sys
import subprocess

nodes = [4096, 2048, 1024, 512, 256, 128, 64, 32, 16]

dep = None

def sub(stman, node):
    global dep
    cmd = "qsub -A csc303 -l walltime=02:00:00 -l nodes={0} ".format(node)
    if dep != None:
        cmd = cmd + "-W depend=afterany:{0} ".format(dep.rstrip("\n\r"))
    cmd = cmd + stman + ".sh"
    out = subprocess.Popen(cmd, stdout = subprocess.PIPE, stderr=subprocess.STDOUT, shell=True)
    dep, stderr = out.communicate()
    print cmd, dep

for n in nodes:
    sub("AdiosStMan", n)
    sub("Adios2StMan", n)
    sub("Hdf5StMan", n)
    sub("Concat", n)

