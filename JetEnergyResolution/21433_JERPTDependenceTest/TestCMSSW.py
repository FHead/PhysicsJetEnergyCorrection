from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing()

import FWCore.ParameterSet.Config as cms
process = cms.Process('JERSF')

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1))
process.source = cms.Source('EmptySource')

process.test = cms.EDAnalyzer("TestJERSF", textfile = cms.untracked.string('JERSFs_Zjets_Dijets_Combined_Func3_and_Rest_RunACB.txt'))

process.p = cms.Path(process.test)
