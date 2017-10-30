/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERIonMixGenerator.h"

#include <stdio.h>                      // for NULL, sprintf
#include <algorithm>

#include <iosfwd>                       // for ostream
#include "TDatabasePDG.h"               // for TDatabasePDG
#include "TObjArray.h"                  // for TObjArray
#include "TParticle.h"                  // for TParticle
#include "TParticlePDG.h"               // for TParticlePDG
#include "TRandom.h"
#include "TMath.h"

#include "FairIon.h"                    // for FairIon
#include "FairParticle.h"               // for FairParticle
#include "FairPrimaryGenerator.h"       // for FairPrimaryGenerator
#include "FairRunSim.h"                 // for FairRunSim
#include "FairLogger.h"                 // for logging
//-------------------------------------------------------------------------------------------------
ERIonMixGenerator::ERIonMixGenerator()
  : ERIonGenerator()
{
//  LOG(WARNING) << "ERIonMixGenerator: "
//               << " Please do not use the default constructor! " 
//               << FairLogger::endl;
}
//-------------------------------------------------------------------------------------------------
ERIonMixGenerator::ERIonMixGenerator(TString name, Int_t z, Int_t a, Int_t q, Int_t mult)
  : ERIonGenerator(name, z, a, q, mult)
{
  fBgIons.clear();
}
//-------------------------------------------------------------------------------------------------
ERIonMixGenerator::~ERIonMixGenerator()
{
// if (fIon) delete fIon;
}
//-------------------------------------------------------------------------------------------------
void ERIonMixGenerator::AddBackgroundIon(TString name, Int_t z, Int_t a, Int_t q, Double_t newIonProb)
{
  SetPhiRange();

  if(fBgIons.size() == 0) {
    fSumProbability = 0;
  }
  fSumProbability += newIonProb;
  fBgIons.insert(std::make_pair(fSumProbability, name));
  LOG(DEBUG) << "Prob " << fSumProbability << FairLogger::endl;
  FairRunSim* run = FairRunSim::Instance();
  if ( ! run ) {
    LOG(ERROR) << "No FairRun instantised!" 
         << FairLogger::endl;
  } else {           
    run->AddNewIon(new FairIon(name, z, a, q));
  }
}
//-------------------------------------------------------------------------------------------------
Bool_t ERIonMixGenerator::ReadEvent(FairPrimaryGenerator* primGen)
{
  Double_t randResult;
  TString  ionName;
  Double_t rigidityMin;
  Double_t rigidityMax;
  Double_t gausRigidity, sigmaRigidity;
  Double_t pMin = fPMin, pMax = fPMax;
  Double_t pGaus = fGausP;
  Double_t pSigma = fSigmaP;
  // Generate particles
  for (Int_t k = 0; k < fMult; k++) {

    randResult = gRandom->Uniform(0., 1.) * (fSumProbability + 1);

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

    if(fPRangeIsSet) {
      rigidityMin = pMin / fIon->GetZ() / 3;
      rigidityMax = pMax / fIon->GetZ() / 3;
	    fPMin = rigidityMin * charge;
	    fPMax = rigidityMax * charge;
    }

    if(fSigmaPIsSet) {
      gausRigidity = pGaus / fIon->GetZ() / 3;
      sigmaRigidity = fSigmaP / fIon->GetZ() / 3;
      fGausP = gausRigidity * charge;
      fSigmaP = sigmaRigidity * charge;
    }
    LOG(DEBUG) << "Pmin  " << fPMin << " Pmax " << fPMax << " GausP " << fGausP << FairLogger::endl;

    SpreadingParameters();
    int pdgType = thisPart->PdgCode();

    LOG(DEBUG) << "ERIonGenerator: Generating " << fMult << " ions of type "
        << ionName << " (PDG code " << pdgType << ")" 
        << FairLogger::endl;
    LOG(DEBUG) << "    Momentum (" << fPx << ", " << fPy << ", " << fPz
        << ") Gev from vertex (" << fX << ", " << fY
        << ", " << fZ << ") cm" << FairLogger::endl;
    primGen->AddTrack(pdgType, fPx, fPy, fPz, fX, fY, fZ);
  }
  fGausP = pGaus;
  fPMin = pMin;
  fPMax = pMax;
  fSigmaP = pSigma;
  return kTRUE;
}
//-------------------------------------------------------------------------------------------------
ClassImp(ERIonMixGenerator)
