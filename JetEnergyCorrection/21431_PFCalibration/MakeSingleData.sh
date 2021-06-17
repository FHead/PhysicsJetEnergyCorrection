#!/bin/bash

Version=$1

AlgorithmList="PFCalibration"
AllLevel="EHadron:true HHadron:false"

echo "   $Version:"
echo "   {"
for Algorithm in $AlgorithmList
do
   echo "      $Algorithm:"
   echo "      {"
   for Level in $AllLevel
   do
      LevelTag=`echo $Level | cut --delim=':' --field=1`
      LevelBool=`echo $Level | cut --delim=':' --field=2`
      echo "         $LevelTag:"
      echo "         {"
      ./Execute --Input ${Version}.txt --DoE $LevelBool \
         --Dependent Eta --Min 0.00 --Max 3.02 --NBin 50 --FixPT 5
      ./Execute --Input ${Version}.txt --DoE $LevelBool \
         --Dependent Eta --Min 0.00 --Max 3.02 --NBin 50 --FixPT 20
      ./Execute --Input ${Version}.txt --DoE $LevelBool \
         --Dependent Eta --Min 0.00 --Max 3.02 --NBin 50 --FixPT 50
      ./Execute --Input ${Version}.txt --DoE $LevelBool \
         --Dependent Eta --Min 0.00 --Max 3.02 --NBin 50 --FixPT 100
      ./Execute --Input ${Version}.txt --DoE $LevelBool \
         --Dependent Eta --Min 0.00 --Max 3.02 --NBin 50 --FixPT 500
      ./Execute --Input ${Version}.txt --DoE $LevelBool \
         --Dependent PT --Min 1.00 --Max 7000 --NBin 50 --FixEta 0.0
      ./Execute --Input ${Version}.txt --DoE $LevelBool \
         --Dependent PT --Min 1.00 --Max 7000 --NBin 50 --FixEta 1.5
      ./Execute --Input ${Version}.txt --DoE $LevelBool \
         --Dependent PT --Min 1.00 --Max 7000 --NBin 50 --FixEta 2.5
      ./Execute --Input ${Version}.txt --DoE $LevelBool \
         --Dependent PT --Min 1.00 --Max 7000 --NBin 50 --FixEta 2.75
      ./Execute --Input ${Version}.txt --DoE $LevelBool \
         --Dependent PT --Min 1.00 --Max 7000 --NBin 50 --FixEta 3.0
      echo "         },"
   done
   echo "      },"
done
echo "   },"
