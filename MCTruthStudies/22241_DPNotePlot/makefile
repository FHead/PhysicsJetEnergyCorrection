default: TestRun

Execute: MakePlot.cpp
	CompileRootMacro MakePlot.cpp Execute

TestRun: Execute
	./Execute --l1 Autumn18_V19_MC_L1FastJet_AK4PFchs.txt \
		--l2l3 Autumn18_V19_MC_L2Relative_AK4PFchs.txt \
		--output TestRun2018.pdf --year 2018
	./Execute --l1 Fall17_17Nov2017_V32_MC_L1FastJet_AK4PFchs.txt \
		--l2l3 Fall17_17Nov2017_V32_MC_L2Relative_AK4PFchs.txt \
		--output TestRun2017.pdf --year 2017
	./Execute --l1 Summer16_07Aug2017_V11_MC_L1FastJet_AK4PFchs.txt \
		--l2l3 Summer16_07Aug2017_V11_MC_L2Relative_AK4PFchs.txt \
		--output TestRun2016.pdf --year 2016
	cp TestRun*.pdf ~/public/html/
