#!/bin/bash
STAANGLE=5
ENDANGLE=35

cd result
rm -fv *.root
cd -
rm -fv err.txt

root -l -b -q "thetas_writer.C(${STAANGLE}, ${ENDANGLE})" 
wait
root -l result/*.root
