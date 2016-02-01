#!/bin/bash

#nohup root -l -q 'NeuRad_sim.C(100000, "sim500MeV10000evrc.root", "par500MeV10000evrc.root", 0.5)' &
#nohup root -l -q 'NeuRad_sim.C(100000, "sim400MeV10000evrc.root", "par400MeV10000evrc.root", 0.4)' &

<<<<<<< HEAD
#nohup root -l -q 'NeuRad_digi.C(10000,"sim500MeV10000evrc.root", "par500MeV10000evrc.root", "digi500MeV10000evrc_onepe.root")' &
#nohup root -l -q 'NeuRad_digi_full.C(10000,"sim500MeV10000evrc.root", "par500MeV10000evrc.root", "digi500MeV10000evpe.root")' &
#nohup root -l -q 'NeuRad_digi_full.C(10000,"sim400MeV10000evrc.root", "par400MeV10000evrc.root", "digi400MeV10000evpe.root")' &
=======
nohup root -l -q 'NeuRad_digi_full.C(100000,"sim500MeV100000evrc.root", "par500MeV100000evrc.root", "digi500MeV100000evpe.root")' &
nohup root -l -q 'NeuRad_digi_full.C(100000,"sim400MeV100000evrc.root", "par400MeV100000evrc.root", "digi400MeV100000evpe.root")' &

>>>>>>> bbe70682f27e08f0fad578c53f17976d65d4812c
#nohup root -l -q 'NeuRad_digi_DT.C(10000,"sim500MeV10000evrc.root", "par500MeV10000evrc.root", "digi500MeV10000evrc_DT0001.root", 0.001)' &
#nohup root -l -q 'NeuRad_digi_DT.C(10000,"sim500MeV10000evrc.root", "par500MeV10000evrc.root", "digi500MeV10000evrc_DT001.root", 0.01)' &
#nohup root -l -q 'NeuRad_digi_DT.C(10000,"sim500MeV10000evrc.root", "par500MeV10000evrc.root", "digi500MeV10000evrc_DT01.root", 0.1)' &
#nohup root -l -q 'NeuRad_digi_DT.C(10000,"sim500MeV10000evrc.root", "par500MeV10000evrc.root", "digi500MeV10000evrc_DT1.root", 1.)' &
#nohup root -l -q 'NeuRad_digi.C(10000,"sim500MeV10000evrc.root", "par500MeV10000evrc.root", "digi500MeV10000evrc.root")' &
#nohup root -l -q 'NeuRad_sim.C(1000, "sim1.root", "par.root", 0.5)' &
#nohup root -l -q 'NeuRad_digi.C(1000,"sim.root", "par.root", "digi.root")' &

#nohup root -l -q 'NeuRad_digi.C(100000,"sim400MeV100000evrc.root", "par400MeV100000evrc.root", "digi400MeV100000evrc_woj.root")' &

#nohup root -l -q 'NeuRad_digi.C(100000,"sim500MeV100000evrc.root", "par500MeV100000evrc.root", "digi500MeV100000evrc_woj.root")' &
<<<<<<< HEAD

#nohup root -l -q 'NeuRad_digi_full.C(0,5,"sim500MeV10000evrc.root", "par500MeV10000evrc.root", "digi500MeV10000evpe_0_5.root")' &
#nohup root -l -q 'NeuRad_digi_full.C(5,10,"sim500MeV10000evrc.root", "par500MeV10000evrc.root", "digi500MeV10000evpe_5_10.root")' &

nohup root -l -q 'merge.C("digi500MeV10000evpe_0_5.root", "digi500MeV10000evpe_5_10.root", "digi500MeV10evpe.root")' 
=======
>>>>>>> bbe70682f27e08f0fad578c53f17976d65d4812c
