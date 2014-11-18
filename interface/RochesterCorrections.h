// A slight revision of the bare RochesterCorrection code...
// Modifications from L. Gray
#ifndef __ROCHESTER_CORRECTIONS_H__
#define __ROCHESTER_CORRECTIONS_H__
#include <iostream>
#include <map>
//#ifdef
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "CLHEP/Random/RandomEngine.h"
//#endif
#include <TSystem.h>
#include <TROOT.h>
#include <TChain.h>
#include <TClonesArray.h>
#include <TString.h>
#include <TMath.h>
#include <TLorentzVector.h>
#include <TRandom3.h>

namespace rochcor {
  class RochesterCorrections {
  public:
    RochesterCorrections();    
    ~RochesterCorrections();
    // momentum, charge, runopt,
    void momcor_mc(TLorentzVector&, int, edm::StreamID const&);
    void momcor_data(TLorentzVector&, int, int);
    int etabin(float);
    int phibin(float);
  private:
    // #ifdef
    edm::Service<edm::RandomNumberGenerator> rng;
    // #else
    //TRandom3 eran;
    //TRandom3 sran;
    //#endif
    //static float netabin[9] = {-2.4,-2.1,-1.4,-0.7,0.0,0.7,1.4,2.1,2.4};

    static const int neta = 8;
    static const int nphi = 8;
    static const double pi;
    static const float netabin[neta+1];
    static const float genm_smr; //gen mass peak with eta dependent gaussian smearing => better match in Z mass profile vs. eta/phi
    static const float genm; //gen mass peak without smearing => Z mass profile vs. eta/phi in CMS note
    static const float recm; //rec mass peak in MC (2011A)
    static const float drecm; //rec mass peak in data (2011A)
    static const float mgscl_stat; //stat. error of global factor for mass peak in MC (2011A)  
    static const float mgscl_syst; //syst. error of global factor for mass peak in MC (2011A)  
    static const float dgscl_stat; //stat. error of global factor for mass peak in data (2011A)
    static const float dgscl_syst; //syst. error of global factor for mass peak in data (2011A)
    //iteration2 after FSR : after Z Pt correction
    static const float sf[neta];
    static const float sfer[neta];
    static const float gsf[neta];
    static const float gsfer[neta];    
    //---------------------------------------------------------------------------------------------
    static const float dcor_bf[nphi][neta];  
    static const float dcor_ma[nphi][neta];
    static const float mcor_bf[nphi][neta];
    static const float mcor_ma[nphi][neta];
    static const float dcor_bfer[nphi][neta];  
    static const float dcor_maer[nphi][neta];
    static const float mcor_bfer[nphi][neta];
    static const float mcor_maer[nphi][neta];
    //==============================================================================================
    static const float dmavg[nphi][neta];  
    static const float dpavg[nphi][neta];  
    static const float mmavg[nphi][neta];  
    static const float mpavg[nphi][neta];
    //===============================================================================================
    float mptsys_mc_dm[nphi][neta];
    float mptsys_mc_da[nphi][neta];
    float mptsys_da_dm[nphi][neta];
    float mptsys_da_da[nphi][neta];
    float gscler_mc_dev;
    float gscler_da_dev;    
  };
}// namespace rochcor
#endif  
