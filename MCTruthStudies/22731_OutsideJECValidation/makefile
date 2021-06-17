

default: TestRun

TestRun: Execute
	./Execute --JEC1 Summer19UL18_L1Fastjet_V2_Summer19UL18_V2_L1FastJet_AK4PFchs.txt \
		--JEC2 Correction.txt --Output JECCheckRhoOnly.pdf
	./Execute --JEC1 Summer19UL18_L1Fastjet_V2_Summer19UL18_V2_L1FastJet_AK4PFchs.txt \
		--JEC2 FullCorrection.txt --Output JECCheckFull.pdf
	cp *pdf ~/public/html/

Execute: RandomCheck.cpp
	CompileRootMacro RandomCheck.cpp Execute


