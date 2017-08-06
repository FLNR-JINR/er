#!/bin/bash

DIR=output_raw

for i in {1..10}; do
        nohup root -l -b -q "SimToRaw.C(60000, $i)" > $DIR/output_raw_$i.txt 2> $DIR/error_raw_$i.txt &
        #echo $DIR/output_raw_$i.txt
        #echo $DIR/error_raw_$i.txt
done

