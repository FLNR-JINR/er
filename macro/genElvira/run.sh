#!/bin/bash

root -l -b -q "run_test.C(10000)" > out.txt 2> err.txt

grep Tb out.txt | cut -d" " -f6 > data.txt

root -l "analyse.C"
