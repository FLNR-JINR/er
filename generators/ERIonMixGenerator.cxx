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
}
//_________________________________________________________________________

// -----   Destructor   ---------------------------------------------------
ERIonMixGenerator::~ERIonMixGenerator()
{
// if (fIon) delete fIon;
}

void ERIonMixGenerator::AddBackgroundIon(TString name, Int_t z, Int_t a, Int_t q, Int_t probability)
{
/*  SetPhiRange();
  fIon= new FairIon(name, z, a, q);
  FairRunSim* run = FairRunSim::Instance();
  if ( ! run ) {
    LOG(ERROR) << "No FairRun instantised!" 
         << FairLogger::endl;
  } else {
    run->AddNewIon(fIon);
  }*/
}

ClassImp(ERIonMixGenerator)