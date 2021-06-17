#!/bin/bash

Version=$1

AllLevel="L1RC:L1RC L1FastJet:L1FastJet L2Relative:L2Relative L1L2L3:L1FastJet,L2Relative"
if [[ $Version == *"DATA" ]]; then
   AllLevel="L1RC:L1RC L1FastJet:L1FastJet L2Relative:L2Relative L1L2L3:L1FastJet,L2Relative L2Residual:L2Residual L2L3Residual:L2L3Residual L1L2L3L2L3Res:L1FastJet,L2Relative,L2L3Residual"
fi
if [[ $Version == "HLT"* ]]; then
   AllLevel="L1FastJet:L1FastJet L2Relative:L2Relative L1L2L3:L1FastJet,L2Relative"
fi
if [[ $Version == *"Flavor"* ]]; then
   AllLevel="L1FastJet:L1FastJet L2Relative:L2Relative L1L2L3:L1FastJet,L2Relative L5b:b_L5Flavor L5c:c_L5Flavor L5g:g_L5Flavor L5s:s_L5Flavor L5ud:ud_L5Flavor"
fi

AlgorithmList="AK4PF AK4PFchs AK4PFPuppi AK8PF AK8PFchs AK8PFPuppi"
ExtraAlgorithmList=""
if [[ $Version == *"DATA" ]]; then
   AlgorithmList="AK4PFchs"
   ExtraAlgorithmList="AK4PF AK8PF AK8PFchs"
fi
if [[ $Version == "HLT"* ]]; then
   AlgorithmList="AK4PF AK8PF AK4Calo AK8Calo"
fi
if [[ $Version == *"Flavor"* ]]; then
   AlgorithmList="AK4PFchs"
fi
if [[ $Version == *_"HI"_* ]]; then
   AlgorithmList="AK2PF AK3PF AK4PF AK5PF AK6PF"
fi

DoUncertainty=Yes
if [[ $Version == "HLT"* ]]; then
   DoUncertainty=No
fi

MaxEta=5.18
ReducedEta=false
if [[ $Version == "HLT"* ]]; then
   MaxEta=4.999
   ReducedEta=true
fi

