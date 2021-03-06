// -*- C++ -*-
//
// Package:    NtupleMaker
// Class:      HypDilepMaker
// 
/**\class HypDilepMaker HypDilepMaker.h CMS3/NtupleMaker/interface/HypDilepMaker.h

Description: create trilepton hypothesis branches

Implementation:
- combine muons and electrons after preselection
- correct jets and store index vectors
- correct met

*/
//
// Original Author:  Puneeth Kalavase
//         Created:  Wed Jun 18 19:59:33 UTC 2008  
// $Id: HypDilepMaker.h,v 1.13 2010/06/15 10:08:36 fgolf Exp $
//
//
#ifndef CMS2_HYPDILEPMAKER_H
#define CMS2_HYPDILEPMAKER_H

// system include files
#include <memory>
#include <string>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/Math/interface/LorentzVector.h"

//
// class decleration
//

class HypDilepMaker : public edm::EDProducer {
public:
  
    

  explicit HypDilepMaker (const edm::ParameterSet&);
  ~HypDilepMaker();
  
private:
  virtual void beginJob() ;
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  bool testJetForLeptons(const math::XYZTLorentzVectorF& jetP4, const math::XYZTLorentzVectorF& lepP4);
  double mT2_bisect(const math::XYZTLorentzVectorF lep1_p4, const math::XYZTLorentzVectorF lep2_p4,
		    const double met, const double metPhi);
   
  // ----------member data ---------------------------
  edm::InputTag muonsInputTag;
  edm::InputTag muToGenInputTag;
  edm::InputTag electronsInputTag;
  edm::InputTag metInputTag;
  edm::InputTag jetsInputTag;
  double        hypJetMaxEtaCut;
  double        hypJetMinPtCut;
  double        tightptcut;
  double        looseptcut;
    
  std::string aliasprefix_;
};


#endif
