#!/bin/bash

#nohup root -l -q 'res3.C(400, "png","box", 2 , 4, 100., "/home/vitaliy/er/macro/NeuRad/sim/sim400MeV100000evrc.root" , "/home/vitaliy/er/macro/NeuRad/sim/digi400MeV100000evrc.root" )' &
#nohup root -l -q 'res3.C(400, "png","box", 2 , 10, 100., "/home/vitaliy/er/macro/NeuRad/sim/sim400MeV100000evrc.root" , "/home/vitaliy/er/macro/NeuRad/sim/digi400MeV100000evrc.root" )' &
#nohup root -l -q 'res3.C(400, "png","box", 2 , 20, 100., "/home/vitaliy/er/macro/NeuRad/sim/sim400MeV100000evrc.root" , "/home/vitaliy/er/macro/NeuRad/sim/digi400MeV100000evrc.root" )' &
#nohup root -l -q 'res3.C(400, "png","box", 2 , 50, 100., "/home/vitaliy/er/macro/NeuRad/sim/sim400MeV100000evrc.root" , "/home/vitaliy/er/macro/NeuRad/sim/digi400MeV100000evrc.root" )' &

nohup root -l -q 'res3.C(500, "png","box", 2 , 4, 100., "/home/vitaliy/er/macro/NeuRad/sim/sim500MeV100000evrc.root" , "/home/vitaliy/er/macro/NeuRad/sim/digi500MeV100000evrc_woj.root" )' &
nohup root -l -q 'res3.C(500, "png","box", 2 , 10, 100., "/home/vitaliy/er/macro/NeuRad/sim/sim500MeV100000evrc.root" , "/home/vitaliy/er/macro/NeuRad/sim/digi500MeV100000evrc_woj.root" )' &
nohup root -l -q 'res3.C(500, "png","box", 2 , 20, 100., "/home/vitaliy/er/macro/NeuRad/sim/sim500MeV100000evrc.root" , "/home/vitaliy/er/macro/NeuRad/sim/digi500MeV100000evrc_woj.root" )' &
nohup root -l -q 'res3.C(500, "png","box", 2 , 50, 100., "/home/vitaliy/er/macro/NeuRad/sim/sim500MeV100000evrc.root" , "/home/vitaliy/er/macro/NeuRad/sim/digi500MeV100000evrc_woj.root" )' &