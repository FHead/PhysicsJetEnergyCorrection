import demjson
import sys
import re

inputfile = 'Fragments/Autumn18_RunC_V8_Data.txt'
if len(sys.argv) > 1:
    inputfile = sys.argv[1]

print "Processing file " + inputfile

f = open(inputfile)
data = demjson.decode('{'+f.read()+'}')

for version in data:
    for algorithm in data[version]:
        FileName = 'Converted/JSON_' + version + '_' + algorithm + '.json'

        alldata = []

        for level in data[version][algorithm]:
            for curve in data[version][algorithm][level]:

                curvelist = curve.split('_')

                item = {}
                item["Version"] = version
                item["Algorithm"] = algorithm
                item["Level"] = level
                item["Dependency"] = curvelist[0]
                item["Selection"] = re.sub(r'([A-Za-z]+)', r'\1 = ', ", ".join(curvelist[1:]))
                if item["Selection"] == '':
                    item["Selection"] = 'None'

                if level == 'JERSF':
                    item["Mode"] = "JERSF"
                elif curvelist[0] == 'Eta':
                    item["Mode"] = "Line"
                else:
                    item["Mode"] = "Line"

                if version.startswith('PFCalibration'):
                    item["YLabel"] = "Calibration"
                elif version.startswith('JER'):
                    if level == "JERSF":
                        item["YLabel"] = "Resolution Scale Factor"
                    else:
                        item["YLabel"] = "Resolution"
                else:   # Standard JEC
                    if level == "L1RC":
                        item["YLabel"] = "Random Cone"
                    elif level == "Uncertainty":
                        item["YLabel"] = "Uncertainty"
                    else:
                        item["YLabel"] = "Correction"

                item["X"] = []
                item["Y"] = []
                item["YPlus"] = []
                item["YMinus"] = []

                for points in data[version][algorithm][level][curve]['Data']:
                    item["X"].append(points[0])
                    item["Y"].append(points[1])
                    if len(points) == 4:
                        item["YPlus"].append(points[2])
                        item["YMinus"].append(points[3])

                if len(item["YPlus"]) == 0:
                    del item["YPlus"]
                if len(item["YMinus"]) == 0:
                    del item["YMinus"]

                alldata.append(item)

        with open(FileName, 'w') as outputfile:
            outputfile.write(demjson.encode(alldata))

