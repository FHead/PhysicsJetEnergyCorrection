for i in _RunABC_ _RunD_ _
do
   for j in AK4PF AK4PFPuppi AK4PFchs AK8PF AK8PFPuppi AK8PFchs
   do
      ./Execute \
         --JERFileName JRDatabase/Autumn18${i}V7b_MC/Autumn18${i}V7b_MC_PtResolution_${j}.txt \
         --JERSFFileName JRDatabase/Autumn18${i}V7b_MC/Autumn18${i}V7b_MC_SF_${j}.txt \
         --Output JRDatabase/Autumn18${i}V7b_DATA/Autumn18${i}V7b_DATA_PtResolution_${j}.txt
   done
done
