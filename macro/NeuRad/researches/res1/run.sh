#!/bin/bash

nohup root -l -q 'res1.C("png", "box", 2 , "/home/vitaliy/er/macro/NeuRad/sim/sim500MeV100000ev.root" , "digi500MeV100000ev.root" )' &

nohup root -l -q 'res1.C("png","box", 5 , "/home/vitaliy/er/macro/NeuRad/sim/sim500MeV100000ev.root" , "digi500MeV100000ev.root" )' &

nohup root -l -q 'res1.C("png","box", 12 , "/home/vitaliy/er/macro/NeuRad/sim/sim500MeV100000ev.root" , "digi500MeV100000ev.root" )' &

nohup root -l -q 'res1.C("png","box", 30 , "/home/vitaliy/er/macro/NeuRad/sim/sim500MeV100000ev.root" , "digi500MeV100000ev.root" )' &

nohup root -l -q 'res1.C("png","center", 2 , "/home/vitaliy/er/macro/NeuRad/sim/sim500MeV100000evfc.root" , "digi500MeV100000evfc.root" )' &

nohup root -l -q 'res1.C("png","center", 5 , "/home/vitaliy/er/macro/NeuRad/sim/sim500MeV100000evfc.root" , "digi500MeV100000evfc.root" )' &

nohup root -l -q 'res1.C("png","center", 12 , "/home/vitaliy/er/macro/NeuRad/sim/sim500MeV100000evfc.root" , "digi500MeV100000evfc.root" )' &

nohup root -l -q 'res1.C("png","center", 30 , "/home/vitaliy/er/macro/NeuRad/sim/sim500MeV100000evfc.root" , "digi500MeV100000evfc.root" )' &

