//-*- C++ -*-
//
// Package:    NtupleMaker
// Class:      EventTrigMaker
// 
/**\class EventTrigMaker EventTrigMaker.cc CMS2/NtupleMakerMaker/src/EventTrigMaker.cc

Description: <one line class summary>

Implementation:
<Notes on implementation>
*/
//
// Original Author:  Puneeth Kalavase
//         Created:  Fri Jun  6 11:07:38 CDT 2008
// $Id: EventTrigMaker.cc,v 1.1.2.1 2009/11/12 19:31:19 slava77 Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/TriggerNames.h"
#include "CMS2/NtupleMaker/interface/EventTrigMaker.h"


#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1Trigger/interface/L1ParticleMap.h"
#include "DataFormats/L1Trigger/interface/L1ParticleMapFwd.h"
#include "DataFormats/L1Trigger/interface/L1MuonParticle.h"
#include "DataFormats/L1Trigger/interface/L1JetParticle.h"
#include "DataFormats/L1Trigger/interface/L1EtMissParticle.h"
#include "DataFormats/L1Trigger/interface/L1EmParticle.h"

#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"

#include "CondFormats/DataRecord/interface/L1GtTriggerMenuRcd.h"


#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "TString.h"

typedef math::XYZTLorentzVector LorentzVector;
typedef math::XYZPoint Point;
using namespace reco;
using namespace edm;
using namespace std;

//
// constructors and destructor
//

EventTrigMaker::EventTrigMaker(const edm::ParameterSet& iConfig) {

  
  produces<int>    ("evtHLT1"              ).setBranchAlias("evt_HLT1"                 );
  produces<int>    ("evtHLT2"              ).setBranchAlias("evt_HLT2"                 );
  produces<int>    ("evtHLT3"              ).setBranchAlias("evt_HLT3"                 );
  produces<int>    ("evtHLT4"              ).setBranchAlias("evt_HLT4"                 );
  produces<int>    ("evtHLT5"              ).setBranchAlias("evt_HLT5"                 );
  produces<int>    ("evtHLT6"              ).setBranchAlias("evt_HLT6"                 );
  produces<int>    ("evtHLT7"              ).setBranchAlias("evt_HLT7"                 );
  produces<int>    ("evtHLT8"              ).setBranchAlias("evt_HLT8"                 );
  produces<int>    ("evtL11"               ).setBranchAlias("evt_L1_1"                 );
  produces<int>    ("evtL12"               ).setBranchAlias("evt_L1_2"                 );
  produces<int>    ("evtL13"               ).setBranchAlias("evt_L1_3"                 );
  produces<int>    ("evtL14"               ).setBranchAlias("evt_L1_4"                 );
  produces<std::vector<TString> > ("evtL1trigNames"   ).setBranchAlias("evt_L1_trigNames"      );    
  produces<std::vector<TString> > ("evtHLTtrigNames"  ).setBranchAlias("evt_HLT_trigNames"     );
  
  
  // info
  haveTriggerInfo_ = iConfig.getUntrackedParameter<bool>("haveTriggerInfo");

}


EventTrigMaker::~EventTrigMaker() {}

void  EventTrigMaker::beginJob(const edm::EventSetup&) {
      
}

void EventTrigMaker::endJob() {
  
}

