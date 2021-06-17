

default: TestRun

TestRun: Execute
	time ./Execute --PTMin 30 --PTMax 6500 --CopyMode true \
		--Input SynchTree/Result_AK4PFCHS.root --Fraction 1.0 \
		--JEC Summer19UL18_L1Fastjet_V2_Summer19UL18_V2_L1FastJet_AK4PFchs.txt \
		--OutputJEC Correction.txt
	cp JEC.pdf ~/public/html/

Execute: PerformSimpleFit.cpp
	rm -f Execute
	CompileRootMacro PerformSimpleFit.cpp Execute
