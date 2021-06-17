V = Summer16_23Sep2016V4_MC
# V = Summer16_07Aug2017_V11_MC
# V = Autumn18_V8_MC
# V = Fall17_17Nov2017_V32_MC
V = Summer16_07Aug2017GH_V11_DATA
A = AK4PFchs

default: DoRun

DoRun: Execute
	bash CheckAll.sh $(V) $(A) RawInput

Execute: CheckJEC.cpp
	CompileRootMacro CheckJEC.cpp Execute
