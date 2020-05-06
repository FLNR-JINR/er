#!/bin/bash
NEVENTS=10000
STARTANG=11
NTHREADS=16
ANGNUM=$(grep -c -v "ncncw" input/thetaCMN15.txt)
echo "ANGNUM: ${ANGNUM}"
if [ -d result ]; then
	cd result
        rm -fv resultGr.pdf
	cd -
else
	mkdir result
fi

root -l "cross_section.C(${NEVENTS}, ${STARTANG}, ${NTHREADS}, ${ANGNUM})"
