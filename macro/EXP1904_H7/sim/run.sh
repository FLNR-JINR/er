#!/bin/bash
set -e
root -l -b -q 'sim_digi.C(1000)'
root -l -b -q 'reco.C(1000)'
root -l -b -q 'check.C()'