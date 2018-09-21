#!/bin/bash

NEVENTS=1000
INDIR=../output
OUTDIR=output_digi
GRAPHSOUTDIR=digi_graphs
BUILDDIR=../../../../fork_expertroot_build

if [ -d ${BUILDDIR} ]; then
	cd ${BUILDDIR}
	make -j3
	cd -
else
	echo -e "\e[1m\e[32m========== You must set directory to compilation =========== \e[0m"
	exit
fi

if [ -d ${OUTDIR} ]; then
	cd ${OUTDIR}
	rm -fv *
	cd -
else
	mkdir ${OUTDIR}
fi

if [ -d ${GRAPHSOUTDIR} ]; then
	cd ${GRAPHSOUTDIR}
	rm -fv *
	cd -
else
	mkdir ${GRAPHSOUTDIR}
fi
echo -e "\e[1m\e[32m========== Cleanup finished =========== \e[0m"

root -l -q -b "digi.C(${NEVENTS}, 0, \"${INDIR}\", \"${OUTDIR}\")" 2> ${OUTDIR}/err.txt > ${OUTDIR}/out.txt

root -l -q -b "draw_graph.C(0, \"${OUTDIR}\", \"${GRAPHSOUTDIR}\")"

#cat ${OUTDIR}/err.txt
#cat ${OUTDIR}/out.txt
