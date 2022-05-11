#!/bin/bash

InputFile=$1
Version=$2

Prefix=JSON

AllLevel="L1RC:L1RC:False L1FastJet:L1FastJet:False L2Relative:L2Relative:False L1L2L3:L1L2L3Res:True"
if [[ $Version == *"DATA" ]]; then
   AllLevel="L1RC:L1RC:False L1FastJet:L1FastJet:False L2Relative:L2Relative:False L1L2L3L2L3Res:L1L2L3Res:True L2Residual:L2Residual:False L2L3Residual:L2L3Residual:False"
fi

# AlgorithmList="AK4PF AK4PFchs AK4PFPuppi AK8PF AK8PFchs AK8PFPuppi"
AlgorithmList="AK4PFchs"

for Algorithm in $AlgorithmList
do
   mkdir -p Temp
   rm -f Temp/*

   Area=0.5026544
   if [[ $Algorithm == AK8* ]]; then
      Area=2.0106176
   fi

   for Level in $AllLevel
   do
      LevelTag=`echo $Level | cut -d ':' -f 1`
      LevelName=`echo $Level | cut -d ':' -f 2`
      LevelCompound="--IsCompound `echo $Level | cut -d ':' -f 3`"

      Common="--Input $InputFile --Prefix $Prefix --Version $Version --Algorithm $Algorithm --LevelTag $LevelTag --Level $LevelName $LevelCompound $ReducedEta $OldEta --FixArea $Area --OutputBase Temp/"

      python3 ExportCurve.py $Common --Dependent Eta --Min -5.0 --Max 5.0 --NBin -1 --FixPT 100 --FixRho 20
      python3 ExportCurve.py $Common --Dependent Eta --Min -5.0 --Max 5.0 --NBin -1 --FixPT 100 --FixRho 40
      python3 ExportCurve.py $Common --Dependent Eta --Min -5.0 --Max 5.0 --NBin -1 --FixPT 100 --FixRho 60
      python3 ExportCurve.py $Common --Dependent Eta --Min -5.0 --Max 5.0 --NBin -1 --FixPT 30 --FixRho 40
      python3 ExportCurve.py $Common --Dependent Eta --Min -5.0 --Max 5.0 --NBin -1 --FixPT 300 --FixRho 40
      python3 ExportCurve.py $Common --Dependent Rho --Min 0 --Max 70 --NBin 50 --FixPT 100 --FixEta 0.01
      python3 ExportCurve.py $Common --Dependent Rho --Min 0 --Max 70 --NBin 50 --FixPT 100 --FixEta -0.01
      python3 ExportCurve.py $Common --Dependent Rho --Min 0 --Max 70 --NBin 50 --FixPT 100 --FixEta 1.5
      python3 ExportCurve.py $Common --Dependent Rho --Min 0 --Max 70 --NBin 50 --FixPT 100 --FixEta 2.3
      python3 ExportCurve.py $Common --Dependent Rho --Min 0 --Max 70 --NBin 50 --FixPT 100 --FixEta 2.5
      python3 ExportCurve.py $Common --Dependent Rho --Min 0 --Max 70 --NBin 50 --FixPT 100 --FixEta 2.9
      python3 ExportCurve.py $Common --Dependent Rho --Min 0 --Max 70 --NBin 50 --FixPT 100 --FixEta 3.0
      python3 ExportCurve.py $Common --Dependent Rho --Min 0 --Max 70 --NBin 50 --FixPT 100 --FixEta 4.0
      python3 ExportCurve.py $Common --Dependent Rho --Min 0 --Max 70 --NBin 50 --FixPT 50 --FixEta -0.01
      python3 ExportCurve.py $Common --Dependent Rho --Min 0 --Max 70 --NBin 50 --FixPT 50 --FixEta 0.01
      python3 ExportCurve.py $Common --Dependent Rho --Min 0 --Max 70 --NBin 50 --FixPT 50 --FixEta 1.5
      python3 ExportCurve.py $Common --Dependent Rho --Min 0 --Max 70 --NBin 50 --FixPT 50 --FixEta 2.3
      python3 ExportCurve.py $Common --Dependent Rho --Min 0 --Max 70 --NBin 50 --FixPT 50 --FixEta 2.5
      python3 ExportCurve.py $Common --Dependent Rho --Min 0 --Max 70 --NBin 50 --FixPT 50 --FixEta 2.9
      python3 ExportCurve.py $Common --Dependent Rho --Min 0 --Max 70 --NBin 50 --FixPT 50 --FixEta 3.0
      python3 ExportCurve.py $Common --Dependent Rho --Min 0 --Max 70 --NBin 50 --FixPT 50 --FixEta 4.0
      python3 ExportCurve.py $Common --Dependent PT --Min 1 --Max 7000 --NBin 100 --FixEta -0.01 --FixRho 30
      python3 ExportCurve.py $Common --Dependent PT --Min 1 --Max 7000 --NBin 100 --FixEta -0.01 --FixRho 15
      python3 ExportCurve.py $Common --Dependent PT --Min 1 --Max 7000 --NBin 100 --FixEta 0.01 --FixRho 30
      python3 ExportCurve.py $Common --Dependent PT --Min 1 --Max 7000 --NBin 100 --FixEta 0.01 --FixRho 15
      python3 ExportCurve.py $Common --Dependent PT --Min 1 --Max 7000 --NBin 100 --FixEta 1.5 --FixRho 30
      python3 ExportCurve.py $Common --Dependent PT --Min 1 --Max 7000 --NBin 100 --FixEta 1.5 --FixRho 15
      python3 ExportCurve.py $Common --Dependent PT --Min 1 --Max 7000 --NBin 100 --FixEta 2.3 --FixRho 30
      python3 ExportCurve.py $Common --Dependent PT --Min 1 --Max 7000 --NBin 100 --FixEta 2.3 --FixRho 15
      python3 ExportCurve.py $Common --Dependent PT --Min 1 --Max 7000 --NBin 100 --FixEta 2.5 --FixRho 30
      python3 ExportCurve.py $Common --Dependent PT --Min 1 --Max 7000 --NBin 100 --FixEta 2.5 --FixRho 15
      python3 ExportCurve.py $Common --Dependent PT --Min 1 --Max 7000 --NBin 100 --FixEta 2.9 --FixRho 30
      python3 ExportCurve.py $Common --Dependent PT --Min 1 --Max 7000 --NBin 100 --FixEta 2.9 --FixRho 15
      python3 ExportCurve.py $Common --Dependent PT --Min 1 --Max 7000 --NBin 100 --FixEta 3.0 --FixRho 30
      python3 ExportCurve.py $Common --Dependent PT --Min 1 --Max 7000 --NBin 100 --FixEta 3.0 --FixRho 15
      python3 ExportCurve.py $Common --Dependent PT --Min 1 --Max 7000 --NBin 100 --FixEta 4.0 --FixRho 30
      python3 ExportCurve.py $Common --Dependent PT --Min 1 --Max 7000 --NBin 100 --FixEta 4.0 --FixRho 15
   done
   for Level in "Uncertainty:Total:False"
   do
      LevelTag=`echo $Level | cut -d ':' -f 1`
      LevelName=`echo $Level | cut -d ':' -f 2`
      LevelCompound="--IsCompound `echo $Level | cut -d ':' -f 3`"

      Common="--Input $InputFile --Prefix $Prefix --Version $Version --Algorithm $Algorithm --LevelTag $LevelTag --Level $LevelName $LevelCompound $ReducedEta $OldEta --FixArea $Area --OutputBase Temp/"

      python3 ExportCurve.py $Common --Dependent Eta --Min -5.0 --Max 5.0 --NBin -1 --FixPT 30 --FixRho 30
      python3 ExportCurve.py $Common --Dependent Eta --Min -5.0 --Max 5.0 --NBin -1 --FixPT 100 --FixRho 30
      python3 ExportCurve.py $Common --Dependent Eta --Min -5.0 --Max 5.0 --NBin -1 --FixPT 300 --FixRho 30
      python3 ExportCurve.py $Common --Dependent PT --Min 10 --Max 7000 --NBin 100 --FixEta 0.01 --FixRho 30
      python3 ExportCurve.py $Common --Dependent PT --Min 10 --Max 7000 --NBin 100 --FixEta -0.01 --FixRho 30
      python3 ExportCurve.py $Common --Dependent PT --Min 10 --Max 7000 --NBin 100 --FixEta 1.5 --FixRho 30
      python3 ExportCurve.py $Common --Dependent PT --Min 10 --Max 7000 --NBin 100 --FixEta 2.9 --FixRho 30
      python3 ExportCurve.py $Common --Dependent PT --Min 10 --Max 7000 --NBin 100 --FixEta 3.99 --FixRho 30
      python3 ExportCurve.py $Common --Dependent PT --Min 10 --Max 7000 --NBin 100 --FixEta 4.01 --FixRho 30
   done

   python3 MergeJSON.py --Input `ls Temp/*` --Output JSON_$Prefix${Version}_$Algorithm.json
done
