#! /usr/bin/env python

import correctionlib._core as core
import argparse
import json
import math
import time

def boolean_string(s):
    if s not in {'False', 'True', 'false', 'true'}:
        raise ValueError('Not a valid boolean string')
    return s == 'True' or s == 'true'

parser = argparse.ArgumentParser()
parser.add_argument("--Input",      required = True,   type = str)
parser.add_argument("--OutputBase", required = True,   type = str)
parser.add_argument("--Prefix",     default = "",      type = str)
parser.add_argument("--Version",    required = True,   type = str)
parser.add_argument("--Algorithm",  required = True,   type = str)
parser.add_argument("--Level",      required = True,   type = str)
parser.add_argument("--LevelTag",                      type = str)
parser.add_argument("--IsCompound", default = False,   type = boolean_string)
parser.add_argument("--Dependent",  default = "PT",    type = str)
parser.add_argument("--JER")
parser.add_argument("--DoOldEta")
parser.add_argument("--Min",        default = 10,      type = float)
parser.add_argument("--Max",        default = 100,     type = float)
parser.add_argument("--NBin",       default = 1000,    type = int)
parser.add_argument("--FixRho",     default = 20,      type = float)
parser.add_argument("--FixPT",      default = 100,     type = float)
parser.add_argument("--FixEta",     default = 0,       type = float)
parser.add_argument("--FixPhi",                        type = float)
parser.add_argument("--FixArea",    default = 0.50265, type = float)

args = vars(parser.parse_args())
if args["LevelTag"] == None:
    args["LevelTag"] = args["Level"]

print(args)

#################
# Construct tag #
#################

Tag = ""
if args["Dependent"] != "Rho":
    if Tag != "":
        Tag = Tag + ", "
    Tag = Tag + "Rho = %.02f" % args["FixRho"]
if args["Dependent"] != "PT":
    if Tag != "":
        Tag = Tag + ", "
    Tag = Tag + "PT = %.02f" % args["FixPT"]
if args["Dependent"] != "Eta":
    if Tag != "":
        Tag = Tag + ", "
    Tag = Tag + "Eta = %.02f" % args["FixEta"]
if args["Dependent"] != "Phi" and args["FixPhi"] != None:
    if Tag != "":
        Tag = Tag + ", "
    Tag = Tag + "Phi = %.02f" % args["FixPhi"]

print("Tag = \"%s\"" % Tag)


#####################
# Setup x locations #
#####################

X = [];

if args["Dependent"] == "Eta":
    EtaBinEdge = [];
    if args["JER"] != None:
        EtaBinEdge = [-5.191, -4.7, -3.2, -3.139, -3.0, -2.964, -2.853, -2.8, -2.65, -2.5, -2.322, -2.3, -2.1, -2.043, -1.93, -1.9, -1.74, -1.7, -1.305, -1.3, -1.131, -1.1, -0.8, -0.783, -0.522, -0.5, 0.0, 0.5, 0.522, 0.783, 0.8, 1.1, 1.131, 1.3, 1.305, 1.7, 1.74, 1.9, 1.93, 2.043, 2.1, 2.3, 2.322, 2.5, 2.65, 2.8, 2.853, 2.964, 3.0, 3.139, 3.2, 4.7, 5.191];
    elif args["LevelTag"] == "Uncertainty":
        EtaBinEdge = [-5.4, -5.0, -4.4, -4.0, -3.5, -3.0, -2.8, -2.6, -2.4, -2.2, -2.0, -1.8, -1.6, -1.4, -1.2, -1.0, -0.8, -0.6, -0.4, -0.2, 0.0, 0.2, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0, 3.5, 4.0, 4.4, 5.0, 5.4];
    elif args["DoOldEta"] == None:
        EtaBinEdge = [-5.191, -4.889, -4.716, -4.538, -4.363, -4.191, -4.013, -3.839, -3.664, -3.489, -3.314, -3.139, -2.964, -2.853, -2.65, -2.5, -2.322, -2.172, -2.043, -1.93, -1.83, -1.74, -1.653, -1.566, -1.479, -1.392, -1.305, -1.218, -1.131, -1.044, -0.957, -0.879, -0.783, -0.696, -0.609, -0.522, -0.435, -0.348, -0.261, -0.174, -0.087, 0, 0.087, 0.174, 0.261, 0.348, 0.435, 0.522, 0.609, 0.696, 0.783, 0.879, 0.957, 1.044, 1.131, 1.218, 1.305, 1.392, 1.479, 1.566, 1.653, 1.74, 1.83, 1.93, 2.043, 2.172, 2.322, 2.5, 2.65, 2.853, 2.964, 3.139, 3.314, 3.489, 3.664, 3.839, 4.013, 4.191, 4.363, 4.538, 4.716, 4.889, 5.191];
    else:
        EtaBinEdge = [-5, -4.9, -4.8, -4.7, -4.6, -4.5, -4.4, -4.3, -4.2, -4.1, -4.0, -3.9, -3.8, -3.7, -3.6, -3.5, -3.4, -3.3, -3.2, -3.1, -3.0, -2.9, -2.8, -2.7, -2.6, -2.5, -2.4, -2.3, -2.2, -2.1, -2.0, -1.9, -1.8, -1.7, -1.6, -1.5, -1.4, -1.3, -1.2, -1.1, -1.0, -0.9, -0.8, -0.7, -0.6, -0.5, -0.4, -0.3, -0.2, -0.1, 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3.0, 3.1, 3.2, 3.3, 3.4, 3.5, 3.6, 3.7, 3.8, 3.9, 4.0, 4.1, 4.2, 4.3, 4.4, 4.5, 4.6, 4.7, 4.8, 4.9, 5.0, -5.191, -4.889, -4.716, -4.538, -4.363, -4.191, -4.013, -3.839, -3.664, -3.489, -3.314, -3.139, -2.964, -2.853, -2.65, -2.51, -2.322, -2.172, -2.043, -1.93, -1.83, -1.74, -1.653, -1.566, -1.479, -1.392, -1.305, -1.218, -1.131, -1.044, -0.957, -0.879, -0.783, -0.696, -0.609, -0.522, -0.435, -0.348, -0.261, -0.174, -0.087, 0.087, 0.174, 0.261, 0.348, 0.435, 0.522, 0.609, 0.696, 0.783, 0.879, 0.957, 1.044, 1.131, 1.218, 1.305, 1.392, 1.479, 1.566, 1.653, 1.74, 1.83, 1.93, 2.043, 2.172, 2.322, 2.51, 2.65, 2.853, 2.964, 3.139, 3.314, 3.489, 3.664, 3.839, 4.013, 4.191, 4.363, 4.538, 4.716, 4.889, 5.191];
    EtaBinEdge.sort();
    for eta in EtaBinEdge:
        X.append(eta - 0.0001)
        X.append(eta + 0.0001)
