#!/bin/bash

./run_parallel.sh
./run_analysis_parallel.sh

root -l merge_graphs.C

echo -e "\e[1m\e[32m========== Analysis merging finished ====== \e[0m"
