// -*- C++ -*-
//
// Package:    PFJetMaker
// Class:      PFJetMaker
// 
/**\class PFJetMaker PFJetMaker.cc temp/PFJetMaker/src/PFJetMaker.cc

   Description: <one line class summary>

   Implementation:
   <Notes on implementation>
*/
//
// Original Author:  Puneeth Devanand KALAVASE
//         Created:  Tue Sep  1 22:18:18 CEST 2009
// $Id: PFJetMaker.h,v 1.5 2010/03/03 04:20:20 kalavase Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"


//
// class decleration
//

class PFJetMaker : public edm::EDProducer {
public:
     explicit PFJetMaker(const edm::ParameterSet&);
     ~PFJetMaker();

private:
     virtual void beginJob() ;
     virtual void produce(edm::Event&, const edm::EventSetup&);
     virtual void endJob() ;
      
     // ----------member data ---------------------------
     edm::InputTag pfJetsInputTag_;
     double         pfJetPtCut_;
     std::string nameL2L3JetCorrector_;
	std::string aliasprefix_;
};