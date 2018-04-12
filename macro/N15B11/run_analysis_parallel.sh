#!/bin/bash

OUTDIR=output_parallel
SIMPREFIX=sim_

for FILE in ${OUTDIR}/${SIMPREFIX}*.root; do

	BASENAME=$(basename ${FILE})
	SUFFIX=${BASENAME##*_}
	INDEX=${SUFFIX%.*}
	
#	echo ${FILE} ${BASENAME} ${SUFFIX} ${INDEX}

	root -l -b -q "analyse.C(${INDEX}, \"${OUTDIR}\", \"${SIMPREFIX}\")"

done

#root -l "analyse.C(0, \"${OUTDIR}\", \"sim_\")"