// ------------ method called to produce the data  ------------
void EventTrigMaker::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  
  auto_ptr<int>      evt_HLT1              (new int);
  auto_ptr<int>      evt_HLT2              (new int);
  auto_ptr<int>      evt_HLT3              (new int);
  auto_ptr<int>      evt_HLT4              (new int);
  auto_ptr<int>      evt_HLT5              (new int);
  auto_ptr<int>      evt_HLT6              (new int);
  auto_ptr<int>      evt_HLT7              (new int);
  auto_ptr<int>      evt_HLT8              (new int);
  auto_ptr<int>      evt_L11               (new int);
  auto_ptr<int>      evt_L12               (new int);
  auto_ptr<int>      evt_L13               (new int);
  auto_ptr<int>      evt_L14               (new int);
  auto_ptr<vector<TString> >      evt_HLT_trigNames        (new vector<TString>);
  auto_ptr<vector<TString> >      evt_L1_trigNames         (new vector<TString>);
  
  //fill HLT info
  if(haveTriggerInfo_) {
    int *hlt1       = new int;
    int *hlt2       = new int;
    int *hlt3       = new int;
    int *hlt4       = new int;
    int *hlt5       = new int;
    int *hlt6       = new int;
    int *hlt7       = new int;
    int *hlt8       = new int;
    int *l11        = new int;
    int *l12        = new int;
    int *l13        = new int;
    int *l14        = new int;
    //string *l1names  = new string; //got warning 'unused variable'

    fillHLTInfo(iEvent, hlt1, hlt2, hlt3, hlt4, hlt5, 
		hlt6, hlt7, hlt8, *evt_HLT_trigNames);
    
    edm::ESHandle<L1GtTriggerMenu> menuRcd;
    iSetup.get<L1GtTriggerMenuRcd>().get(menuRcd) ;
    const L1GtTriggerMenu* menu = menuRcd.product();
    fillL1Info(iEvent, l11, l12, l13, l14, *evt_L1_trigNames, menu);
    
    *evt_HLT1 = *hlt1;
    *evt_HLT2 = *hlt2;
    *evt_HLT3 = *hlt3;
    *evt_HLT4 = *hlt4;
    *evt_HLT5 = *hlt5;
    *evt_HLT6 = *hlt6;
    *evt_HLT7 = *hlt7;
    *evt_HLT8 = *hlt8;
    *evt_L11  = *l11;
    *evt_L12  = *l12;
    *evt_L13  = *l13;
    *evt_L14  = *l14;
    
  } else {
    *evt_HLT1 = -999;
    *evt_HLT2 = -999;
    *evt_HLT3 = -999;
    *evt_HLT4 = -999;
    *evt_HLT5 = -999;
    *evt_HLT6 = -999;
    *evt_HLT7 = -999;
    *evt_HLT8 = -999;
    *evt_L11  = -999;
    *evt_L12  = -999;
    *evt_L13  = -999;
    *evt_L14  = -999;
   
  }

  iEvent.put(evt_HLT1             ,"evtHLT1"            );
  iEvent.put(evt_HLT2             ,"evtHLT2"            );
  iEvent.put(evt_HLT3             ,"evtHLT3"            );
  iEvent.put(evt_HLT4             ,"evtHLT4"            );
  iEvent.put(evt_HLT5             ,"evtHLT5"            );
  iEvent.put(evt_HLT6             ,"evtHLT6"            );
  iEvent.put(evt_HLT7             ,"evtHLT7"            );
  iEvent.put(evt_HLT8             ,"evtHLT8"            );
  iEvent.put(evt_L11              ,"evtL11"             );
  iEvent.put(evt_L12              ,"evtL12"             );
  iEvent.put(evt_L13              ,"evtL13"             );
  iEvent.put(evt_L14              ,"evtL14"             );
  iEvent.put(evt_HLT_trigNames    ,"evtHLTtrigNames"    );
  iEvent.put(evt_L1_trigNames     ,"evtL1trigNames"     );


}



