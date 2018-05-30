#! /bin/bash
find ./ -name '*direct[0-9]*\.root' -delete
eventsCount=1000
if [ "$#" -gt "0" ]; then
	eventsCount=$1
else
	echo "An event count isn't passed as parameter and has default value 1000 "
fi
root -l -q "calib_direct.C(1, 4.7844 / 4, $eventsCount)" &
# root -l -q "calib_direct.C(2, 5.4895 / 4, $eventsCount)" &
# root -l -q "calib_direct.C(3, 6.0024 / 4, $eventsCount)" &
root -l -q "calib_direct.C(4, 7.6869 / 4, $eventsCount)" &
wait

echo merge_direct
root -l -q "merge_direct.C "
root -l