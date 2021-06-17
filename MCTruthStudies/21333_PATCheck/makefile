V = Summer16_23Sep2016V4_MC
# V = Summer16_07Aug2017_V11_MC
A = AK4PFchs

default: TestRun

TestRun: Execute
	./Execute --PT 64.6744 --Eta 0.465376 --Phi 1.39014 --Area 0.528585 --Rho 12.1387 --Uncorr 0.980878 \
		--JEC textFiles/$(V)/$(V)_L1FastJet_$(A).txt,textFiles/$(V)/$(V)_L2Relative_$(A).txt,textFiles/$(V)/$(V)_L3Absolute_$(A).txt,textFiles/$(V)/$(V)_L2L3Residual_$(A).txt
	./Execute --PT 51.3144 --Eta 0.949494 --Phi 2.64906 --Area 0.538559 --Rho 12.1387 --Uncorr 0.974929 \
		--JEC textFiles/$(V)/$(V)_L1FastJet_$(A).txt,textFiles/$(V)/$(V)_L2Relative_$(A).txt,textFiles/$(V)/$(V)_L3Absolute_$(A).txt,textFiles/$(V)/$(V)_L2L3Residual_$(A).txt

Execute: CheckJEC.cpp
	CompileRootMacro CheckJEC.cpp Execute
