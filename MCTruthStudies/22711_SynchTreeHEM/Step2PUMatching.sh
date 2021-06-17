#!/bin/sh --login

#BSUB -q 1nh

WorkDir=$1
NoPUFile=$2
PUFile=$3
Output=$4
ID=$5

source $WorkDir/Setup_CMSSW.sh

pwd

if [[ $NoPUFile != *":"* ]]; then
   cp $NoPUFile NoPUFile.root
else
   hadd -f -k NoPUFile.root `echo $NoPUFile | tr ':' ' '`
fi

if [[ $PUFile != *":"* ]]; then
   cp $PUFile PUFile.root
else
   hadd -f -k PUFile.root `echo $PUFile | tr ':' ' '`
fi

cp $WorkDir/Files/*PileupHistogram.root .

echo Current directory content
ls
echo

jet_synchtree_x \
   -basepath '//' \
   -samplePU `pwd`/PUFile.root \
   -sampleNoPU `pwd`/NoPUFile.root \
   -algo1 ak4pfchs \
   -algo2 ak4pfchs \
   -iftest false \
   -maxEvts 10000000 \
   -ApplyJEC false \
   -outputPath `pwd`/ \
   -npvRhoNpuBinWidth 10 \
   -NBinsNpvRhoNpu 6 \
   -MCPUReWeighting `pwd`/MyMCPileupHistogram.root \
   -DataPUReWeighting `pwd`/MyDataPileupHistogram.root \
   -useweight false \
   -nrefmax 3 \
   -doNotSave false \
   -forceNoPU false
jet_synchtree_x \
   -basepath '//' \
   -samplePU `pwd`/PUFile.root \
   -sampleNoPU `pwd`/NoPUFile.root \
   -algo1 ak4pf \
   -algo2 ak4pf \
   -iftest false \
   -maxEvts 10000000 \
   -ApplyJEC false \
   -outputPath `pwd`/ \
   -npvRhoNpuBinWidth 10 \
   -NBinsNpvRhoNpu 6 \
   -MCPUReWeighting `pwd`/MyMCPileupHistogram.root \
   -DataPUReWeighting `pwd`/MyDataPileupHistogram.root \
   -useweight false \
   -nrefmax 3 \
   -doNotSave false \
   -forceNoPU false
jet_synchtree_x \
   -basepath '//' \
   -samplePU `pwd`/PUFile.root \
   -sampleNoPU `pwd`/NoPUFile.root \
   -algo1 ak4puppi \
   -algo2 ak4puppi \
   -iftest false \
   -maxEvts 10000000 \
   -ApplyJEC false \
   -outputPath `pwd`/ \
   -npvRhoNpuBinWidth 10 \
   -NBinsNpvRhoNpu 6 \
   -MCPUReWeighting `pwd`/MyMCPileupHistogram.root \
   -DataPUReWeighting `pwd`/MyDataPileupHistogram.root \
   -useweight false \
   -nrefmax 3 \
   -doNotSave false \
   -forceNoPU false
jet_synchtree_x \
   -basepath '//' \
   -samplePU `pwd`/PUFile.root \
   -sampleNoPU `pwd`/NoPUFile.root \
   -algo1 ak8pfchs \
   -algo2 ak8pfchs \
   -iftest false \
   -maxEvts 10000000 \
   -ApplyJEC false \
   -outputPath `pwd`/ \
   -npvRhoNpuBinWidth 10 \
   -NBinsNpvRhoNpu 6 \
   -MCPUReWeighting `pwd`/MyMCPileupHistogram.root \
   -DataPUReWeighting `pwd`/MyDataPileupHistogram.root \
   -useweight false \
   -nrefmax 3 \
   -doNotSave false \
   -forceNoPU false
jet_synchtree_x \
   -basepath '//' \
   -samplePU `pwd`/PUFile.root \
   -sampleNoPU `pwd`/NoPUFile.root \
   -algo1 ak8pf \
   -algo2 ak8pf \
   -iftest false \
   -maxEvts 10000000 \
   -ApplyJEC false \
   -outputPath `pwd`/ \
   -npvRhoNpuBinWidth 10 \
   -NBinsNpvRhoNpu 6 \
   -MCPUReWeighting `pwd`/MyMCPileupHistogram.root \
   -DataPUReWeighting `pwd`/MyDataPileupHistogram.root \
   -useweight false \
   -nrefmax 3 \
   -doNotSave false \
   -forceNoPU false
jet_synchtree_x \
   -basepath '//' \
   -samplePU `pwd`/PUFile.root \
   -sampleNoPU `pwd`/NoPUFile.root \
   -algo1 ak8puppi \
   -algo2 ak8puppi \
   -iftest false \
   -maxEvts 10000000 \
   -ApplyJEC false \
   -outputPath `pwd`/ \
   -npvRhoNpuBinWidth 10 \
   -NBinsNpvRhoNpu 6 \
   -MCPUReWeighting `pwd`/MyMCPileupHistogram.root \
   -DataPUReWeighting `pwd`/MyDataPileupHistogram.root \
   -useweight false \
   -nrefmax 3 \
   -doNotSave false \
   -forceNoPU false

mv output_ak4pfchs.root   ${Output}/Result_AK4PFCHS_${ID}.root
mv output_ak4pf.root      ${Output}/Result_AK4PF_${ID}.root
mv output_ak4puppi.root   ${Output}/Result_AK4Puppi_${ID}.root
mv output_ak8pfchs.root   ${Output}/Result_AK8PFCHS_${ID}.root
mv output_ak8pf.root      ${Output}/Result_AK8PF_${ID}.root
mv output_ak8puppi.root   ${Output}/Result_AK8Puppi_${ID}.root

rm *.root
rm *.cc
