#!/bin/bash

python3 ExportCurve.py --Input jsonpog-integration/POG/JME/2018_UL/jet_jerc.json.gz \
   --Output Summer19UL18_V5_MC_AK4PFchs_00001.json \
   --Version Summer19UL18_V5_MC \
   --IsCompound false \
   --Algorithm AK4PFchs \
   --Level Total --LevelTag Uncertainty
