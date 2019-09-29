#!/bin/bash

if [ -z ${GO4SYS+x} ]; then
	echo "GO4SYS is unset. Aborting.";
	exit;
else
	echo "GO4SYS is set to '$GO4SYS'";
fi

# Please specify these variables to your needs ================================

# Please specify the path to a directory which contains the file you want to analyse
# You should not put the trailing slash sign (but actually if you will, it should not affect the result...)
INPUTLMDDIR=/home/komyour/soft/expertroot/macro/calibration/si_calibration

if [ ! -d "${INPUTLMDDIR}" ]; then
	echo -e "Directory ${INPUTLMDDIR} does not exist. Aborting."
	exit;
fi

# Please specify the path to the output directory where the resulting root files will be written
OUTPUTROOTDIR=/home/komyour/soft/expertroot/macro/calibration/si_calibration/input

# TODO check that ${OUTPUTROOTDIR} exists ???

# Please specify the input XML configuration file
SETUPFILE=/home/komyour/soft/expertroot/macro/calibration/si_calibration/setup2_exp201811.xml

# TODO check that ${SETUPFILE} exists

# Please specify input file name
# INPUTFILENAME=dsd_l_02_0001.lmd
INPUTFILENAME=dsd_20_l_03_0005.lmd

# Please specify number of events to process
NEVENTS=0 # Specify 0 to process the full file

TEXTERRFILE=textoutput/err.txt  # Default: textoutput/err.txt
#TEXTERRFILE=/dev/null    # suppress error stream. Saves a little bit of hdd space

TEXTOUTFILE=textoutput/out.txt  # Default: textoutput/out.txt # No reason to suppress

# =============================================================================
# In principle, one should not change anything below this line
# =============================================================================

INPUTFILE=${INPUTLMDDIR}/${INPUTFILENAME}
OUTPUTFILE=${OUTPUTROOTDIR}/${INPUTFILENAME}.root
AUTOSAVEFILE=${OUTPUTROOTDIR}/${INPUTFILENAME}_asf.root
DATETIME=`date`

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

if [ ${NEVENTS} -eq 0 ]; then
	${GO4SYS}/bin/go4analysis -v -lib libAccDaqUserAnalysis.so -asf ${AUTOSAVEFILE} -file ${INPUTFILE} -args ${OUTPUTFILE} ${SETUPFILE} >> ${TEXTOUTFILE} 2> ${TEXTERRFILE}
else
	${GO4SYS}/bin/go4analysis -v -lib libAccDaqUserAnalysis.so -number ${NEVENTS} -asf ${AUTOSAVEFILE} -file ${INPUTFILE} -args ${OUTPUTFILE} ${SETUPFILE} >> ${TEXTOUTFILE} 2> ${TEXTERRFILE}
fi

echo -e "\e[1m\e[32m"`date` "Analysis finished.\e[0m"
