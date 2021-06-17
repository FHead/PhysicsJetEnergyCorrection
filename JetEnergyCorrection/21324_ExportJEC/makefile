default: TestRun

Execute: ExportCurve.cpp
	CompileRootMacro ExportCurve.cpp Execute

ExecuteUncertainty: ExportUncertainty.cpp
	CompileRootMacro ExportUncertainty.cpp ExecuteUncertainty

TestRun: Execute
	./Execute --Version Fall17_17Nov2017_V32_MC --Algorithm AK4PFchs --Level L1FastJet --Dependent Eta --Min -5.2 --Max 5.2

UpdateJEC: Execute ExecuteUncertainty
	bash MakeTestData.sh > JECViewerDevelopment/JECData.js

RunAllVersions: Execute ExecuteUncertainty
	bash RunAllVersions.sh

RunAllJERVersions: Execute ExecuteUncertainty
	bash RunAllJERVersions.sh

CombineAllData:
	bash CombineAllData.sh > JECViewerDevelopment/JECData.js
	ls -lh JECViewerDevelopment/JECData.js

CombineHIData:
	bash CombineHIData.sh > JECViewerDevelopment/JECData.js
	ls -lh JECViewerDevelopment/JECData.js

Deploy:
	cp JECViewerDevelopment/* ~/public/html/JECViewer/
	cp JECViewerDevelopment/* /eos/cms/store/group/phys_jetmet/www/JECViewer/

DeployTest:
	cp JECViewerDevelopment/* ~/public/html/JECViewerTest/

Recover:
	tail -n +2 JECViewerDevelopment/JECData.js \
		| sed "s/^\( [ ]*\)\([A-Za-z0-9_]*\):/\1\"\2\":/" \
		| sed "s/Data:/\"Data\":/" \
		| tr -d '\n' | tr -d ' ' \
		| sed "s/},}/}}/g" | sed "s/},}/}}/g" \
		| sed "s/],]/]]/g" > JECTemp.js
	python3 RecoverFragments.py
