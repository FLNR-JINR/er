#! /bin/bash
rm *.root
root -l -q "calib_first.C(1, 4.7844 / 4)" &
root -l -q "calib_first.C(2, 5.4895 / 4)" &
root -l -q "calib_first.C(3, 6.0024 / 4)" &
root -l -q "calib_first.C(4, 7.6869 / 4)" &
wait

echo merge
root -l -q "merge.C "