// -*- C++ -*-
//
// Package:    RecoConversionMaker
// Class:      RecoConversionMaker
// 
/**\class RecoConversionMaker RecoConversionMaker.cc CMS2/RecoConversionMaker/src/RecoConversionMaker.cc

   Description: <one line class summary>

   Implementation:
   <Notes on implementation>
*/
//
// Original Author:  pts/4
//         Created:  Fri Jun  6 11:07:38 CDT 2008
// $Id: RecoConversionMaker.cc,v 1.4 2011/04/28 00:23:38 dbarge Exp $
//
//

// system include files
#include <memory>
#include "Math/VectorUtil.h"

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CMS3/NtupleMaker/interface/RecoConversionMaker.h"

#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "CommonTools/Statistics/interface/ChiSquaredProbability.h"
// Propagator specific include files

using namespace std;
using namespace reco;
using namespace edm;
typedef math::XYZTLorentzVectorF LorentzVector;
typedef math::XYZPointF Point;


//
// class decleration
//

//
// constructors and destructor
//
RecoConversionMaker::RecoConversionMaker(const edm::ParameterSet& iConfig) {
       
  recoConversionInputTag_ = iConfig.getParameter<edm::InputTag>("recoConversionInputTag");
  beamSpotInputTag_       = iConfig.getParameter<edm::InputTag>("beamSpotInputTag");


  produces<vector<int> >		("convsisConverted"	).setBranchAlias("convs_isConverted"	);
  produces<vector<int> >                ("convsquality"         ).setBranchAlias("convs_quality"        );
  produces<vector<int> >		("convsalgo"		).setBranchAlias("convs_algo"		);
  produces<vector<vector<int> > >	("convstkidx"		).setBranchAlias("convs_tkidx"		);
  produces<vector<vector<int> > >	("convstkalgo"		).setBranchAlias("convs_tkalgo"		);
  produces<vector<vector<int> > >	("convsnHitsBeforeVtx"	).setBranchAlias("convs_nHitsBeforeVtx"	);

  produces<vector<float> >              ("convsndof"            ).setBranchAlias("convs_ndof"           );
  produces<vector<float> >              ("convschi2"            ).setBranchAlias("convs_chi2"           );
  produces<vector<float> >              ("convsdl"              ).setBranchAlias("convs_dl"             );

  //this comes as a momentum vector from the Conversion object, not a p4 so the 
  produces<vector<LorentzVector> >      ("convsrefitPairMomp4"  ).setBranchAlias("convs_refitPairMom_p4");
  produces<vector<LorentzVector> >      ("convsvtxpos"          ).setBranchAlias("convs_vtxpos"         );

  //produces<vector<vector<float> > >     ("convsdlClosestHit"    ).setBranchAlias("convs_dlClosestHit"   );// signed decay length between nearest hit and conversion vertex
  //produces<vector<vector<float> > >     ("convsdlClosestHitErr" ).setBranchAlias("convs_dlClosestHitErr");// signed decay length error

}

