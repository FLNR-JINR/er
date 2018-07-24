#!/bin/bash

# Parameters
NEVENTS=1000
INDIR=../output_parallel
OUTDIR=output_digi_parallel
GRAPHSOUTDIR=digi_graphs_parallel
BUILDDIR=../../../../fork_expertroot_build
NTHREADS=3

# Compilation
if [ -d ${BUILDDIR} ]; then
	cd ${BUILDDIR}
	make -j3
	cd -
else
	echo -e "\e[1m\e[32m========== You must set directory to compilation =========== \e[0m"
	exit
fi

# Search directories
if [ -d ${INDIR} ]; then
	echo -e "\e[1m\e[32m========== ${INDIR} was found =========== \e[0m"
else
	echo -e "\e[1m\e[32m========== ${INDIR} was not found =========== \e[0m"
	exit
fi

# Cleanup
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

for TRH in $(seq 1 ${NTHREADS}); do
	echo "digitization start in thread ${TRH}"
	root -l -q -b "digi.C(${NEVENTS}, ${TRH}, \"${INDIR}\", \"${OUTDIR}\")" 2> ${OUTDIR}/err_${TRH}.txt > ${OUTDIR}/out_${TRH}.txt &
done
wait
echo -e "\e[1m\e[32m========== Digitization finished =========== \e[0m"

for TRH in $(seq 1 ${NTHREADS}); do
	echo "graph form start in thread ${TRH}"
	root -l -q -b "draw_graph.C(${TRH}, \"${OUTDIR}\", \"${GRAPHSOUTDIR}\")" &
done
wait

root -l "merge_graphs.C(\"${GRAPHSOUTDIR}\")"
