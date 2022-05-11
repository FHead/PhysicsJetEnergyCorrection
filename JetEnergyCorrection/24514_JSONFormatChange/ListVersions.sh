#!/bin/bash

echo "Versions = ["
for i in `ls Converted/ | sed "s/^JSON_//" | sed "s/.json$//"`
do
   V=`echo $i | sed "s/_AK/ AK/" | sed "s/_PFCalibration$/ PFCalibration/" | awk '{print $1}'`
   A=`echo $i | sed "s/_AK/ AK/" | sed "s/_PFCalibration$/ PFCalibration/" | awk '{print $2}'`

   echo "   {\"Version\": \"$V\", \"Algorithm\": \"$A\"},"
done
echo "]"

