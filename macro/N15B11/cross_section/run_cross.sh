#!/bin/bash
NEVENTS=100
STARTANG=5
NTHREADS=16
ANGNUM=31
if [ -d result ]; then
	cd result
        rm -fv resultGr.pdf 
	cd -
else
	mkdir result
fi

root -l "cross_section.C(${NEVENTS}, ${STARTANG}, ${NTHREADS}, ${ANGNUM})"
