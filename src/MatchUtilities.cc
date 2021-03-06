#include "CMS3/NtupleMaker/interface/MatchUtilities.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "FWCore/Utilities/interface/Exception.h"

typedef math::XYZTLorentzVectorF LorentzVector;

MatchUtilities::MatchUtilities() {
}

MatchUtilities::~MatchUtilities() {
}



//----------------------------------------------------------------------------------------------
const reco::GenParticle* MatchUtilities::matchCandToGen(const reco::Candidate& cand, 
							const std::vector<reco::GenParticle>* genParticles, 
							int& genidx, int status, const std::vector<int> v_PIDsToExclude) {

  const reco::GenParticle* output = 0;
  double dRmin = 0.2;
  unsigned int i = 0;
  genidx = -9999;
  
  std::vector<reco::GenParticle>::const_iterator itPartEnd = genParticles->end();
  for(std::vector<reco::GenParticle>::const_iterator itPart=genParticles->begin(); itPart!=itPartEnd; ++itPart, ++i) {

    if ( status != 999 && itPart->status() != status ) continue;
    if ( find(v_PIDsToExclude.begin(), v_PIDsToExclude.end(), abs(itPart->pdgId()) ) != v_PIDsToExclude.end() ) 
      continue;

    const math::XYZVector v1(itPart->momentum().x(), itPart->momentum().y(), itPart->momentum().z());

    double dR = ROOT::Math::VectorUtil::DeltaR(v1,cand.p4());
    if (dR < dRmin) {
      dRmin = dR;
      output = &(*itPart);
      genidx = i;
    }
    
  }

  return output;
}

//----------------------------------------------------------------------------------------------
const reco::GenParticle* MatchUtilities::matchCandToGen(const reco::Track& track, 
							const std::vector<reco::GenParticle>* genParticles, 
							int& genidx, int status, const std::vector<int> v_PIDsToExclude) {

  const reco::GenParticle* output = 0;
  double dRmin = 0.2;
  unsigned int i = 0;
  genidx = -9999;
  
  std::vector<reco::GenParticle>::const_iterator itPartEnd = genParticles->end();
  for(std::vector<reco::GenParticle>::const_iterator itPart=genParticles->begin(); 
      itPart!=itPartEnd; ++itPart, ++i) {

    if ( status != 999 && itPart->status() != status ) continue;
    if ( find(v_PIDsToExclude.begin(), v_PIDsToExclude.end(), abs(itPart->pdgId()) ) != v_PIDsToExclude.end() ) 
      continue;

    const math::XYZVector v1(itPart->momentum().x(), itPart->momentum().y(), itPart->momentum().z());

    LorentzVector cand( track.px(), track.py(), track.pz(), track.p() );

    double dR = ROOT::Math::VectorUtil::DeltaR(v1,cand);

    if (dR < dRmin) {
      dRmin = dR;
      output = &(*itPart);
      genidx = i;
    }//END find minimum delta R loop
  
  }//END loop over genParticles

  return output;
}


//----------------------------------------------------------------------------------------------
const reco::GenParticle* MatchUtilities::matchCandToGen(const LorentzVector& candp4, 
							const std::vector<reco::GenParticle>* genParticles, 
							int& genidx, int status, const std::vector<int> v_PIDsToExclude) {

  const reco::GenParticle* output = 0;
  double dRmin = 0.2;
  unsigned int i = 0;
  genidx = -9999;
  
  std::vector<reco::GenParticle>::const_iterator itPartEnd = genParticles->end();
  for(std::vector<reco::GenParticle>::const_iterator itPart=genParticles->begin(); itPart!=itPartEnd; ++itPart, ++i) {

    if ( status != 999 && itPart->status() != status ) continue;
    if ( find(v_PIDsToExclude.begin(), v_PIDsToExclude.end(), abs(itPart->pdgId()) ) != v_PIDsToExclude.end() ) 
      continue;

    const math::XYZVector v1(itPart->momentum().x(), itPart->momentum().y(), itPart->momentum().z());

    double dR = ROOT::Math::VectorUtil::DeltaR(v1,candp4);

    if (dR < dRmin) {
      dRmin = dR;
      output = &(*itPart);
      genidx = i;
    }//END find minimum delta R loop
  
  }//END loop over genParticles

  return output;
}
 
//----------------------------------------------------------------------------------------------
const pat::PackedGenParticle* MatchUtilities::matchCandToGen(const LorentzVector& candp4, 
							const std::vector<pat::PackedGenParticle>* genParticles, 
							int& genidx, int status, const std::vector<int> v_PIDsToExclude) {

  const pat::PackedGenParticle* output = 0;
  double dRmin = 0.2;
  unsigned int i = 0;
  genidx = -9999;
  
  std::vector<pat::PackedGenParticle>::const_iterator itPartEnd = genParticles->end();
  for(std::vector<pat::PackedGenParticle>::const_iterator itPart=genParticles->begin(); itPart!=itPartEnd; ++itPart, ++i) {

    if ( status != 999 && itPart->status() != status ) continue;
    if ( find(v_PIDsToExclude.begin(), v_PIDsToExclude.end(), abs(itPart->pdgId()) ) != v_PIDsToExclude.end() ) 
      continue;

    const math::XYZVector v1(itPart->momentum().x(), itPart->momentum().y(), itPart->momentum().z());

    double dR = ROOT::Math::VectorUtil::DeltaR(v1,candp4);

    if (dR < dRmin) {
      dRmin = dR;
      output = &(*itPart);
      genidx = i;
    }//END find minimum delta R loop
  
  }//END loop over genParticles

  return output;
}

