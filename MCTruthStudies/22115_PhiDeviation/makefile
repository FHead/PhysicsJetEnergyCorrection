default: TestRun

TestRun: Execute
	./Execute \
		--input `ls Samples/* | head -n50 | tr '\n' ',' | sed 's/,$$/\n/'` \
		--etamin -3.0 --etamax -1.5 \
		--output CheckPhi.pdf
	cp CheckPhi.pdf ~/public/html

FullRun: Execute
	./Execute \
		--input `ls Samples/* | head -n500 | tr '\n' ',' | sed 's/,$$/\n/'` \
		--etamin -3.0 --etamax -2.5 \
		--output CheckPhi3.pdf
	./Execute \
		--input `ls Samples/* | head -n500 | tr '\n' ',' | sed 's/,$$/\n/'` \
		--etamin -2.5 --etamax -2.0 \
		--output CheckPhi2.pdf
	./Execute \
		--input `ls Samples/* | head -n500 | tr '\n' ',' | sed 's/,$$/\n/'` \
		--etamin -2.0 --etamax -1.5 \
		--output CheckPhi1.pdf
	cp CheckPhi[123].pdf ~/public/html

Execute: PhiDeviation.cpp
	CompileRootMacro PhiDeviation.cpp Execute
