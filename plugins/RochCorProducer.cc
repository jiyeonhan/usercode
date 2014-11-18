// -*- C++ -*-
//
// Package:    MuonAnalysis/RochesterCorrection
// Class:      RochesterCorrection
// 
/**\class RochesterCorrection RochesterCorrection.cc MuonAnalysis/RochesterCorrection/plugins/RochesterCorrection.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Jiyeon Han
//         Created:  Thu, 13 Nov 2014 21:02:25 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/CompositeCandidate.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "MuonAnalysis/RochesterCorrection/interface/RochesterCorrections.h"

//
// class declaration
//

class RochCorProducer : public edm::EDProducer {
public:
  explicit RochCorProducer(const edm::ParameterSet&);
  ~RochCorProducer();
  
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  
  typedef math::XYZTLorentzVector LorentzVector;
  
private:
  virtual void beginJob() override;
  virtual void produce(edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;
  
  edm::InputTag theMuonLabel_;  
  edm::EDGetTokenT<reco::MuonCollection> theRecoMuonToken_;
  
  boost::shared_ptr<rochcor::RochesterCorrections> correction_;      
  //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
  //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
  //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
  //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
  
  // ----------member data ---------------------------
};

//
// constants, enums and typedefs
//


//
// static data member definitions
//

//
// constructors and destructor
//
RochCorProducer::RochCorProducer(const edm::ParameterSet& iConfig) :
  theMuonLabel_( iConfig.getParameter<edm::InputTag>( "MuonLabel" ) ),
  theRecoMuonToken_( mayConsume<reco::MuonCollection>( theMuonLabel_ ) )
{
  //register your products
  /* Examples
     produces<ExampleData2>();
     
     //if do put with a label
     produces<ExampleData2>("label");
     
     //if you want to put into the Run
     produces<ExampleData2,InRun>();
  */
  //now do what ever other initialization is needed
  
  produces<reco::MuonCollection>();
  
}


RochCorProducer::~RochCorProducer()
{
  
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
  
}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
RochCorProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;

  edm::Handle<reco::MuonCollection> allMuons;
  iEvent.getByToken(theRecoMuonToken_, allMuons);

  correction_.reset(new rochcor::RochesterCorrections( ) );
  
  std::auto_ptr<reco::MuonCollection> pOut(new reco::MuonCollection);

  // Apply the correction and produce the new muons
  for( std::vector<reco::Muon>::const_iterator muon = allMuons->begin(); muon != allMuons->end(); ++muon ) {

    double eta = muon->eta();
    double phi = muon->phi();
    double mass = muon->mass();

    TLorentzVector mu;
    mu.SetPtEtaPhiM(muon->pt(), eta, phi, mass);
    int chg = muon->charge();
    
    if(iEvent.isRealData()){
      correction_->momcor_data(mu, chg, 0);
    }else{
      correction_->momcor_mc(mu, chg, iEvent.streamID());
    }

    double pt = mu.Pt();

    reco::Muon * newMuon = muon->clone();
    newMuon->setP4( reco::Particle::PolarLorentzVector( pt, eta, phi, mass ) );
    //std::cout<<"muon pt comparison = "<<muon->pt()<<" : "<<pt<<" => "<<eta<<" , "<<phi<<" , "<<chg<<std::endl;
    pOut->push_back(*newMuon);
  }

  iEvent.put(std::move(pOut));
    
}

// ------------ method called once each job just before starting event loop  ------------
void 
RochCorProducer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
RochCorProducer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
/*
void
RochCorProducer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a run  ------------
/*
void
RochCorProducer::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when starting to processes a luminosity block  ------------
/*
void
RochCorProducer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
RochCorProducer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
RochCorProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(RochCorProducer);