//----------------------------------------------------------------------------------------------
const reco::Candidate* MatchUtilities::matchGenToCand(const reco::GenJet& genJet,
						      std::vector<const reco::Candidate*> cand) {

  const reco::Candidate* output = 0;
  double dRmin = 0.2;

  std::vector<const reco::Candidate*>::const_iterator itCand;
  
  for(itCand=cand.begin(); itCand!=cand.end(); ++itCand) {

    const math::XYZVector v1(genJet.momentum().x(), genJet.momentum().y(), genJet.momentum().z());

    double dR = ROOT::Math::VectorUtil::DeltaR(v1, (*itCand)->p4());
    if (dR < dRmin) {
      dRmin = dR;
      output = *itCand;
    }
  }

  return output;
}


//----------------------------------------------------------------------------------------------
const reco::GenJet* MatchUtilities::matchCandToGenJet(const reco::Candidate& jet, 
						      const std::vector<reco::GenJet>* genJets) { 
  
  const reco::GenJet* output = 0;
  double dRmin = 0.3;
  
  std::vector<reco::GenJet>::const_iterator itJetEnd = genJets->end();
  for(std::vector<reco::GenJet>::const_iterator itJet=genJets->begin(); itJet!=itJetEnd; ++itJet) {

    const math::XYZVector v1(itJet->momentum().x(), itJet->momentum().y(), itJet->momentum().z());

    double dR = ROOT::Math::VectorUtil::DeltaR(v1, jet.p4());
    if (dR < dRmin) {
      dRmin = dR;
      output = &(*itJet);
    }
  }

  return output;
}
//----------------------------------------------------------------------------------------------
const reco::GenJet* MatchUtilities::matchCandToGenJet(const LorentzVector& jetp4, 
						      const std::vector<reco::GenJet>* genJets,
						      int &genidx) { 
  
  const reco::GenJet* output = 0;
  double dRmin = 0.3;
  int i = 0;
  genidx = -9999;
  
  std::vector<reco::GenJet>::const_iterator itJetEnd = genJets->end();
  for(std::vector<reco::GenJet>::const_iterator itJet=genJets->begin(); itJet!=itJetEnd; ++itJet, ++i) {

    const math::XYZVector v1(itJet->momentum().x(), itJet->momentum().y(), itJet->momentum().z());

    double dR = ROOT::Math::VectorUtil::DeltaR(v1, jetp4);
    if (dR < dRmin) {
      dRmin = dR;
      output = &(*itJet);
      genidx = i;
    }
  }

  return output;
}



//----------------------------------------------------------------------------------------------
const reco::Candidate* MatchUtilities::matchGenToCand(const reco::GenParticle& p, 
						      std::vector<const reco::Candidate*> cand) {

  const reco::Candidate* output = 0;
  double dRmin = 0.2;

  std::vector<const reco::Candidate*>::const_iterator itCand;
  
  for(itCand=cand.begin(); itCand!=cand.end(); ++itCand) {

    const math::XYZVector v1(p.momentum().x(), p.momentum().y(), p.momentum().z());

    double dR = ROOT::Math::VectorUtil::DeltaR(v1, (*itCand)->p4());
    if (dR < dRmin) {
      dRmin = dR;
      output = *itCand;
    }
  }

  return output;
}
//----------------------------------------------------------------------------------------------

const int MatchUtilities::getMatchedGenIndex(const reco::GenParticle& p, const std::vector<reco::GenParticle>* genParticles, 
					     int status, const std::vector<int> v_PIDsToExclude) {

  double dRmin = 0.2; 
  std::vector<reco::GenParticle>::const_iterator itCand;
  int idx = -9999;
  int temp = 0;
  math::XYZVector v1(p.momentum().x(), p.momentum().y(), p.momentum().z());
  for(itCand = genParticles->begin(); itCand != genParticles->end(); itCand++, temp++) {
    
    if(status != 999 && itCand->status() != status)
      continue;
    if ( find(v_PIDsToExclude.begin(), v_PIDsToExclude.end(), abs(itCand->pdgId()) ) != v_PIDsToExclude.end() ) 
      continue;

    double dR = ROOT::Math::VectorUtil::DeltaR(v1, itCand->p4());
    
    if(dR < dRmin) {
      idx = temp;
      dRmin = dR;
    }
  }

  return idx;
}


//----------------------------------------------------------------------------------------------

const void MatchUtilities::alignRecoPatJetCollections(const std::vector<reco::CaloJet>& v_ref,
						      std::vector<pat::Jet>& v_toAllign) {
  alignCollections(v_ref, v_toAllign);
}
//----------------------------------------------------------------------------------------------
const void MatchUtilities::alignRecoPatElectronCollections(const std::vector<reco::GsfElectron>& v_ref,
							   std::vector<pat::Electron>& v_toAllign) {
  alignCollections(v_ref, v_toAllign);
}
//----------------------------------------------------------------------------------------------
const void MatchUtilities::alignRecoPatMuonCollections(const std::vector<reco::Muon>& v_ref,
						       std::vector<pat::Muon>& v_toAllign) {
  alignCollections(v_ref, v_toAllign);
}
//----------------------------------------------------------------------------------------------
const void MatchUtilities::alignJPTcaloJetCollections(const std::vector<reco::CaloJet>& v_ref,
						      std::vector<reco::CaloJet>& v_toAllign) {
  alignCollections(v_ref, v_toAllign);
}

  
