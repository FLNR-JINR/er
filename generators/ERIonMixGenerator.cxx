// -------------------------------------------------------------------------
// -----                   ERIonMixGenerator source file                 -----
// -----                   Created by            -----
// -------------------------------------------------------------------------
#include "ERIonMixGenerator.h"

#include "FairIon.h"                    // for FairIon
#include "FairParticle.h"               // for FairParticle
#include "FairPrimaryGenerator.h"       // for FairPrimaryGenerator
#include "FairRunSim.h"                 // for FairRunSim
#include "FairLogger.h"                 // for logging

#include "Riosfwd.h"                    // for ostream
#include "TDatabasePDG.h"               // for TDatabasePDG
#include "TObjArray.h"                  // for TObjArray
#include "TParticle.h"                  // for TParticle
#include "TParticlePDG.h"               // for TParticlePDG
#include "TRandom.h"
#include "TMath.h"

#include <stdio.h>                      // for NULL, sprintf

// -----   Default constructor   ------------------------------------------
ERIonMixGenerator::ERIonMixGenerator()
  : ERIonGenerator()
{
//  LOG(WARNING) << "ERIonMixGenerator: "
//               << " Please do not use the default constructor! " 
//               << FairLogger::endl;
}
// ------------------------------------------------------------------------

// -----   Default constructor   ------------------------------------------
ERIonMixGenerator::ERIonMixGenerator(TString name, Int_t z, Int_t a, Int_t q, Int_t mult)
  : ERIonGenerator(name, z, a, q, mult)
{
  fBgIons.clear();
}
//_________________________________________________________________________

// -----   Destructor   ---------------------------------------------------
ERIonMixGenerator::~ERIonMixGenerator()
{
// if (fIon) delete fIon;
}

void ERIonMixGenerator::AddBackgroundIon(TString name, Int_t z, Int_t a, Int_t q, Double_t newIonProb)
{
  SetPhiRange();
  static Double_t sumProbability;

  if(fBgIons.size() == 0)
  {
    sumProbability = 1;
  }

  sumProbability += newIonProb;

  /*if((sumProbability) >= 1)
  {
    LOG(DEBUG) << "Summary probability of appearing background ions more then 1"
       << FairLogger::endl;
    return ;
  }*/

  std::map<Double_t, TString>::iterator itBgIons;
  for(itBgIons = fBgIons.begin(); itBgIons != fBgIons.end(); itBgIons++) {
    // ---- normalization of prababilities values with new ion --------------
    Double_t curIonProb = itBgIons->first;
    TString  ionName = itBgIons->second;
    Double_t ionProb = curIonProb - (curIonProb * newIonProb / sumProbability);
    fBgIons.erase(itBgIons);
    fBgIons.insert(std::make_pair(ionProb, ionName));
  }

  fBgIons.insert(std::make_pair(newIonProb / sumProbability, name));
  
  for(itBgIons = fBgIons.begin(); itBgIons != fBgIons.end(); itBgIons++) {
    std::cout << "Prob " << itBgIons->first << std::endl;
  }

  FairRunSim* run = FairRunSim::Instance();
  if ( ! run ) {
    LOG(ERROR) << "No FairRun instantised!" 
         << FairLogger::endl;
  } else {           
    run->AddNewIon(new FairIon(name, z, a, q));
  }
}

Bool_t ERIonMixGenerator::ReadEvent(FairPrimaryGenerator* primGen)
{
  Double_t randResult;
  TString  ionName;
  Double_t rigidityMin;
  Double_t rigidityMax;
  Double_t gausRigidity;
  Double_t pMin = fPMin, pMax = fPMax;
  Double_t pGaus = fGausP;
  // Generate particles
  for (Int_t k = 0; k < fMult; k++) {

    randResult = gRandom->Uniform(0., 1.);

    auto it = std::find_if(fBgIons.begin(), fBgIons.end(),
                            [randResult](const std::pair<Double_t, TString> &t)->bool
                            {
                              return (t.first > randResult);
                            }
                          ); 
    (it == fBgIons.end()) ? ionName = fIon->GetName() : ionName = it->second;

    TParticlePDG* thisPart =
    TDatabasePDG::Instance()->GetParticle(ionName);
    if ( ! thisPart ) {
      LOG(WARNING) << "ERIonGenerator: Ion " << ionName
      << " not found in database!" << FairLogger::endl;
      return kFALSE;
    }

    Double_t charge = thisPart->Charge();

    if(fPRangeIsSet){

      rigidityMin = pMin / fIon->GetZ() / 3;
      rigidityMax = pMax / fIon->GetZ() / 3;
	    fPMin = rigidityMin * charge;
	    fPMax = rigidityMax * charge;
    }

    if(fSigmaPIsSet)
    {
      gausRigidity = pGaus / fIon->GetZ() / 3;
      fGausP = gausRigidity * charge;
    }

    std::cout << "Pmin  " << fPMin << " Pmax " << fPMax << " GausP " << fGausP << std::endl;

    SpreadingParameters();

    int pdgType = thisPart->PdgCode();

    std::cout << "ERIonGenerator: Generating " << fMult << " ions of type "
        << ionName << " (PDG code " << pdgType << ")" 
        << std::endl;
    std::cout << "    Momentum (" << fPx << ", " << fPy << ", " << fPz
        << ") Gev from vertex (" << fX << ", " << fY
        << ", " << fZ << ") cm" << std::endl;
    primGen->AddTrack(pdgType, fPx, fPy, fPz, fX, fY, fZ);


  }
  fGausP = pGaus;
  fPMin = pMin;
  fPMax = pMax;
  return kTRUE;
}

ClassImp(ERIonMixGenerator)