#!/bin/bash

for i in `ls Fragments/* | grep -v JER_`
do
   python ParseFile.py $i
done
