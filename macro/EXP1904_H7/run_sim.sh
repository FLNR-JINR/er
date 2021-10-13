#/bin/bash

#root -l -b -q 'sim_digi.C(10000)'
#root -l -b -q 'reco_by_sim.C("sim_digi.root", "reco.root")'
#root -l -b -q 'select_by_sim.C("sim_digi.root", "reco.root", "selected.root")'
root -l -b -q 'analysis.C("selected.root", "analysis.root")'
root -l 'drawH7.C'