//-----------------------------------------------------------------------
// fill HLT info
//-----------------------------------------------------------------------
void EventTrigMaker::fillHLTInfo(const Event& iEvent, int *h1, int *h2, int *h3, int *h4,
			     int *h5, int *h6, int *h7, int *h8, std::vector<TString>& hltnames) {
			     

  edm::Handle<edm::TriggerResults> triggerResults;
  iEvent.getByLabel(edm::InputTag("TriggerResults", "", "HLT"), triggerResults);
  edm::TriggerNames triggerNames(*triggerResults);
  *h1=0;
  *h2=0;
  *h3=0;
  *h4=0;
  *h5=0;
  *h6=0;
  *h7=0;
  *h8=0;
  
  //trigger index, trigger string and L1 accept
  
  
  unsigned int ntriggers = triggerResults->size();
  if(ntriggers > 255)
    throw cms::Exception("EventTrigMaker: Number of HLT trigger variables must be increased!");
  for (unsigned int i = 0; i < ntriggers; i++) {
    
     hltnames.push_back(triggerNames.triggerName(i).c_str());
   
    if(i<=31) {
      unsigned int bitmask = 1;
      if(triggerResults->accept(i)) {
	bitmask <<=i;
	*h1 |= bitmask;
      }
       }
    
       
    if(i>=32&&i<=63) {
      unsigned int bitmask = 1;
      if(triggerResults->accept(i)) {
	bitmask <<=(i-32);
	*h2 |= bitmask;
      }
    }
    
       if(i>=64&&i<=95) {
         unsigned int bitmask = 1;
         if(triggerResults->accept(i)) {
           bitmask <<=(i-64);
           *h3 |= bitmask;
         }
       }

       if(i>=96&&i<=127) {
         unsigned int bitmask = 1;
         if(triggerResults->accept(i)) {
           bitmask <<=(i-96);
           *h4 |= bitmask;
         }
       }

       if(i>=128&&i<=159) {
         unsigned int bitmask = 1;
         if(triggerResults->accept(i)) {
           bitmask <<=(i-128);
           *h5 |= bitmask;
         }
       }

       if(i>=160&&i<=191) {
         unsigned int bitmask = 1;
         if(triggerResults->accept(i)) {
           bitmask <<=(i-160);
           *h6 |= bitmask;
         }
       }

       if(i>=192&&i<=223) {
         unsigned int bitmask = 1;
         if(triggerResults->accept(i)) {
           bitmask <<=(i-192);
           *h7 |= bitmask;
         }
       }

       if(i>=224&&i<=255) {
         unsigned int bitmask = 1;
         if(triggerResults->accept(i)) {
           bitmask <<=(i-224);
           *h8 |= bitmask;
         }
       }

     }
}

//----------------------------------------------------------
//fill L1 info
//---------------------------------------------------------
void EventTrigMaker::fillL1Info(const Event& iEvent, int* l1_1, int* l1_2,
			    int* l1_3, int* l1_4, std::vector<TString>& l1names,
			    const L1GtTriggerMenu* menu) {
  
  edm::Handle<L1GlobalTriggerReadoutRecord > gtRecord;
  iEvent.getByLabel("gtDigis", gtRecord);
  //if(L1PMC.isValid()) {
  const DecisionWord dWord = gtRecord->decisionWord();

  for(AlgorithmMap::const_iterator algo = menu->gtAlgorithmMap().begin();
      algo != menu->gtAlgorithmMap().end(); algo++) {
    if(algo->first != algo->second.algoName()) {
       cout << "The name of the L1 Trigger bit in the Algorithm map is not" 
	    << " the same as the name from the L1GtAlgorithm object."
	    << "Something is wrong!!!!" << endl;
    }
    
     l1names.push_back( algo->second.algoName().c_str() );
  }
	
   *l1_1=0;
   *l1_2=0;
   *l1_3=0;
   *l1_4=0;
   unsigned int ntriggers = dWord.size();
   if(ntriggers > 128)
     throw cms::Exception("EventTrigMaker: Number of HLT trigger variables must be increased!");
   for(unsigned int i = 0; i<ntriggers ; i++) {
     if(i<=31) {
       unsigned int bitmask = 1;
       if(dWord.at(i)) {
         bitmask <<=i;
         *l1_1 |= bitmask;
       }
     }
     
     if(i>=32&&i<=63) {
       unsigned int bitmask = 1;
       if(dWord.at(i)) {
         bitmask <<=(i-32);
         *l1_2 |= bitmask;
       }
     }
     
     if(i>=64&&i<=95) {
       unsigned int bitmask = 1;
       if(dWord.at(i)) {
         bitmask <<=(i-64);
         *l1_3 |= bitmask;
       }
     }
     
     if(i>=96&&i<=127) {
       unsigned int bitmask = 1;
       if(dWord.at(i)) {
         bitmask <<=(i-96);
         *l1_4 |= bitmask;
       }
     }
   }
}

//define this as a plug-in
DEFINE_FWK_MODULE(EventTrigMaker);
