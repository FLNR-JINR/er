#! /bin/bash

root -l -b -q 'digibuilder.C('"\"/zfs/store5.hydra.local/projects/user-projects/e/er/exp/exp201904/converted/$1.root\""', '"\"./results/$1.digi.root\""')'
root -l -b -q 'digicleaner.C('"\"./results/$1.digi.root\""', '"\"./results/$1.cleaned.root\""')'
root -l -b -q 'reco.C('"\"./results/$1.cleaned.root\""', '"\"./results_new/$1.reco.root\""')'
