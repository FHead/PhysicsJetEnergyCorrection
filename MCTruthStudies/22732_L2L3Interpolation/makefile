default: TestRunRho TestRunFull

TestRunRho: Execute
	./Execute --OutsideFileName Summer19UL18_V2_MC_L2Relative_AK4PFchs.txt \
		--InsideFileName ParallelMCHEM_L2Relative_AK4PFchs.txt \
		--Output Interpolated_L2Relative_AK4PFchs.txt \
		--PhiMin -1.5708 --PhiMax -0.8727 --Shift 0.15 \
		--HEMPhiMin -1.57079632679 --HEMPhiMax -0.87266462599 --HEMShift 0.1 \
		--MinEta -3.0 --MaxEta -1.5 \
		--R0 0.25 --N 1000000 --PTClip 10

TestRunFull: Execute
	./Execute --OutsideFileName Summer19UL18_V2_MC_L2Relative_AK4PFchs.txt \
		--InsideFileName ParallelMCFullHEM_L2Relative_AK4PFchs.txt \
		--Output InterpolatedFull_L2Relative_AK4PFchs.txt \
		--PhiMin -1.5708 --PhiMax -0.8727 --Shift 0.15 \
		--HEMPhiMin -1.57079632679 --HEMPhiMax -0.87266462599 --HEMShift 0.1 \
		--MinEta -3.0 --MaxEta -1.5 \
		--R0 0.25 --N 1000000 --PTClip 10

Execute: CombineTextFiles.cpp
	CompileRootMacro CombineTextFiles.cpp Execute
