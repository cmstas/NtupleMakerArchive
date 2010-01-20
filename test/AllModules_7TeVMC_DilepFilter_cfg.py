import FWCore.ParameterSet.Config as cms

process = cms.Process("CMS2")

from Configuration.EventContent.EventContent_cff import *

process.configurationMetadata = cms.untracked.PSet(
        version = cms.untracked.string('$Revision: 1.1.2.3 $'),
        annotation = cms.untracked.string('CMS2'),
        name = cms.untracked.string('CMS2 test configuration')
)

# load event level configurations
process.load("Configuration.StandardSequences.Services_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = "MC_31X_V3::All"

process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
)

import sys
import os
import string


process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = ''
process.MessageLogger.cerr.FwkReport.reportEvery = 1



#-------------------------------------------------
# PAT configuration
#-------------------------------------------------

process.load("PhysicsTools.PatAlgos.patSequences_cff")
process.patDefaultSequence = cms.Sequence(
    process.allLayer1Objects *
    process.selectedLayer1Objects
)


#change JetID tag
from PhysicsTools.PatAlgos.tools.jetTools import *
addJetID( process, cms.InputTag('prunedUncorrectedCMS2Jets'), "antikt5" )
switchJetCollection(process, 
                    cms.InputTag('prunedUncorrectedCMS2Jets'),   
                    doJTA            = True,            
                    doBTagging       = True,            
                    jetCorrLabel     = ('AK5','Calo'),  
                    doType1MET       = True,
                    genJetCollection = cms.InputTag("cms2antikt5GenJets"),
                    doJetID          = True,
                    jetIdLabel       = "antikt5"
                    )

# add statement to prevent the PAT from using generator information
from PhysicsTools.PatAlgos.tools.coreTools import *
#uncomment for data
#removeMCMatching(process, 'All')

#-----------------------------------------------------------
# configure input data files and number of event to process
#-----------------------------------------------------------

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)
process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
)

process.source = cms.Source("PoolSource",
    skipEvents = cms.untracked.uint32(0),
    fileNames = cms.untracked.vstring(
    '/store/mc/Summer09/TTbar/GEN-SIM-RECO/MC_31X_V3_7TeV-v1/0005/42500546-EB9C-DE11-95C3-00163E112471.root'
    ),
)


process.EventSelectionDilFilt = cms.PSet(
    SelectEvents = cms.untracked.PSet(
    SelectEvents = cms.vstring('pWithHyps', 'pWithGenDil')
                )
        )
process.outHypOrGenDilFilt_CMS2 = cms.OutputModule(
        "PoolOutputModule",
        process.EventSelectionDilFilt,
        verbose = cms.untracked.bool(True),
        dropMetaData = cms.untracked.string("NONE"),
        fileName = cms.untracked.string('ntuple.root')
)

process.outHypOrGenDilFilt_CMS2.outputCommands = cms.untracked.vstring( 'drop *' )
process.outHypOrGenDilFilt_CMS2.outputCommands.extend(cms.untracked.vstring('keep *_*Maker*_*_CMS2*'))

# load event level configurations
process.load("CMS2.NtupleMaker.cms2CoreSequences_cff")
process.load("CMS2.NtupleMaker.cms2GENSequence_cff")
process.load("CMS2.NtupleMaker.cms2PATSequence_cff")


# loosen thresholds on collections
process.hypDilepMaker.TightLepton_PtCut=cms.double(20)
process.hypDilepMaker.LooseLepton_PtCut=cms.double(10)

process.prunedUncorrectedCMS2Jets.inputUncorrectedJetCollection = cms.InputTag("antikt5CaloJets")
process.pfJetMaker.pfJetsInputTag = cms.InputTag("antikt5PFJets")
process.load("JetMETCorrections.Configuration.L2L3Corrections_Summer09_7TeV_cff")
process.jetMaker.correctionTags   = cms.string("Summer09_7TeV_L2Relative_AK5Calo:Summer09_7TeV_L3Absolute_AK5Calo")
process.scjetMaker.correctionTags   = cms.string("Summer09_7TeV_L2Relative_SC5Calo:Summer09_7TeV_L3Absolute_SC5Calo")
switchJECSet(process,"Summer09_7TeV")

# don't forget 8e29
process.hltMakerSequence += process.hlt8e29Maker
process.l1Maker.fillL1Particles = cms.untracked.bool(False)
##
process.load('CMS2.NtupleMaker.pixelDigiMaker_cfi')
process.load('CMS2.NtupleMaker.beamHaloSequence_cff')

#-------------------------------------------------
# process paths;
#-------------------------------------------------
process.cms2WithEverything             = cms.Sequence( process.coreCMS2Sequence
                                                       * process.cms2GENSequence
                                                       * process.cms2beamHaloSequence
                                                       * process.pixelDigiMaker
                                                       * process.patDefaultSequence * process.cms2PATSequence)

#since filtering is done in the last step, there is no reason to remove these paths
#just comment out/remove an output which is not needed
process.load('CMS2.NtupleMaker.hypFilter_cfi')
process.pWithHyps       = cms.Path(process.cms2WithEverything * process.hypFilter     )
process.pWithGenDil     = cms.Path(process.cms2WithEverything * process.dilepGenFilter)

process.outpath         = cms.EndPath(process.outHypOrGenDilFilt_CMS2)
