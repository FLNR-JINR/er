#include "TH1F.h"
#include "TClonesArray.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"


#include "ExpertNeuRadPoint.h"
#include "ExpertNeuRadStep.h"
#include "ExpertNeuRadPointAnalyser.h"

const Double_t ExpertNeuRadPointAnalyser::fHistoELossThreshold = 100.;

// ----------------------------------------------------------------------------
ExpertNeuRadPointAnalyser::ExpertNeuRadPointAnalyser()
  : FairTask("Expert NeuRad Point analyse")
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ExpertNeuRadPointAnalyser::ExpertNeuRadPointAnalyser(Int_t verbose)
  : FairTask("Expert NeuRad Point analyse")
{  
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ExpertNeuRadPointAnalyser::~ExpertNeuRadPointAnalyser()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ExpertNeuRadPointAnalyser::SetParContainers()
{
  // Get run and runtime database
  FairRunAna* run = FairRunAna::Instance();
  if ( ! run ) Fatal("SetParContainers", "No analysis run");

  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  if ( ! rtdb ) Fatal("SetParContainers", "No runtime database");

  if ( fVerbose /*&& fLandDigiPar*/ ) {
    LOG(INFO) << "ExpertNeuRadPointAnalyser::SetParContainers() "<< FairLogger::endl;
  }
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus ExpertNeuRadPointAnalyser::Init()
{
    // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  
  fNeuRadPoints = (TClonesArray*) ioman->GetObject("NeuRadPoint");
  fNeuRadFirstStep = (TClonesArray*) ioman->GetObject("NeuRadFirstStep");
  //todo check
  
  fhFibersPerSumEnergyLoss =  
        new TH1F("FibersPerSumEnergyLoss", "Fibers per sum of energy loss", 100000, 0., 100000.);
  fhSumEnergyLossOfAllFibers =  
          new TH1F("SumEnergyLossOfAllFibers", "Sum Of ELoss in all fibers", 100000, 0., 100000.);
  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ExpertNeuRadPointAnalyser::Exec(Option_t* opt)
{
  //Fill FibersPerSumEnergyLoss histo
  Int_t NPoints = fNeuRadPoints->GetEntriesFast();
  Int_t NFibers = 64.;
  Double_t* SumEloss = new Double_t[NFibers];
  // init 
  for (Int_t i_fiber = 0; i_fiber < NFibers; i_fiber++)
    SumEloss[i_fiber] = 0.;
  
  // Sum energy per fibers
  Double_t sumOfEnergy = 0.;
  for (Int_t i_point = 0; i_point < NPoints; i_point++){
    ExpertNeuRadPoint* point = (ExpertNeuRadPoint*)fNeuRadPoints->At(i_point);
    Int_t point_fiber_nb = point->GetFiberInBundleNb();
    SumEloss[point_fiber_nb] += point->GetEnergyLoss();
    sumOfEnergy+= point->GetEnergyLoss();
  }
  if (sumOfEnergy > fHistoELossThreshold)
    fhSumEnergyLossOfAllFibers->Fill(sumOfEnergy);
  for (Int_t i_fiber = 0; i_fiber < NFibers; i_fiber++){
    if (SumEloss[i_fiber] > fHistoELossThreshold)
      fhFibersPerSumEnergyLoss->Fill(SumEloss[i_fiber]);
  }
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ExpertNeuRadPointAnalyser::Reset()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ExpertNeuRadPointAnalyser::Finish()
{  
  fhSumEnergyLossOfAllFibers->Write(); 
  fhFibersPerSumEnergyLoss->Write();
}
// ----------------------------------------------------------------------------

ClassImp(ExpertNeuRadPointAnalyser)
