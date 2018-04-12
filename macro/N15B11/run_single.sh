#!/bin/bash

OUTDIR=output
NEVENTS=1000

cd ../../../build/
make -j4
cd -

echo -e "\e[1m\e[32m========== Compilation finished =========== \e[0m"

if [ -d ${OUTDIR} ]; then
	cd ${OUTDIR}
	rm -fv *
	cd -
else
	mkdir ${OUTDIR}
fi

echo -e "\e[1m\e[32m========== Cleanup finished =============== \e[0m"

root -l -b -q "sim.C(${NEVENTS}, 0, ${OUTDIR})" > ${OUTDIR}/out.txt 2> ${OUTDIR}/err.txt

echo -e "\e[1m\e[32m========== Simulation finished ============ \e[0m"
