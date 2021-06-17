Fraction = --Fraction 1.0
Plot     = --PlotPU false --PlotPT false --PlotPhi true

default: TestRunClosure

TestRunClosure: ExecuteClosure
	# ./ExecuteClosure --Input SynchTree/Result_AK4PFCHS.root \
	# 	--Output AK4PFCHS_Outside.pdf --OutputRoot AK4PFCHS_Outside.root \
	# 	--JEC Summer19UL18_L1Fastjet_V2_Summer19UL18_V2_L1FastJet_AK4PFchs.txt $(Fraction) $(Plot)
	# ./ExecuteClosure --Input SynchTree/Result_AK4PFCHS.root \
	# 	--Output AK4PFCHS_NoJEC.pdf --OutputRoot AK4PFCHS_NoJEC.root \
	# 	--JEC None $(Fraction) $(Plot)
	./ExecuteClosure --Input SynchTree/Result_AK4PFCHS.root \
		--Output AK4PFCHS_HEML1.pdf --OutputRoot AK4PFCHS_HEML1.root \
		--JEC CandidateHEML1.txt $(Fraction) $(Plot)
	cp AK4PFCHS*pdf ~/public/html

ExecuteClosure: PlotClosure.cpp
	CompileRootMacro PlotClosure.cpp ExecuteClosure
