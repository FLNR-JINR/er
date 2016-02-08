#!/bin/bash

#nohup root -l -q 'NeuRad_sim.C(10000, "sim500MeV10000evrcv2.root", "par500MeV10000evrcv2.root", 0.5)' &
#nohup root -l -q 'NeuRad_sim.C(10000, "sim400MeV10000evrcv2.root", "par400MeV10000evrcv2.root", 0.4)' &

#nohup root -l -q 'NeuRad_sim.C(1000, "sim500MeV1000evrc.root", "par500MeV1000evrc.root", 0.5)' &
#nohup root -l -q 'NeuRad_sim.C(1000, "sim400MeV1000evrc.root", "par400MeV1000evrc.root", 0.4)' &

#nohup root -l -q 'NeuRad_digi.C(10000,"sim500MeV10000evrc.root", "par500MeV10000evrc.root", "digi500MeV10000evrc_onepe.root")' &
#nohup root -l -q 'NeuRad_digi_full.C(10000,"sim500MeV10000evrc.root", "par500MeV10000evrc.root", "digi500MeV10000evpe.root")' &
#nohup root -l -q 'NeuRad_digi_full.C(10000,"sim400MeV10000evrc.root", "par400MeV10000evrc.root", "digi400MeV10000evpe.root")' &

#nohup root -l -q 'NeuRad_digi_full.C(100000,"sim500MeV100000evrc.root", "par500MeV100000evrc.root", "digi500MeV100000evpe.root")' &
#nohup root -l -q 'NeuRad_digi_full.C(100000,"sim400MeV100000evrc.root", "par400MeV100000evrc.root", "digi400MeV100000evpe.root")' &

#nohup root -l -q 'NeuRad_digi_DT.C(10000,"sim500MeV10000evrc.root", "par500MeV10000evrc.root", "digi500MeV10000evrc_DT0001.root", 0.001)' &
#nohup root -l -q 'NeuRad_digi_DT.C(10000,"sim500MeV10000evrc.root", "par500MeV10000evrc.root", "digi500MeV10000evrc_DT001.root", 0.01)' &
#nohup root -l -q 'NeuRad_digi_DT.C(10000,"sim500MeV10000evrc.root", "par500MeV10000evrc.root", "digi500MeV10000evrc_DT01.root", 0.1)' &
#nohup root -l -q 'NeuRad_digi_DT.C(10000,"sim500MeV10000evrc.root", "par500MeV10000evrc.root", "digi500MeV10000evrc_DT1.root", 1.)' &
#nohup root -l -q 'NeuRad_digi.C(10000,"sim500MeV10000evrc.root", "par500MeV10000evrc.root", "digi500MeV10000evrc.root")' &
#nohup root -l -q 'NeuRad_sim.C(1000, "sim1.root", "par.root", 0.5)' &
#nohup root -l -q 'NeuRad_digi.C(1000,"sim.root", "par.root", "digi.root")' &

#nohup root -l -q 'NeuRad_digi.C(100000,"sim400MeV100000evrc.root", "par400MeV100000evrc.root", "digi400MeV100000evrc_woj.root")' &

#nohup root -l -q 'NeuRad_digi.C(100000,"sim500MeV100000evrc.root", "par500MeV100000evrc.root", "digi500MeV100000evrc_woj.root")' &

#nohup root -l -q 'NeuRad_digi.C(0,1000,"sim500MeV10000evrcv2.root", "par500MeV10000evrcv2.root", "digi500MeV10000evpe_0_1000.root")' &
#nohup root -l -q 'NeuRad_digi.C(1000,2000,"sim500MeV10000evrc.root", "par500MeV10000evrc.root", "digi500MeV10000evpe_1000_2000.root")' &
#nohup root -l -q 'NeuRad_digi.C(2000,3000,"sim500MeV10000evrc.root", "par500MeV10000evrc.root", "digi500MeV10000evpe_2000_3000.root")' &
#nohup root -l -q 'NeuRad_digi.C(3000,4000,"sim500MeV10000evrc.root", "par500MeV10000evrc.root", "digi500MeV10000evpe_3000_4000.root")' &
#nohup root -l -q 'NeuRad_digi.C(4000,5000,"sim500MeV10000evrc.root", "par500MeV10000evrc.root", "digi500MeV10000evpe_4000_5000.root")' &
#nohup root -l -q 'NeuRad_digi.C(5000,6000,"sim500MeV10000evrc.root", "par500MeV10000evrc.root", "digi500MeV10000evpe_5000_6000.root")' &
#nohup root -l -q 'NeuRad_digi.C(6000,7000,"sim500MeV10000evrc.root", "par500MeV10000evrc.root", "digi500MeV10000evpe_6000_7000.root")' &
#nohup root -l -q 'NeuRad_digi.C(6000,7000,"sim500MeV10000evrc.root", "par500MeV10000evrc.root", "digi500MeV10000evpe_7000_8000.root")' &
#nohup root -l -q 'NeuRad_digi.C(6000,7000,"sim500MeV10000evrc.root", "par500MeV10000evrc.root", "digi500MeV10000evpe_8000_9000.root")' &
#nohup root -l -q 'NeuRad_digi.C(6000,7000,"sim500MeV10000evrc.root", "par500MeV10000evrc.root", "digi500MeV10000evpe_9000_10000.root")' &

#nohup root -l -q 'merge.C("digi500MeV10000evpe_0_1000.root,digi500MeV10000evpe_1000_2000.root,digi500MeV10000evpe_2000_3000.root,digi500MeV10000evpe_3000_4000.root,digi500MeV10000evpe_4000_5000.root,digi500MeV10000evpe_5000_6000.root,digi500MeV10000evpe_6000_7000.root,digi500MeV10000evpe_7000_8000.root,digi500MeV10000evpe_8000_9000.root,digi500MeV10000evpe_9000_10000.root","digi500MeV10000evpe.root")' &

nohup root -l -q 'NeuRad_digi.C(0,10000,"sim500MeV10000evrcv2.root", "par500MeV10000evrcv2.root", "digi500MeV10000evpefull.root")' &

#nohup root -l -q 'NeuRad_digi.C(0,250,"sim500MeV1000evrc.root", "par500MeV1000evrc.root", "digi500MeV1000evpe_0_250.root")' &
#nohup root -l -q 'NeuRad_digi.C(250,500,"sim500MeV1000evrc.root", "par500MeV1000evrc.root", "digi500MeV1000evpe_250_500.root")' &
#nohup root -l -q 'NeuRad_digi.C(500,750,"sim500MeV1000evrc.root", "par500MeV1000evrc.root", "digi500MeV1000evpe_500_750.root")' &
#nohup root -l -q 'NeuRad_digi.C(750,1000,"sim500MeV1000evrc.root", "par500MeV1000evrc.root", "digi500MeV1000evpe_750_1000.root")' &
#nohup root -l -q 'merge.C("digi500MeV1000evpe_0_250.root,digi500MeV1000evpe_250_500.root,digi500MeV1000evpe_500_750.root,digi500MeV1000evpe_750_1000.root", "digi500MeV1000evpe.root")' & 
