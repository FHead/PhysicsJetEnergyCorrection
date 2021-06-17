V=$1
A=$2
Input=$3

while read -r line
do
   RawPT=`echo $line | awk '{print $1}'`
   Eta=`echo $line | awk '{print $4}'`
   Phi=`echo $line | awk '{print $3}'`
   Area=`echo $line | awk '{print 0.503}'`
   Rho=`echo $line | awk '{print 0.1}'`

   echo $line
   echo

   ./Execute --PT $RawPT --Eta $Eta --Phi $Phi --Area $Area --Rho $Rho \
      --JEC textFiles/${V}/${V}_L1FastJet_${A}.txt,textFiles/${V}/${V}_L2Relative_${A}.txt,textFiles/${V}/${V}_L3Absolute_${A}.txt,textFiles/${V}/${V}_L2L3Residual_${A}.txt

   echo
   echo
   echo
done < $Input


