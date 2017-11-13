#include "ERNDDigitizer.h"

#include <vector>

#include "TVector3.h"
#include "TGeoMatrix.h"
#include "TMath.h"
#include "TRandom3.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include<iostream>
using namespace std;

#include "ERDetectorList.h"
#include "ERNDPoint.h"

Int_t ERNDDigitizer::fEvent = 0;
// ----------------------------------------------------------------------------
ERNDDigitizer::ERNDDigitizer()
  : FairTask("ER ND Digi producing scheme")
,fNDPoints(NULL)
,fNDDigis(NULL)
,fLYDispersionA(0)
,fLYDispersionB(0)
,fTimeDispersionPar(0),
fQuenchThreshold(0),
fLYThreshold(0),
fProbabilityB(0),
fProbabilityC(0)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNDDigitizer::ERNDDigitizer(Int_t verbose)
  : FairTask("ER ND Digi producing scheme ", verbose)
,fNDPoints(NULL)
,fNDDigis(NULL)
,fLYDispersionA(0)
,fLYDispersionB(0)
,fTimeDispersionPar(0),
fQuenchThreshold(0),
fLYThreshold(0),
fProbabilityB(0),
fProbabilityC(0)
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
  FairRunAna* run = FairRunAna::Instance();
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
  std::cout << std::endl;
  std::cout << "####### EVENT " << fEvent++ << " #####" << std::endl;
  std::cout << std::endl;
  std::cout << "ERNDDigitizer: "<< std::endl;
  Reset();

  for (Int_t iPoint = 0; iPoint < fNDPoints->GetEntriesFast(); iPoint++){
    ERNDPoint* point = (ERNDPoint*)fNDPoints->At(iPoint);
    TVector3 pointPos;
    point->Position(pointPos);
    TVector3 dpos = TVector3(0.01, 0.01, 0.01); //ошибка пока фиксирована
    TVector3 pos;
    fSetup->PMTPos(pointPos,pos);
    Float_t LYDispersion = (fLYDispersionA*fLYDispersionA)*point->LightYield()+(fLYDispersionB*fLYDispersionB)*(point->LightYield()*point->LightYield());
    Float_t lightYield = gRandom->Gaus(point->LightYield(), LYDispersion);
    Float_t time = gRandom->Gaus(point->GetTime(), TMath::Sqrt(fTimeDispersionPar/point->LightYield()));
    Float_t neutronProb;
    Float_t quench = point->LightYield()/point->GetEnergyLoss();
    if ((point->LightYield() > fLYThreshold) && (quench < fQuenchThreshold)){
      neutronProb = 1.;
    }
    if ((point->LightYield() < fLYThreshold) && (quench < fQuenchThreshold)){
      neutronProb = fProbabilityB+(1-fProbabilityB)*(point->LightYield()/fLYThreshold);
    }
    if ((point->LightYield() > fLYThreshold) && (quench > fQuenchThreshold)){
      neutronProb = 0.;
    }
    if ((point->LightYield() < fLYThreshold) && (quench > fQuenchThreshold)){
      neutronProb = fProbabilityC*(1-point->LightYield()/fLYThreshold);
    }
    AddDigi(kND, pos, dpos,iPoint,lightYield, time, neutronProb);
  }

  std::cout << "Digis count: " << fNDDigis->GetEntriesFast() << std::endl;
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
                       Int_t point_index, Float_t eloss, Float_t time,Float_t neutronProb)
{
  ERNDDigi *Digi = new((*fNDDigis)[fNDDigis->GetEntriesFast()])
              ERNDDigi(fNDDigis->GetEntriesFast(),detID, pos, dpos, point_index, eloss, time, neutronProb);
  return Digi;
}
// ----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
ClassImp(ERNDDigitizer)
