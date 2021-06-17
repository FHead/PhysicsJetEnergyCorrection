Fraction = 0.1

default: TestRun

TestRun: Execute
	time ./Execute --Input SynchTree/Result_AK4PFCHS.root --Fraction $(Fraction) --Output DefaultClosure.pdf \
		--JEC Summer19UL18_V2_MC_L1FastJet_AK4PFchs.txt,Summer19UL18_V2_MC_L2Relative_AK4PFchs.txt
	time ./Execute --Input SynchTree/Result_AK4PFCHS.root --Fraction $(Fraction) --Output L1FullClosure.pdf \
		--JEC L1FullCorrection.txt,Summer19UL18_V2_MC_L2Relative_AK4PFchs.txt
	time ./Execute --Input SynchTree/Result_AK4PFCHS.root --Fraction $(Fraction) --Output L1RhoClosure.pdf \
		--JEC L1RhoCorrection.txt,Summer19UL18_V2_MC_L2Relative_AK4PFchs.txt
	time ./Execute --Input SynchTree/Result_AK4PFCHS.root --Fraction $(Fraction) --Output L1L2L3FullClosure.pdf \
		--JEC L1FullCorrection.txt,InterpolatedFull_L2Relative_AK4PFchs.txt
	time ./Execute --Input SynchTree/Result_AK4PFCHS.root --Fraction $(Fraction) --Output L1L2L3RhoClosure.pdf \
		--JEC L1RhoCorrection.txt,Interpolated_L2Relative_AK4PFchs.txt
	cp *.pdf ~/public/html/
	
Debug: Execute
	time ./Execute --Input SynchTree/Result_AK4PFCHS.root --Fraction $(Fraction) --Output L1L2L3FullClosure.pdf \
		--JEC Interpolated_L2Relative_AK4PFchs.txt

Execute: PlotClosure.cpp
	CompileRootMacro PlotClosure.cpp Execute


