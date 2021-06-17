default: RunCheck


Execute: PlotJEC.cpp
	CompileRootMacro PlotJEC.cpp Execute

RunCheck: Execute
	./Execute --jecl1 "Trivial.txt","Trivial.txt" --jecl2l3 "JEC.txt","JEC2.txt" --label "L2Relative","L2Residual" --output Meow.pdf --offset false

