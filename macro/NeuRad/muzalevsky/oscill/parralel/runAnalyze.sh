#!/bin/bash

DIR=output_analyze

for i in {1..10}; do
        nohup root -l -b -q "RawToAnalyze.C(3000, $i)" > $DIR/output_ana_$i.txt 2> $DIR/error_ana_$i.txt &
        #echo $DIR/output_ana_$i.txt
        #echo $DIR/error_ana_$i.txt
done

