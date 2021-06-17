default: TestRun

TestRun: Execute
	./Execute --FileName1 Gjet_combinationfile/Lucas/Gjet_combinationfile_2019-03-01_A.root \
		--FileName2 Gjet_combinationfile/Tribeni/Gjet_combinationfile_2018A_A.root \
		--OutputFileName RunA.pdf
	cp RunA.pdf ~/public/html/

Execute: PlotComparison.cpp
	CompileRootMacro PlotComparison.cpp Execute
