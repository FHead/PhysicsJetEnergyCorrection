
default: RunExample # RunSingleData

RunExample:
	bash RunExample.sh

RunSingleData:
	# bash MakeSingleData.sh jsonpog-integration/POG/JME/2018_UL/jet_jerc.json.gz Summer19UL18_V5_MC
	# bash MakeSingleData.sh jsonpog-integration/POG/JME/2018_UL/jet_jerc.json.gz Summer19UL18_RunA_V5_DATA
	# bash MakeSingleData.sh jsonpog-integration/POG/JME/2018_UL/jet_jerc.json.gz Summer19UL18_RunB_V5_DATA
	# bash MakeSingleData.sh jsonpog-integration/POG/JME/2018_UL/jet_jerc.json.gz Summer19UL18_RunC_V5_DATA
	bash MakeSingleData.sh jsonpog-integration/POG/JME/2018_UL/jet_jerc.json.gz Summer19UL18_RunD_V5_DATA

ListLevels:
	# zgrep -o 'Summer19UL18_V5_MC_[A-Za-z0-9]*_AK4PFchs' jsonpog-integration/POG/JME/2018_UL/jet_jerc.json.gz
	zgrep -o 'Summer19UL18_[A-Za-z0-9_]*_AK4PFchs' jsonpog-integration/POG/JME/2018_UL/jet_jerc.json.gz
