#!/bin/bash

OUTDIR=output_parallel

# Number of events per thread
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

for THR in {1..8}
do
	echo "Starting simulation in thread ${THR}"
	root -l -b -q "sim.C(${NEVENTS}, ${THR}, \"${OUTDIR}\")" > ${OUTDIR}/out_${THR}.txt 2> ${OUTDIR}/err_${THR}.txt &
done

# Just for reference
###### root -l -b -q "sim.C(10000, 0)" > ${OUTDIR}/out.txt 2> ${OUTDIR}/err.txt

wait

echo -e "\e[1m\e[32m========== Simulation finished ============ \e[0m"