echo "   $Version:"
echo "   {"
for Algorithm in $AlgorithmList
do
   echo "      $Algorithm:"
   echo "      {"
   for Level in $AllLevel
   do
      LevelTag=`echo $Level | cut --delim=':' --field=1`
      LevelName=`echo $Level | cut --delim=':' --field=2`
      echo "         $LevelTag:"
      echo "         {"
      if [[ $LevelTag == "L1FastJet" ]] || [[ $LevelTag == "L1L2L3" ]] || [[ $LevelTag == "L1RC" ]]; then
         ./Execute --Version $Version --Algorithm $Algorithm --Level $LevelName \
            --Dependent Eta --Min -$MaxEta --Max $MaxEta --NBin -1 --FixPT 100 --FixRho 20 --ReducedEta $ReducedEta
      fi
      ./Execute --Version $Version --Algorithm $Algorithm --Level $LevelName \
         --Dependent Eta --Min -$MaxEta --Max $MaxEta --NBin -1 --FixPT 100 --FixRho 40 --ReducedEta $ReducedEta
      if [[ $LevelTag == "L1FastJet" ]] || [[ $LevelTag == "L1L2L3" ]] || [[ $LevelTag == "L1RC" ]]; then
         ./Execute --Version $Version --Algorithm $Algorithm --Level $LevelName \
            --Dependent Eta --Min -$MaxEta --Max $MaxEta --NBin -1 --FixPT 100 --FixRho 60 --ReducedEta $ReducedEta
      fi
      ./Execute --Version $Version --Algorithm $Algorithm --Level $LevelName \
         --Dependent Eta --Min -$MaxEta --Max $MaxEta --NBin -1 --FixPT 30 --FixRho 40 --ReducedEta $ReducedEta
      ./Execute --Version $Version --Algorithm $Algorithm --Level $LevelName \
         --Dependent Eta --Min -$MaxEta --Max $MaxEta --NBin -1 --FixPT 300 --FixRho 40 --ReducedEta $ReducedEta
      if [[ $LevelTag == "L1FastJet" ]] || [[ $LevelTag == "L1L2L3" ]] || [[ $LevelTag == "L1RC" ]]; then
         ./Execute --Version $Version --Algorithm $Algorithm --Level $LevelName \
            --Dependent Rho --Min 0 --Max 70 --NBin 50 --FixPT 100 --FixEta 0.0 --ReducedEta $ReducedEta
         ./Execute --Version $Version --Algorithm $Algorithm --Level $LevelName \
            --Dependent Rho --Min 0 --Max 70 --NBin 50 --FixPT 100 --FixEta 1.5 --ReducedEta $ReducedEta
         ./Execute --Version $Version --Algorithm $Algorithm --Level $LevelName \
            --Dependent Rho --Min 0 --Max 70 --NBin 50 --FixPT 100 --FixEta 2.3 --ReducedEta $ReducedEta
         ./Execute --Version $Version --Algorithm $Algorithm --Level $LevelName \
            --Dependent Rho --Min 0 --Max 70 --NBin 50 --FixPT 100 --FixEta 2.9 --ReducedEta $ReducedEta
         ./Execute --Version $Version --Algorithm $Algorithm --Level $LevelName \
            --Dependent Rho --Min 0 --Max 70 --NBin 50 --FixPT 100 --FixEta 3.0 --ReducedEta $ReducedEta
         ./Execute --Version $Version --Algorithm $Algorithm --Level $LevelName \
            --Dependent Rho --Min 0 --Max 70 --NBin 50 --FixPT 100 --FixEta 4.0 --ReducedEta $ReducedEta
      fi
      ./Execute --Version $Version --Algorithm $Algorithm --Level $LevelName \
         --Dependent PT --Min 1 --Max 7000 --NBin 100 --FixEta 0 --FixRho 30 --ReducedEta $ReducedEta
      ./Execute --Version $Version --Algorithm $Algorithm --Level $LevelName \
         --Dependent PT --Min 1 --Max 7000 --NBin 100 --FixEta 1.5 --FixRho 30 --ReducedEta $ReducedEta
      ./Execute --Version $Version --Algorithm $Algorithm --Level $LevelName \
         --Dependent PT --Min 1 --Max 7000 --NBin 100 --FixEta 2.3 --FixRho 30 --ReducedEta $ReducedEta
      ./Execute --Version $Version --Algorithm $Algorithm --Level $LevelName \
         --Dependent PT --Min 1 --Max 7000 --NBin 100 --FixEta 2.9 --FixRho 30 --ReducedEta $ReducedEta
      ./Execute --Version $Version --Algorithm $Algorithm --Level $LevelName \
         --Dependent PT --Min 1 --Max 7000 --NBin 100 --FixEta 3.0 --FixRho 30 --ReducedEta $ReducedEta
      ./Execute --Version $Version --Algorithm $Algorithm --Level $LevelName \
         --Dependent PT --Min 1 --Max 7000 --NBin 100 --FixEta 4.0 --FixRho 30 --ReducedEta $ReducedEta
      echo "         },"
   done
   if [[ $DoUncertainty == "Yes" ]]; then
      for Level in Uncertainty
      do
         echo "         $Level:"
         echo "         {"
         ./ExecuteUncertainty --Version $Version --Algorithm $Algorithm --Level $Level \
            --Dependent Eta --Min -$MaxEta --Max $MaxEta --NBin -1 --FixPT 30 --ReducedEta $ReducedEta
         ./ExecuteUncertainty --Version $Version --Algorithm $Algorithm --Level $Level \
            --Dependent Eta --Min -$MaxEta --Max $MaxEta --NBin -1 --FixPT 100 --ReducedEta $ReducedEta
         ./ExecuteUncertainty --Version $Version --Algorithm $Algorithm --Level $Level \
            --Dependent Eta --Min -$MaxEta --Max $MaxEta --NBin -1 --FixPT 300 --ReducedEta $ReducedEta
         ./ExecuteUncertainty --Version $Version --Algorithm $Algorithm --Level $Level \
            --Dependent PT --Min 10 --Max 7000 --NBin 100 --FixEta 0.0 --ReducedEta $ReducedEta
         ./ExecuteUncertainty --Version $Version --Algorithm $Algorithm --Level $Level \
            --Dependent PT --Min 10 --Max 7000 --NBin 100 --FixEta 2.9 --ReducedEta $ReducedEta
         ./ExecuteUncertainty --Version $Version --Algorithm $Algorithm --Level $Level \
            --Dependent PT --Min 10 --Max 7000 --NBin 100 --FixEta 1.5 --ReducedEta $ReducedEta
         ./ExecuteUncertainty --Version $Version --Algorithm $Algorithm --Level $Level \
            --Dependent PT --Min 10 --Max 7000 --NBin 100 --FixEta 4.0 --ReducedEta $ReducedEta
         echo "         },"
      done
   fi
   echo "      },"
