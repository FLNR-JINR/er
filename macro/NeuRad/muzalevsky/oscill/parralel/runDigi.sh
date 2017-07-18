#!/bin/bash

DIR=output_digi

for i in {1..10}; do
	nohup root -l -b -q "digi.C(40000, $i)" > $DIR/output_digi_$i.txt 2> $DIR/error_digi_$i.txt &
	#echo $DIR/output_digi_$i.txt
	#echo $DIR/error_digi_$i.txt
done
