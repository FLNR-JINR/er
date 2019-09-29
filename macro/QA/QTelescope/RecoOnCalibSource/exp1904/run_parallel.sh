#!/bin/bash

if [ -z ${GO4SYS+x} ]; then
	echo "GO4SYS is unset. Aborting.";
	exit;
else
	echo "GO4SYS is set to '$GO4SYS'";
fi

# =============================================================================

# Please specify the path to a directory which contains _ONLY_ the files you want to analyse
# You should not put the trailing slash sign (but actually if you will, it should not affect the result...)
# You need to have write access to this directory
INPUTLMDDIR=/opt/keeper/stock/kozlov/calib201907/digibulder/alltel_90/

if [ ! -d "${INPUTLMDDIR}" ]; then
	echo -e "Directory ${INPUTLMDDIR} does not exist. Aborting."
	exit;
fi

# Please specify the path to the output directory where the resulting root files will be written
OUTPUTROOTDIR=/opt/keeper/stock/kozlov/calib201907/digibulder/alltel_90

# TODO check that ${OUTPUTROOTDIR} exists ???

# Please specify the input XML configuration file
SETUPFILE=./input/setupEXP1904.xml

# TODO check that ${SETUPFILE} exists

# Please specify the prefix for the generated lml files
# In principle, there are no constraints, but you may want to keep it the same as the directory name
PREFIX=alltel_90

# Please specify how many threads do you want to occupy
NTHREADS=4

# =============================================================================

# NOTE THAT ALL *.lml FILES WILL BE REMOVED FROM THE SOURCE DATA DIRECTORY
rm -fv ${INPUTLMDDIR}/${PREFIX}*.lml

# =============================================================================

FILES=${INPUTLMDDIR}/*.lmd

# Count input data files
NFILES=`ls -1q ${FILES} | wc -l`
# Count how many files should be processed by one thread
let NINBATCH=${NFILES}/${NTHREADS}+1

echo "nFiles=${NFILES}"
echo "nThreads=${NTHREADS}"
echo "nInBatch=(${NFILES}/${NTHREADS})+1=${NINBATCH}"

let IBATCH=1
let iFile=0

for FILE in ${FILES}
do
	let CURINBATCH=${iFile}%${NINBATCH}
	let IBATCH=${iFile}/${NINBATCH}
	echo "$FILE: curInBatch=${CURINBATCH} iBatch=${IBATCH}"
	echo $FILE >> ${INPUTLMDDIR}/${PREFIX}_${IBATCH}.lml
	let iFile++
done

echo -e "\e[1m\e[32m"`date` "Analysis started.\e[0m"

for THR in $(seq 1 ${NTHREADS})
do
	INPUTFILE=${INPUTLMDDIR}/${PREFIX}_${THR}.lml
	INPUTFILENAME=$(basename "${INPUTFILE}")
	OUTPUTFILE=${OUTPUTROOTDIR}/${INPUTFILENAME}.root
	AUTOSAVEFILE=${OUTPUTROOTDIR}/${INPUTFILENAME}_asf.root
	DATETIME=`date`

	TEXTERRFILE=${OUTPUTROOTDIR}/${INPUTFILENAME}_err.txt  # Default: textoutput/err.txt
	#TEXTERRFILE=/dev/null    # suppress error stream. Saves a little bit of hdd space

	TEXTOUTFILE=${OUTPUTROOTDIR}/${INPUTFILENAME}_out.txt  # Default: textoutput/out.txt # No reason to suppress


	# Cleanup analysis results ====================================================

	# Remove output file if it already exists from previous analysis runs
	rm -fv ${OUTPUTFILE}
	# Remove autosave file if it already exists from previous analysis runs
	rm -fv ${AUTOSAVEFILE}
	# Remove textual output if it already exists from previous analysis runs
	if [ -d textoutput ]; then
		rm -fv textoutput/*.txt
	else
		mkdir textoutput
	fi

	# =============================================================================

ANARUNINFO="========================== ANALYSIS RUN INFO ===================================
Current date and time                      ${DATETIME}
Processing input file                      ${INPUTFILE}
acording to the setup configuration file   ${SETUPFILE}
into output file                           ${OUTPUTFILE}
with autosave file                         ${AUTOSAVEFILE}
Text output file:                          ${TEXTOUTFILE}
Text error file:                           ${TEXTERRFILE}
Text summary files:                        textoutput/*.txt
================================================================================"

cat > ${TEXTOUTFILE} << EOF
${ANARUNINFO}
EOF

echo -e "\e[1m\e[34m${ANARUNINFO}\e[0m"

echo -e "\e[1m\e[32m"`date` "Analysis started.\e[0m"
${GO4SYS}/bin/go4analysis -v -lib libAccDaqUserAnalysis.so -asf ${AUTOSAVEFILE} -file ${INPUTFILE} -args ${OUTPUTFILE} ${SETUPFILE} >> ${TEXTOUTFILE} 2> ${TEXTERRFILE} &

done

echo -e "\e[1m\e[32mWaiting for all threads to finish\e[0m"
wait

echo -e "\e[1m\e[32m"`date` "Analysis finished.\e[0m"
