#!/bin/bash

dir=archive/archive_from_er/case_1

if [ -d ${dir} ];then
  echo Start..
else
  echo "Uncorrect dir. Stop.."
  exit
fi

./remove.sh
wait

if [ -d ${dir}/N15/ ];then
  echo "N15 dir was found!"
  cp ${dir}/N15/* input/
else
  echo "${dir}/N15/ is missing!"
fi

if [ -d ${dir}/B11/ ];then
  echo "B11 dir was found!"
else
  echo "${dir}/B11/ is missing!"
  exit
fi

if [ -f ${dir}/B11/out.txt ];then
  cp ${dir}/B11/out.txt input/outB11.txt
else
  if [ -f ${dir}/B11/outB11.txt ];then
    cp ${dir}/B11/outB11.txt input/
  else
    echo "Not found any ${dir}/B11/out.* file!"
    exit
  fi
fi

if [ -f ${dir}/B11/target_int_num.txt ];then
  cp ${dir}/B11/target_int_num.txt input/target_int_numB11.txt
else
  if [ -f ${dir}/B11/target_int_numB11.txt ];then
    cp ${dir}/B11/target_int_numB11.txt input/
  else
    echo "Not found any target_int_num*"
    exit
  fi
fi

if [ -f ${dir}/B11/thetaCMB11.txt ];then
  cp ${dir}/B11/thetaCMB11.txt input/
else
  echo "thetaB11.txt is missing!"
  exit
fi

ls -l input/
