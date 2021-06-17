

default: TestRun

TestRun: Execute
	./Execute --JEC1 Summer19UL18_V2_MC_L2Relative_AK4PFchs.txt \
		--JEC2 Interpolated_L2Relative_AK4PFchs.txt --Output JECCheckRhoOnly.pdf
	cp *pdf ~/public/html/

Execute: RandomCheck.cpp
	CompileRootMacro RandomCheck.cpp Execute


