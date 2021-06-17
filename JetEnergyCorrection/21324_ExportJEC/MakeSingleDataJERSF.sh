#!/bin/bash

Version=$1

AllLevel="EtaResolution:EtaResolution PtResolution:PtResolution PhiResolution:PhiResolution"
AlgorithmList="AK4PF AK4PFchs AK4PFPuppi AK8PF AK8PFchs AK8PFPuppi"

echo "   JER_$Version:"
echo "   {"
for Algorithm in $AlgorithmList
do
   echo "      $Algorithm:"
   echo "      {"
   bash ExportJERSF.sh JRDatabase/${Version}/${Version}_SF_${Algorithm}.txt
   echo "      },"
done
echo "   },"
