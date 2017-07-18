#!/bin/bash

DIR=output_sim

for i in {1..10}; do
	nohup root -l -b -q "sim.C(40000, $i)" > $DIR/output_sim_$i.txt 2> $DIR/error_sim_$i.txt &
	#echo $DIR/output_sim_$i.txt
	#echo $DIR/error_sim_$i.txt
done
