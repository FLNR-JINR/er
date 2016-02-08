#!/bin/bash

#nohup root -l -q 'res10.C(500, "png","box", 2 , 4, 100., "/home/vitaliy/er/macro/NeuRad/sim.root",  "/home/vitaliy/er/macro/NeuRad/par.root" , "/home/vitaliy/er/macro/NeuRad/digi.root" )' &
#nohup root -l -q 'res3.C(400, "png","box", 2 , 10, 100., "/home/vitaliy/er/macro/NeuRad/sim/sim400MeV100000evrc.root" , "/home/vitaliy/er/macro/NeuRad/sim/digi400MeV100000evpe.root" )' &
#nohup root -l -q 'res3.C(400, "png","box", 2 , 20, 100., "/home/vitaliy/er/macro/NeuRad/sim/sim400MeV100000evrc.root" , "/home/vitaliy/er/macro/NeuRad/sim/digi400MeV100000evpe.root" )' &
#nohup root -l -q 'res3.C(400, "png","box", 2 , 50, 100., "/home/vitaliy/er/macro/NeuRad/sim/sim400MeV100000evrc.root" , "/home/vitaliy/er/macro/NeuRad/sim/digi400MeV100000evpe.root" )' &

nohup root -l -q 'res10.C(500, "png","box", 2 , 4, 100., "/home/vitaliy/er/macro/NeuRad/sim/sim500MeV10000evrc.root" , "/home/vitaliy/er/macro/NeuRad/sim/par500MeV10000evrc.root", "/home/vitaliy/er/macro/NeuRad/sim/digi500MeV100000evpe.root" )' &
#nohup root -l -q 'res3.C(500, "png","box", 2 , 10, 100., "/home/vitaliy/er/macro/NeuRad/sim/sim500MeV10000evrc.root" , "/home/vitaliy/er/macro/NeuRad/sim/digi500MeV100000evpe.root" )' &
#nohup root -l -q 'res3.C(500, "png","box", 2 , 20, 100., "/home/vitaliy/er/macro/NeuRad/sim/sim500MeV10000evrc.root" , "/home/vitaliy/er/macro/NeuRad/sim/digi500MeV100000evrcpe.root" )' &
#nohup root -l -q 'res3.C(500, "png","box", 2 , 50, 100., "/home/vitaliy/er/macro/NeuRad/sim/sim500MeV10000evrc.root" , "/home/vitaliy/er/macro/NeuRad/sim/digi500MeV100000evpe.root" )' &
