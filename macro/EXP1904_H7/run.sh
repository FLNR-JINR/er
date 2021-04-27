#! /bin/bash
root -l -b -q 'digibuilder.C("h7_ct_00_0001.root", "h7_ct_00_0001.digi.root")'
root -l -b -q 'digicleaner.C("h7_ct_00_0001.digi.root", "h7_ct_00_0001.cleaned.root")'
# Run one event of simmulation to create setup geometry
root -l -b -q 'sim_digi.C(1)'
root -l -b -q 'reco.C("h7_ct_00_0001.cleaned.root", "reco_real.root")'