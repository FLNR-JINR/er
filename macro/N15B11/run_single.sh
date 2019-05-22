#!/bin/bash

OUTDIR=output
NEVENTS=100
ANGEL=35

cd ../../../build
make -j3
cd -

echo -e "\e[1m\e[32m========== Compilation finished =========== \e[0m"

if [ -d ${OUTDIR} ]; then
	cd ${OUTDIR}
	rm -fv *
	cd -
else
	mkdir ${OUTDIR}
fi

cd ../../geometry/
rm -fv N15B11_detector.geo.root
cd -
echo -e "\e[1m\e[32m========== Cleanup finished  === Angle( ${ANGEL} ) ============ \e[0m"

if [ -d geo/ ]; then
    cd geo/
    root -l -b -q "create_N15B11_detector_geo.C(${ANGEL})"
    wait
    cd -
else
    echo -e "\e[1m\e[32m========== Directory to build geometry was not found =========== \e[0m"
    exit
fi
echo -e "\e[1m\e[32m========== Geometry created  === Angle( ${ANGEL} ) ============ \e[0m"

root -l -b -q "sim.C(${NEVENTS}, 0, \"${OUTDIR}\", ${ANGEL})" > ${OUTDIR}/out.txt 2> ${OUTDIR}/err.txt
wait
echo -e "\e[1m\e[32m========== Simulation finished ============ \e[0m"

root -l -b -q "calc_parallel.C(0, \"${OUTDIR}\")" 2>> ${OUTDIR}/calc.err.txt
wait

echo "N15 numbers in Gas and in Semi:"
tail -1 ${OUTDIR}/numbers.txt
echo "B11 numbers in Gas and in Semi:"
head -1 ${OUTDIR}/numbers.txt

echo -e "\e[1m\e[32m========== Calculations finished ============ \e[0m"