void RecoConversionMaker::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {


  auto_ptr<vector<int> >		convs_isConverted	(new vector<int>		);
  auto_ptr<vector<int> >		convs_algo		(new vector<int>		);
  auto_ptr<vector<int> >                convs_quality           (new vector<int>                );
  
  auto_ptr<vector<vector<int> > >       convs_tkidx		(new vector<vector<int> >       );
  auto_ptr<vector<vector<int> > >       convs_tkalgo            (new vector<vector<int> >       );
  auto_ptr<vector<vector<int> > >       convs_nHitsBeforeVtx	(new vector<vector<int> >       );
  
  auto_ptr<vector<float> >              convs_ndof              (new vector<float>              );          
  auto_ptr<vector<float> >              convs_chi2              (new vector<float>              );            
  auto_ptr<vector<float> >              convs_dl                (new vector<float>              );
  auto_ptr<vector<LorentzVector> >      convs_refitPairMom_p4   (new vector<LorentzVector>      );
  auto_ptr<vector<LorentzVector> >      convs_vtxpos            (new vector<LorentzVector>      );
  //auto_ptr<vector<vector<float> > >     convs_dlClosestHit      (new vector<vector<float> >     );
  //auto_ptr<vector<vector<float> > >     convs_dlClosestHitErr   (new vector<vector<float> >     );
	   

  // get reco Conversions
  Handle<View<Conversion> > convs_h;
  iEvent.getByLabel("reducedEgamma", recoConversionInputTag_.label(), convs_h);
  
  Handle<BeamSpot> beamSpotH;
  iEvent.getByLabel(beamSpotInputTag_, beamSpotH);

  for(View<Conversion>::const_iterator it = convs_h->begin();
      it != convs_h->end(); it++) { 

    convs_isConverted->push_back(it->isConverted());
    convs_algo->push_back(it->algo());
    //quality 
    int qualityMask = 0;
    for(int iM = 0; iM < 32; ++iM) {
      if(it->quality((Conversion::ConversionQuality)iM))
	qualityMask |= 1 << iM;
    }
    convs_quality->push_back(qualityMask);
    
    // Can't use tracks in miniA
    /*
    vector<edm::RefToBase<reco::Track> > v_temp_trks = it->tracks();
    vector<int> v_temp_out;
    vector<int> v_temp_outalgo;
    for(unsigned int i = 0; i < v_temp_trks.size(); i++) {
      v_temp_out.push_back(v_temp_trks.at(i).key());
      v_temp_outalgo.push_back(v_temp_trks.at(i)->algo());
    }
			       

    convs_tkidx->push_back(v_temp_out);
    convs_tkalgo->push_back(v_temp_outalgo);

    v_temp_out.clear();
    v_temp_outalgo.clear();
    */

    vector<int> v_temp_out;
    vector<uint8_t> v_temp_nhits = it->nHitsBeforeVtx();
    for(unsigned int i = 0; i < v_temp_nhits.size(); i++) 
      v_temp_out.push_back(v_temp_nhits.at(i));
    convs_nHitsBeforeVtx->push_back(v_temp_out);

    convs_ndof->push_back(it->conversionVertex().ndof() );
    convs_chi2->push_back(it->conversionVertex().chi2() );
    convs_dl->push_back(lxy(beamSpotH->position(), *it));

    convs_refitPairMom_p4->push_back(LorentzVector(it->refittedPair4Momentum()));
    convs_vtxpos->push_back(LorentzVector(it->conversionVertex().x(), 
					  it->conversionVertex().y(),
					  it->conversionVertex().z(),
					  0));
    
    /*
      std::vector<Measurement1DFloat> v_temp_dlClosestHit = it->dlClosestHitToVtx();
      vector<float> v_fltemp, v_fltemp_err;
      for(unsigned int i = 0; i < v_temp_dlClosestHit.size(); i++) {
      v_fltemp.push_back(v_temp_dlClosestHit.at(i).value());
      v_fltemp_err.push_back(v_temp_dlClosestHit.at(i).error());
      }
      convs_dlClosestHit->push_back(v_fltemp);
      convs_dlClosestHitErr->push_back(v_fltemp_err);
    */
      

  }//reco conversion loop



  iEvent.put(convs_isConverted		, "convsisConverted"	 );
  iEvent.put(convs_quality              , "convsquality"         );
  iEvent.put(convs_algo			, "convsalgo"		 );
  iEvent.put(convs_tkidx		, "convstkidx"		 );
  iEvent.put(convs_tkalgo		, "convstkalgo"		 );
  iEvent.put(convs_nHitsBeforeVtx	, "convsnHitsBeforeVtx"	 );

  iEvent.put(convs_ndof                 , "convsndof"            );
  iEvent.put(convs_chi2                 , "convschi2"            );
  iEvent.put(convs_dl                   , "convsdl"              );
  iEvent.put(convs_refitPairMom_p4      , "convsrefitPairMomp4"  );
  iEvent.put(convs_vtxpos               , "convsvtxpos"          );
  //iEvent.put(convs_dlClosestHit       , "convsdlClosestHit"    );
  //iEvent.put(convs_dlClosestHitErr	, "convsdlClosestHitErr" );
  
}


double RecoConversionMaker::lxy(const math::XYZPoint& myBeamSpot, const Conversion& conv) const {

  const reco::Vertex &vtx = conv.conversionVertex();
  if (!vtx.isValid()) return -9999.;

  math::XYZVectorF mom = conv.refittedPairMomentum();
  
  double dbsx = vtx.x() - myBeamSpot.x();
  double dbsy = vtx.y() - myBeamSpot.y();
  double lxy = (mom.x()*dbsx + mom.y()*dbsy)/mom.rho();
  return lxy;  
  
}

// ------------ method called once each job just before starting event loop  ------------
void RecoConversionMaker::beginJob() {
}

// ------------ method called once each job just after ending the event loop  ------------
void RecoConversionMaker::endJob() {

}

//define this as a plug-in
DEFINE_FWK_MODULE(RecoConversionMaker);
