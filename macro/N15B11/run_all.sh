#!/bin/bash

# Directories
COMPILATIONDIR=../../../build
SIMOUTDIR=output_parallel
RESULTSDIR=result
CALCOUTDIR=calc_output
INDIR=../output_parallel
OUTDIR=output_digi_parallel
GRAPHSOUTDIR=digi_graphs_parallel

# Variables
NEVENTS=1000
MINANGLE=35
MAXANGLE=35
NTHREADS=3

# Digitization add or don't add
TOADDDIGI='yes'
echo -n "Enter 'yes' to add digitization: "
read STRING
if [[ $TOADDDIGI = $STRING ]]
then
    cd digi
    if [ -d ${RESULTSDIR} ]; then
        cd ${RESULTSDIR}
        rm -fv *
        cd -
    else
        mkdir ${RESULTSDIR}
    fi
    cd ../
fi

#Calculate step and Interaction numbers
a=1
b=1
STEP=$(echo "$a/$b" | bc -l) #STEP=a/b
ITNUMBER=$(echo "1+($MAXANGLE-$MINANGLE)/$STEP" | bc -l)

# Compilation
echo ${COMPILATIONDIR:?You must set directory to compilation}
if [ -d ${COMPILATIONDIR} ]; then
	cd ${COMPILATIONDIR}
	make -j${NTHREADS}
	cd -
else
	echo -e "Compilation directory: \e[1m\e[34m${COMPILATIONDIR}\e[0m was not found"
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

#ITNUMBER=3
for IT in $(seq 1 ${ITNUMBER}); do
    ####################################### Simulation #######################################
	ANG=$(echo "$MINANGLE+($IT-1)*$STEP" | bc -l) #curent angle calculate

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
    ####################################### ThetaCM write ######################################
    cd ${SIMOUTDIR}
    grep "ThetaCM Mean for N15:" out_1.txt | cut -f5 -d " " >> ../${RESULTSDIR}/thetaCMN15.txt
    grep "ThetaCM Mean for B11:" out_1.txt | cut -f5 -d " " >> ../${RESULTSDIR}/thetaCMB11.txt
    cd -
    ####################################### Digitization #######################################
    if [[ $TOADDDIGI = $STRING ]]
    then
        cd digi
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
        # Digi start
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
        root -l -b -q "merge_graphs.C(\"${GRAPHSOUTDIR}\")"
        cp ${GRAPHSOUTDIR}/eloss.pdf ${RESULTSDIR}/eloss_${ANG}.pdf
        cd ../
    fi
done

wait
cat ${RESULTSDIR}/out.txt
echo -e "\e[1m\e[32m========== FINISHED ========= \e[0m"

####################################### Cross-section calculate #######################################
wait

if [ -d cross_section/ ]; then
	cd cross_section
	./remove.sh
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
cp ${RESULTSDIR}/thetaCM* cross_section/input/
cd cross_section
#cd input/
#cp out.txt ../arhive
#cd -
wait
root -l  "cross_section.C(${NEVENTS}, ${MINANGLE}, ${NTHREADS}, ${ITNUMBER}, ${STEP})"
wait
exit
