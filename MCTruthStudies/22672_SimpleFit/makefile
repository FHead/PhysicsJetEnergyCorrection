

default: TestRun

TestRun: Execute
	time ./Execute --EtaMin 0.000 --EtaMax 0.087 --PTMin 8 --PTMax 6500 \
		--Input SynchTree/Result_AK4PFCHS.root --Fraction 1 \
		--JEC Summer19UL18_L1Fastjet_V2_Summer19UL18_V2_L1FastJet_AK4PFchs.txt
	cp JEC.pdf ~/public/html/

Execute: PerformSimpleFit.cpp
	CompileRootMacro PerformSimpleFit.cpp Execute
