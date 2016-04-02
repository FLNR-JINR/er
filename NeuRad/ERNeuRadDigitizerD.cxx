#include "ERNeuRadDigitizerD.h"

#include <iostream>

#include "TClonesArray.h"

#include "FairLogger.h"

#include "ERNeuRadPMTSignal.h"
#include "ERNeuRadPMTSignalD.h"

using namespace std;
// ----------------------------------------------------------------------------
ERNeuRadDigitizerD::ERNeuRadDigitizerD()
  : ERNeuRadDigitizer()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadDigitizerD::ERNeuRadDigitizerD(Int_t verbose)
  : ERNeuRadDigitizer(verbose)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadDigitizerD::~ERNeuRadDigitizerD()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
InitStatus ERNeuRadDigitizerD::Init(){

  std::cerr << "ERNeuRadDigitizerD::Init()" << std::endl;
  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  
  fNeuRadPoints = (TClonesArray*) ioman->GetObject("NeuRadPoint");
  fNeuRadFirstStep = (TClonesArray*) ioman->GetObject("NeuRadFirstStep");
  //todo check
  
  // Register output array NeuRadFiberPoint and NeuRadDigi
  fNeuRadFiberPoint = new TClonesArray("ERNeuRadFiberPoint",fNeuRadPoints->GetEntriesFast()*2);
                          //*2, because front and back
  ioman->Register("NeuRadFiberPoint", "NeuRad Fiber points", fNeuRadFiberPoint, kTRUE);
  fNeuRadPMTSignal = new TClonesArray("ERNeuRadPMTSignalD", 1000);
  ioman->Register("NeuRadPMTSignal", "Signal after PMT", fNeuRadPMTSignal, kTRUE);
  fNeuRadDigi = new TClonesArray("ERNeuRadDigi",1000);
  ioman->Register("NeuRadDigi", "Digital response in NeuRad", fNeuRadDigi, kTRUE);
  
  fRand = new TRandom3();
  
  fNeuRadSetup = ERNeuRadSetup::Instance();
  fNeuRadSetup->Print();
  
  return kSUCCESS;
}
//-----------------------------------------------------------------------------

void ERNeuRadDigitizerD::PMTSignalsAndDigiCreating(Int_t iBundle, Int_t iFiber,
                                std::vector<ERNeuRadFiberPoint* >** frontPointsPerFibers,
                                std::vector<ERNeuRadFiberPoint* >** backPointsPerFibers)
{
  if( frontPointsPerFibers[iBundle][iFiber].size() > 0){
    ERNeuRadPMTSignalD* pmtFSignal = (ERNeuRadPMTSignalD*)AddPMTSignal(iBundle, iFiber,frontPointsPerFibers[iBundle][iFiber].size());
    for(Int_t iFPoint = 0; iFPoint < frontPointsPerFibers[iBundle][iFiber].size(); iFPoint++){
      ERNeuRadFiberPoint* FPoint = frontPointsPerFibers[iBundle][iFiber][iFPoint];
      pmtFSignal->AddFiberPoint(FPoint);
      pmtFSignal->AddLink(FairLink("NeuRadFiberPoint",FPoint->Index()));
    }
    pmtFSignal->Generate();
    /*
    if (pmtFSignal->Exist()){
      vector<Double_t> intersections = pmtFSignal->GetIntersections(fDiscriminatorThreshold);
      for (Int_t iInter = 0; iInter < intersections.size(); iInter+=2){
        AddDigi(intersections[iInter], intersections[iInter+1],0., 0.,0.,0.,iFiber);
      }
    }
    */
  }
  
  if (backPointsPerFibers[iBundle][iFiber].size() > 0){
    ERNeuRadPMTSignalD* pmtBSignal =  (ERNeuRadPMTSignalD*)AddPMTSignal(iBundle, iFiber,backPointsPerFibers[iBundle][iFiber].size());
    for(Int_t iFPoint = 0; iFPoint < backPointsPerFibers[iBundle][iFiber].size(); iFPoint++){
      ERNeuRadFiberPoint* FPoint = backPointsPerFibers[iBundle][iFiber][iFPoint];
      pmtBSignal->AddFiberPoint(FPoint);
      pmtBSignal->AddLink(FairLink("NeuRadFiberPoint",FPoint->Index()));
    }
    pmtBSignal->Generate();
    /*
    if (pmtBSignal->Exist()){
      vector<Double_t> intersections = pmtBSignal->GetIntersections(fDiscriminatorThreshold);
      for (Int_t iInter = 0; iInter < intersections.size(); iInter+=2){
        AddDigi(intersections[iInter], intersections[iInter+1],0., 0.,0.,0.,iFiber);
      }
    }
    */
  }
}

ERNeuRadPMTSignal* ERNeuRadDigitizerD::AddPMTSignal(Int_t iBundle, Int_t iFiber, Int_t fpoints_count){
  ERNeuRadPMTSignal *pmtSignal = new((*fNeuRadPMTSignal)[PMTSignalCount()])
              ERNeuRadPMTSignalD(iBundle, iFiber,fpoints_count);
  return  pmtSignal;
}

ClassImp(ERNeuRadDigitizerD)