done
for Algorithm in $ExtraAlgorithmList
do
   echo "      $Algorithm:"
   echo "      {"
   for Level in L1RC:L1RC
   do
      LevelTag=`echo $Level | cut --delim=':' --field=1`
      LevelName=`echo $Level | cut --delim=':' --field=2`
      echo "         $LevelTag:"
      echo "         {"
      if [[ $LevelTag == "L1FastJet" ]] || [[ $LevelTag == "L1L2L3" ]] || [[ $LevelTag == "L1RC" ]]; then
         ./Execute --Version $Version --Algorithm $Algorithm --Level $LevelName \
            --Dependent Eta --Min -$MaxEta --Max $MaxEta --NBin -1 --FixPT 100 --FixRho 20 --ReducedEta $ReducedEta
      fi
      ./Execute --Version $Version --Algorithm $Algorithm --Level $LevelName \
         --Dependent Eta --Min -$MaxEta --Max $MaxEta --NBin -1 --FixPT 100 --FixRho 40 --ReducedEta $ReducedEta
      if [[ $LevelTag == "L1FastJet" ]] || [[ $LevelTag == "L1L2L3" ]] || [[ $LevelTag == "L1RC" ]]; then
         ./Execute --Version $Version --Algorithm $Algorithm --Level $LevelName \
            --Dependent Eta --Min -$MaxEta --Max $MaxEta --NBin -1 --FixPT 100 --FixRho 60 --ReducedEta $ReducedEta
      fi
      ./Execute --Version $Version --Algorithm $Algorithm --Level $LevelName \
         --Dependent Eta --Min -$MaxEta --Max $MaxEta --NBin -1 --FixPT 30 --FixRho 40 --ReducedEta $ReducedEta
      ./Execute --Version $Version --Algorithm $Algorithm --Level $LevelName \
         --Dependent Eta --Min -$MaxEta --Max $MaxEta --NBin -1 --FixPT 300 --FixRho 40 --ReducedEta $ReducedEta
      if [[ $LevelTag == "L1FastJet" ]] || [[ $LevelTag == "L1L2L3" ]] || [[ $LevelTag == "L1RC" ]]; then
         ./Execute --Version $Version --Algorithm $Algorithm --Level $LevelName \
            --Dependent Rho --Min 0 --Max 70 --NBin 50 --FixPT 100 --FixEta 0.0 --ReducedEta $ReducedEta
         ./Execute --Version $Version --Algorithm $Algorithm --Level $LevelName \
            --Dependent Rho --Min 0 --Max 70 --NBin 50 --FixPT 100 --FixEta 1.5 --ReducedEta $ReducedEta
         ./Execute --Version $Version --Algorithm $Algorithm --Level $LevelName \
            --Dependent Rho --Min 0 --Max 70 --NBin 50 --FixPT 100 --FixEta 2.3 --ReducedEta $ReducedEta
         ./Execute --Version $Version --Algorithm $Algorithm --Level $LevelName \
            --Dependent Rho --Min 0 --Max 70 --NBin 50 --FixPT 100 --FixEta 2.9 --ReducedEta $ReducedEta
         ./Execute --Version $Version --Algorithm $Algorithm --Level $LevelName \
            --Dependent Rho --Min 0 --Max 70 --NBin 50 --FixPT 100 --FixEta 3.0 --ReducedEta $ReducedEta
         ./Execute --Version $Version --Algorithm $Algorithm --Level $LevelName \
            --Dependent Rho --Min 0 --Max 70 --NBin 50 --FixPT 100 --FixEta 4.0 --ReducedEta $ReducedEta
      fi
      ./Execute --Version $Version --Algorithm $Algorithm --Level $LevelName \
         --Dependent PT --Min 1 --Max 7000 --NBin 100 --FixEta 0 --FixRho 30 --ReducedEta $ReducedEta
      ./Execute --Version $Version --Algorithm $Algorithm --Level $LevelName \
         --Dependent PT --Min 1 --Max 7000 --NBin 100 --FixEta 1.5 --FixRho 30 --ReducedEta $ReducedEta
      ./Execute --Version $Version --Algorithm $Algorithm --Level $LevelName \
         --Dependent PT --Min 1 --Max 7000 --NBin 100 --FixEta 2.3 --FixRho 30 --ReducedEta $ReducedEta
      ./Execute --Version $Version --Algorithm $Algorithm --Level $LevelName \
         --Dependent PT --Min 1 --Max 7000 --NBin 100 --FixEta 2.9 --FixRho 30 --ReducedEta $ReducedEta
      ./Execute --Version $Version --Algorithm $Algorithm --Level $LevelName \
         --Dependent PT --Min 1 --Max 7000 --NBin 100 --FixEta 3.0 --FixRho 30 --ReducedEta $ReducedEta
      ./Execute --Version $Version --Algorithm $Algorithm --Level $LevelName \
         --Dependent PT --Min 1 --Max 7000 --NBin 100 --FixEta 4.0 --FixRho 30 --ReducedEta $ReducedEta
      echo "         },"
   done
   echo "      },"
done
echo "   },"
