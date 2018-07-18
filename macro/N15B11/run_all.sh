#!/bin/bash

# Directories
SIMOUTDIR=output_parallel
RESULTSDIR=result
COMPILATIONDIR=../../../fork_expertroot_build
CALCOUTDIR=calc_output

# Variables
NEVENTS=1000
MINANGEL=35
MAXANGEL=35
NTHREADS=3
DIGIPREFIX=digi_

#Calculate step and Iteratins nubers
a=1
b=10
STEP=$(echo "$a/$b" | bc -l) #STEP=a/b
ITNUMBER=$(echo "($MAXANGEL-$MINANGEL+1)/$STEP" | bc -l)

# Compilation
if [ -d ${COMPILATIONDIR} ]; then
	cd ${COMPILATIONDIR}
	make -j3
	cd -
else
	echo -e "\e[1m\e[32m========== You must set directory to build =========== \e[0m"
	exit
fi

echo -e "\e[1m\e[32m========== Compilation finished =========== \e[0m"

if [ -d ${RESULTSDIR} ]; then
	cd ${RESULTSDIR}
	rm -fv *
	cd -
else
	mkdir ${RESULTSDIR}
fi
date > ${RESULTSDIR}/out.txt

ITNUMBER=2
for IT in $(seq 1 ${ITNUMBER}); do

	ANG=$(echo "$MINANGEL+($IT-1)*$STEP" | bc -l) #curent angle calculate

	if [ -d ${SIMOUTDIR} ]; then
		cd ${SIMOUTDIR}
		rm -fv *
		cd -
	else
		mkdir ${SIMOUTDIR}
	fi

	if [ -d ${CALCOUTDIR} ]; then
		cd ${CALCOUTDIR}
		rm -fv *
		cd -
	else
		mkdir ${CALCOUTDIR}
	fi

	cd ../../geometry/
	rm -fv N15B11_detector.geo.root
	cd -
	echo -e "\e[1m\e[32m========== Cleanup finished  === Angle( ${ANG} ) ============ \e[0m"

	if [ -d geo/ ]; then
		cd geo/
		root -l -b -q "create_N15B11_detector_geo.C(${ANG})"
		wait
		cd -
	else
		echo -e "\e[1m\e[32m========== Directory to build geometry was not found =========== \e[0m"
                exit
	fi

	echo -e "\e[1m\e[32m========== Geometry created  === Angle( ${ANG} ) ============ \e[0m"

	for THR in $(seq 1 ${NTHREADS}); do
		echo "Starting simulation in thread ${THR}"
		root -l -b -q "sim.C(${NEVENTS}, ${THR}, \"${SIMOUTDIR}\", ${ANG})" > ${SIMOUTDIR}/out_${THR}.txt 2> ${SIMOUTDIR}/err_${THR}.txt &
	done
	wait
 	echo -e "\e[1m\e[32m========== Simulations finished  === Angle( ${ANG} ) ============ \e[0m"

	for THR in $(seq 1 ${NTHREADS}); do
		echo "Starting calculation in thread ${THR}"
		root -l -b -q "calc_parallel.C(${THR})" 2>> ${CALCOUTDIR}/calc_parr.err.txt
		wait
	done
	wait

	root -l -b -q calculate_all.C >> ${CALCOUTDIR}/all_calculted.txt 2>> ${CALCOUTDIR}/all_calculated.err.txt
	wait
	echo "Angle: " ${ANG} >> ${RESULTSDIR}/out.txt
	wait
	grep -m 1 "summ" ${SIMOUTDIR}/out_1.txt >> ${RESULTSDIR}/out.txt
	wait
	cat ${CALCOUTDIR}/all_calculted.txt >> ${RESULTSDIR}/out.txt
	wait
	echo "======================================" >> ${RESULTSDIR}/out.txt
	wait

	echo Angle: ${ANG} >> ${RESULTSDIR}/target_int_num.txt
	for THR in $(seq 1 ${NTHREADS}); do
		echo THR:${THR} >> ${RESULTSDIR}/target_int_num.txt
		grep "Interactions number in target: " ${SIMOUTDIR}/out_${THR}.txt >> ${RESULTSDIR}/target_int_num.txt
	done
	wait
	echo "======================================" >> ${RESULTSDIR}/target_int_num.txt

	echo Angle: ${ANG} >> ${RESULTSDIR}/dPhi_info.txt
	grep "dPhi range: " ${SIMOUTDIR}/out_1.txt >> ${RESULTSDIR}/dPhi_info.txt

	echo "======================================" >> ${RESULTSDIR}/dPhi_info.txt
	echo -e "\e[1m\e[32m========== All calculation finished === Angle( ${ANG} ) ========= \e[0m"
wait
done

wait

echo -e "\e[1m\e[32m========== FINISHED ========= \e[0m"

cat ${RESULTSDIR}/out.txt
wait

if [ -d cross_section ]; then
	cd cross_section
	./remove
	cd -
else
	echo -e "\e[1m\e[32m========== cross_section directory was not found ========= \e[0m"
	exit
fi

grep "summ:" ${RESULTSDIR}/out.txt >> cross_section/input/out.txt
grep "nB11Gas:" ${RESULTSDIR}/out.txt >> cross_section/input/out.txt
grep "nN15Gas:" ${RESULTSDIR}/out.txt >> cross_section/input/out.txt
cp ${RESULTSDIR}/target_* cross_section/input/
cp ${RESULTSDIR}/dPhi_info.txt cross_section/input/
cd cross_section
#cd input/
#cp out.txt ../arhive
#cd -
wait
root -l  "cross_section.C(${NEVENTS}, ${MINANGEL}, ${NTHREADS}, ${ITNUMBER}, ${STEP})"
wait
exit
