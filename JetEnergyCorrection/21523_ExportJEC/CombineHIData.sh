#!/bin/bash

echo "var JECData ="
echo "{"
for Version in `ls Fragments/* FragmentsRAA/*`
do
   cat ${Version}
done
echo "}"
