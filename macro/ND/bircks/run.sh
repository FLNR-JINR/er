#! /bin/bash
root -l -q "run.C(1000,0.1)" > 1.dat 2> 1.dat &
root -l -q "run.C(1000,0.2)" > 2.dat 2> 2.dat &
root -l -q "run.C(1000,0.3)" > 3.dat 2> 3.dat &