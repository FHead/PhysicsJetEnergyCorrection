#! /usr/bin/env python

import argparse
import json

parser = argparse.ArgumentParser()
parser.add_argument("--Input",  required = True, nargs = '+', default = [])
parser.add_argument("--Output", required = True)

args = vars(parser.parse_args())

print(args)

AllData = []
for file in args["Input"]:
    with open(file) as handle:
        Data = json.load(handle)
        AllData.append(Data)

with open(args["Output"], 'w') as handle:
    handle.write(json.dumps(AllData))