else:
    Min = args["Min"]
    Max = args["Max"]
    NBin = args["NBin"]
    for i in range(NBin + 1):
        if args["Dependent"] == "PT":
            X.append(math.exp(math.log(Min) + (math.log(Max) - math.log(Min)) / NBin * i))
        else:
            X.append((Min + (Max - Min) / NBin * i))


###########################
# Setup correction object #
###########################

Corrector = core.CorrectionSet.from_file(args["Input"])
if args["IsCompound"] == True:
    Evaluator = Corrector.compound["%s_%s_%s" % (args["Version"], args["Level"], args["Algorithm"])]
else:
    Evaluator = Corrector["%s_%s_%s" % (args["Version"], args["Level"], args["Algorithm"])]
NArgs = len(Evaluator.inputs)

#####################
# Evaluate Y values #
#####################

Y = []
for x in X:
    Area = float(args["FixArea"])
    Eta  = float(args["FixEta"])
    PT   = float(args["FixPT"])
    Rho  = float(args["FixRho"])
    if args["FixPhi"] != None:
        Phi  = float(args["FixPhi"])
    else:
        Phi = 0.0

    if args["Dependent"] == "PT":
        PT = x
    if args["Dependent"] == "Eta":
        Eta = x
    if args["Dependent"] == "Phi":
        Phi = x
    if args["Dependent"] == "Rho":
        Rho = x
    if args["Dependent"] == "Area":
        Area = x

    Xs = []
    for i in range(NArgs):
        if Evaluator.inputs[i].name == 'JetPt':
            Xs.append(PT)
        elif Evaluator.inputs[i].name == 'JetEta':
            Xs.append(Eta)
        elif Evaluator.inputs[i].name == 'JetA':
            Xs.append(Area)
        elif Evaluator.inputs[i].name == 'Rho':
            Xs.append(Rho)
        else:   # Ask Yi to add new items above!
            print(Evaluator.inputs[i].name)

    Result = Evaluator.evaluate(*Xs);

    # if NArgs == 1:
    #     Result = Evaluator.evaluate(Xs[0])
    # elif NArgs == 2:
    #     Result = Evaluator.evaluate(Xs[0], Xs[1])
    # elif NArgs == 3:
    #     Result = Evaluator.evaluate(Xs[0], Xs[1], Xs[2])
    # elif NArgs == 4:
    #     Result = Evaluator.evaluate(Xs[0], Xs[1], Xs[2], Xs[3])
    # elif NArgs == 5:
    #     Result = Evaluator.evaluate(Xs[0], Xs[1], Xs[2], Xs[3], Xs[4])
    # elif NArgs == 6:
    #     Result = Evaluator.evaluate(Xs[0], Xs[1], Xs[2], Xs[3], Xs[4], Xs[5])
    # else:
    #     print('NArgs = %d!' % NArgs)

    if Result < -100:
        Result = -1

    Y.append(Result)


#################
# Write snippet #
#################

Output = {}
Output["Version"]    = args["Prefix"] + args["Version"]
Output["Algorithm"]  = args["Algorithm"]
Output["Level"]      = args["LevelTag"]
Output["Dependency"] = args["Dependent"]
Output["Selection"]  = Tag
Output["Mode"]       = "Line"

if args["Version"].startswith('PFCalibration'):
    Output["YLabel"] = "Calibration"
elif args["Version"].startswith("JER"):
    if args["Level"] == "JERSF":
        Output["YLabel"] = "Resolution Scale Factor"
    else:
        Output["YLabel"] = "Resolution"
else:   # Standard JEC
    if args["Level"] == "L1RC":
        Output["YLabel"] = "Random Cone"
    elif args["Level"] == "Uncertainty":
        Output["YLabel"] = "Uncertainty"
    else:
        Output["YLabel"] = "Correction"

Output["X"] = X
Output["Y"] = Y

# print(Output)

with open(args["OutputBase"] + str(time.time()), 'w') as outputfile:
    outputfile.write(json.dumps(Output))

