#!/bin/sh

source Setup_FileLocation.sh

for i in JRA HEM Full FullHEM
do
   jet_l2_correction_x \
      -input $Step3Output/Summed_${i}.root \
      -algs ak4pfchsl1 \
      -era ParallelMC${i} \
      -output l2.root \
      -outputDir Files/ \
      -makeCanvasVariable AbsCorVsJetPt:JetEta \
      -l2l3 true \
      -batch true \
      -histMet median \
      -delphes false \
      -maxFitIter 30 \
      -l2calofit DynamicMin \
      -l2pffit splineAkima \
      -ptclip 20

   mv Files/ParallelMC${i}_L2Relative_AK4PFchsl1.txt Files/ParallelMC${i}_L2Relative_AK4PFchs.txt
done



