#!/bin/bash

echo "var JECData ="
echo "{"
for Version in `ls Fragments/*HI*` Fragments/Autumn18_V8_MC.txt Fragments/Autumn18_Run*_V8_DATA.txt
do
   cat ${Version}
done
echo "}"
