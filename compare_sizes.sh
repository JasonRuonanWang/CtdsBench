#!/bin/bash

rm -rf output
mkdir output

if [ $# -lt 1 ]; then
	echo "Usage: $0 <nrows>" 1>&2
	exit 1
fi
rows=$1

echo "      stman    dtype dims cellsize expect [MB] real [MB] rel. diff [%]"
echo "=========== ======== ==== ======== =========== ========= ============="
for stman in none Adios2StMan Adios2StMan-HDF5; do
	for dtype in float double complex dcomplex; do
		for dims in 1 2; do
			for cellsize in 4 8 16 32 64 128 256 512 1024; do

				output=output/${stman}_${dtype}_${dims}d_${cellsize}
				msg=`./write_fixed $stman ${output} $cellsize $dims $dtype $rows`

				if [ $? -ne 0 ]; then
					echo "Failure during execution of write_example: $msg" 1>&2
					exit 1
				fi

				expected=`echo $msg | sed 's/.*total bytes = \([0-9]\+\).*/\1/'`
				real=`du -bs $output | sed 's/\([0-9]\+\).*/\1/'`
				diff=`echo "scale=2; ($real - $expected) * 100. /$expected" | bc`
				expected=`echo "scale=2; $expected / 1024. / 1024" | bc`
				real=`echo "scale=2; $real / 1024. / 1024" | bc`
				printf "%11s %8s %4d %8d %11s %9s %13s\n" $stman $dtype $dims $cellsize $expected $real $diff
			done
		done
	done
done
