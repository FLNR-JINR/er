#! /bin/bash

root -l -b -q 'sim_digi.C(1000)'
root -l -b -q 'reco.C("sim_digi.root", "reco_sim.root")'