#! /bin/bash

root -l -b -q 'digibuilder.C("h7_ct_00_0001.lmd.root", "./results/h7_ct_00_0001.digi.root")'
root -l -b -q 'digicleaner.C("./results/h7_ct_00_0001.digi.root", "./results/h7_ct_00_0001.cleaned.root")'
root -l -b -q 'reco.C("./results/h7_ct_00_0001.cleaned.root", "./results/h7_ct_00_0001.reco.root")'