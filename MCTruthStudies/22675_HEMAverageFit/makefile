default: TestRun

TestRun: Execute
	./Execute --Input AK4PFCHS_NoJEC.root --Output OOA_12.pdf --Bin 12
	./Execute --Input AK4PFCHS_NoJEC.root --Output OOA_14.pdf --Bin 14
	./Execute --Input AK4PFCHS_NoJEC.root --Output OOA_16.pdf --Bin 16
	./Execute --Input AK4PFCHS_NoJEC.root --Output OOA_18.pdf --Bin 18
	./Execute --Input AK4PFCHS_NoJEC.root --Output OOA_21.pdf --Bin 21
	cp *pdf ~/public/html/

Execute: MakePlot.cpp
	CompileRootMacro MakePlot.cpp Execute

