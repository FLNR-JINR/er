#include "ERNDDigitizer.h"

#include "TVector3.h"
#include "TGeoMatrix.h"
#include "TMath.h"
#include "TRandom3.h"

#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"

#include <limits>
#include <vector>
#include <map>

#include "ERDetectorList.h"
#include "ERNDPoint.h"

Int_t ERNDDigitizer::fEvent = 0;
// ----------------------------------------------------------------------------
ERNDDigitizer::ERNDDigitizer()
  : FairTask("ER ND Digi producing scheme")
,fNDPoints(NULL)
,fNDDigis(NULL)
,fEdepErrorA(0)
,fEdepErrorB(0)
,fEdepErrorC(0)
,fLYErrorA(0)
,fLYErrorB(0)
,fLYErrorC(0)
,fQuenchThreshold(0)
,fLYThreshold(0)
,fProbabilityB(0)
,fProbabilityC(0)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNDDigitizer::ERNDDigitizer(Int_t verbose)
  : FairTask("ER ND Digi producing scheme ", verbose)
,fNDPoints(NULL)
,fNDDigis(NULL)
,fEdepErrorA(0)
,fEdepErrorB(0)
,fEdepErrorC(0)
,fLYErrorA(0)
,fLYErrorB(0)
,fLYErrorC(0)
,fQuenchThreshold(0)
,fLYThreshold(0)
,fProbabilityB(0)
,fProbabilityC(0)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNDDigitizer::~ERNDDigitizer()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERNDDigitizer::SetParContainers()
{
  // Get run and runtime database
  FairRun* run = FairRun::Instance();
  if ( ! run ) Fatal("SetParContainers", "No analysis run");

  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  if ( ! rtdb ) Fatal("SetParContainers", "No runtime database");
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus ERNDDigitizer::Init()
{
  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  
  fNDPoints = (TClonesArray*) ioman->GetObject("NDPoint");
  if (!fNDPoints)
    Fatal("Init", "Can`t find collection NDPoint!"); 

  // Register output array fNDDigis
  fNDDigis = new TClonesArray("ERNDDigi",1000);

  ioman->Register("NDDigi", "ND digi", fNDDigis, kTRUE);

  fSetup = ERNDSetup::Instance();
   
  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERNDDigitizer::Exec(Option_t* opt)
{
  
  LOG(DEBUG2) << "ERNDDigitizer event: " << fEvent++ << FairLogger::endl;
  Reset();

  //Sort points by crystalls
  std::map<Int_t, std::vector<Int_t>> pointsByCrystall;
  for (Int_t iPoint = 0; iPoint < fNDPoints->GetEntriesFast(); iPoint++){
    ERNDPoint* point = (ERNDPoint*)fNDPoints->At(iPoint);
    pointsByCrystall[point->StilbenNr()].push_back(iPoint);
  }

  //loop over crystall with points
  for (const auto &itCrystall : pointsByCrystall){
    Float_t edep = 0;   // sum edep in crystall
    Float_t time = std::numeric_limits<float>::max();   // first time in crystall
    Float_t ly = 0.;    // sum ligth yield in crystall

    //@TODO it must be in ERNDHitFinder and by digi
    //calc digi position by first point
    TVector3 pos;       // position of crystall in global
    ERNDPoint* firstPoint = (ERNDPoint*)fNDPoints->At((*(itCrystall.second.begin())));
    TVector3 pointPos;
    firstPoint->Position(pointPos);
    fSetup->PMTPos(pointPos,pos);

    //loop over points in crysrall itCrystall.first()
    for (const auto iPoint : itCrystall.second){
      ERNDPoint* point = (ERNDPoint*)fNDPoints->At(iPoint);
      edep += point->GetEnergyLoss();
      ly += point->LightYield();
      if (point->GetTime() < time){
        time = point->GetTime();
      }
    }

    Float_t edepSigma = sqrt(pow(fEdepErrorA,2) + pow(fEdepErrorB*TMath::Sqrt(edep/1000.),2) + pow(fEdepErrorC*edep,2));
    edep = gRandom->Gaus(edep, edepSigma);

    /*
    fEdepErrorB and fLYErrorB are expressed in per cent divided by square root of edep or LY,
    expressed in MeVlike it is usually done in detector description. e.g. for Edep=0.001 (i.e. 1 Mev)
    sigma is equal0.04*0.001Gev = 0.04 MeV
    */

    Float_t lySigma = sqrt(pow(fLYErrorA,2) + pow(fLYErrorB*TMath::Sqrt(ly/1000.),2) + pow(fLYErrorC*ly,2));
    ly = gRandom->Gaus(ly, lySigma);

    Float_t timeSigma = TMath::Sqrt(fTimeErrorA/ly);
    time = gRandom->Gaus(time, timeSigma);

    Float_t neutronProb = NeutronProbability(edep,ly);
    
    TVector3 dpos = TVector3(0.01, 0.01, 0.01); //ошибка пока фиксирована
    AddDigi(kND, pos, dpos,itCrystall.first,edep, ly, time, neutronProb);
  }
  LOG(DEBUG) << "Digis count: " << fNDDigis->GetEntriesFast() << FairLogger::endl;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERNDDigitizer::Reset()
{
  if (fNDDigis) {
    fNDDigis->Delete();
  }
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERNDDigitizer::Finish()
{   
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNDDigi* ERNDDigitizer::AddDigi(Int_t detID, TVector3& pos, TVector3& dpos,
                       Int_t point_index, Float_t edep, Float_t ly, Float_t time,Float_t neutronProb)
{
  ERNDDigi *Digi = new((*fNDDigis)[fNDDigis->GetEntriesFast()])
              ERNDDigi(fNDDigis->GetEntriesFast(),detID, pos, dpos, point_index, edep, ly, time, neutronProb);
  return Digi;
}
// ----------------------------------------------------------------------------
Float_t ERNDDigitizer::NeutronProbability(Float_t edep, Float_t ly){
  Float_t neutronProb;
  Float_t quench = ly/edep;
  if ((ly > fLYThreshold) && (quench < fQuenchThreshold)){
    neutronProb = 1.;
  }
  if ((ly < fLYThreshold) && (quench < fQuenchThreshold)){
    neutronProb = fProbabilityB+(1-fProbabilityB)*(ly/fLYThreshold);
  }
  if ((ly > fLYThreshold) && (quench > fQuenchThreshold)){
    neutronProb = 0.;
  }
  if ((ly < fLYThreshold) && (quench > fQuenchThreshold)){
    neutronProb = fProbabilityC*(1-ly/fLYThreshold);
  }
  return neutronProb;
}
//-----------------------------------------------------------------------------
ClassImp(ERNDDigitizer)
