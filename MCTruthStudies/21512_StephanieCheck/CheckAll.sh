V=$1
A=$2
Input=$3

while read -r line
do
   RawPT=`echo $line | awk '{print $24}'`
   Eta=`echo $line | awk '{print $17}'`
   Phi=`echo $line | awk '{print $19}'`
   Area=`echo $line | awk '{print $21}'`
   Rho=`echo $line | awk '{print $10}'`

   echo $line
   echo

   ./Execute --PT $RawPT --Eta $Eta --Phi $Phi --Area $Area --Rho $Rho \
      --JEC textFiles/${V}/${V}_L1FastJet_${A}.txt,textFiles/${V}/${V}_L2Relative_${A}.txt,textFiles/${V}/${V}_L3Absolute_${A}.txt,textFiles/${V}/${V}_L2L3Residual_${A}.txt

   echo
   echo
   echo
done < $Input


