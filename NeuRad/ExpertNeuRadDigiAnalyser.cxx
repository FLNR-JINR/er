#include "TH1F.h"
#include "TClonesArray.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"


#include "ExpertNeuRadPoint.h"
#include "ExpertNeuRadStep.h"
#include "ExpertNeuRadDigi.h"
#include "ExpertNeuRadDigiAnalyser.h"

// ----------------------------------------------------------------------------
ExpertNeuRadDigiAnalyser::ExpertNeuRadDigiAnalyser()
  : FairTask("Expert NeuRad Digi analyse")
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ExpertNeuRadDigiAnalyser::ExpertNeuRadDigiAnalyser(Int_t verbose)
  : FairTask("Expert NeuRad Digi analyse")
{  
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ExpertNeuRadDigiAnalyser::~ExpertNeuRadDigiAnalyser()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ExpertNeuRadDigiAnalyser::SetParContainers()
{
  // Get run and runtime database
  FairRunAna* run = FairRunAna::Instance();
  if ( ! run ) Fatal("SetParContainers", "No analysis run");

  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  if ( ! rtdb ) Fatal("SetParContainers", "No runtime database");

  if ( fVerbose /*&& fLandDigiPar*/ ) {
    LOG(INFO) << "ExpertNeuRadDigiAnalyser::SetParContainers() "<< FairLogger::endl;
  }
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus ExpertNeuRadDigiAnalyser::Init()
{
    // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  
  fNeuRadPoints = (TClonesArray*) ioman->GetObject("NeuRadPoint");
  fNeuRadFirstStep = (TClonesArray*) ioman->GetObject("NeuRadFirstStep");
  fNeuRadDigi = (TClonesArray*) ioman->GetObject("NeuRadDigi");
  //todo check
  fhFrontQDCcmXMinusFirst = new TH1F("FrontQDCcmXMinusFirst","FrontQDCcmXMinusFirst, [digi]", 40., 0., 8.);
  fhFrontQDCcmYMinusFirst = new TH1F("FrontQDCcmYMinusFirst","FrontQDCcmYMinusFirst, [digi]", 40., 0., 8.);
  fhBackQDCcmXMinusFirst  = new TH1F("BackQDCcmXMinusFirst", "BackQDCcmXMinusFirst, [digi]", 40., 0., 8.);
  fhBackQDCcmYMinusFirst  = new TH1F("BackQDCcmYMinusFirst", "BackQDCcmYMinusFirst, [digi]", 40., 0., 8.);
  
  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ExpertNeuRadDigiAnalyser::Exec(Option_t* opt)
{
  if (fNeuRadDigi->GetEntriesFast() > 0){
    Double_t frontQDC_cm_x  = 0.;
    Double_t backQDC_cm_x   = 0.;
    Double_t frontQDC_cm_y  = 0.;
    Double_t backQDC_cm_y   = 0.;
    Double_t sumFrontQDC    = 0.;
    Double_t sumBackQDC     = 0.;
    
    for (Int_t i_digi =0; i_digi < fNeuRadDigi->GetEntriesFast(); i_digi++){
      ExpertNeuRadDigi* digi = (ExpertNeuRadDigi*)fNeuRadDigi->At(i_digi);
      Int_t digi_fiber_nr = digi->GetFiberNr();
      digi_fiber_nr+=1;
      Double_t digi_frontQDC = digi->GetFrontQDC();
      Double_t digi_backQDC = digi->GetBackQDC();
      Int_t x_fiber_nr = digi_fiber_nr%8; //@todo Get from Parameter class
      Int_t y_fiber_nr = digi_fiber_nr/8 + 1;
      frontQDC_cm_x += x_fiber_nr*digi_frontQDC;
      frontQDC_cm_y += y_fiber_nr*digi_frontQDC;
      backQDC_cm_x  += x_fiber_nr*digi_backQDC;
      backQDC_cm_y  += y_fiber_nr*digi_backQDC;
      sumFrontQDC   += digi_frontQDC;
      sumBackQDC    += digi_backQDC;
    }
    frontQDC_cm_x /= sumFrontQDC;
    frontQDC_cm_y /= sumFrontQDC;
    backQDC_cm_x  /= sumBackQDC;
    backQDC_cm_y  /= sumBackQDC;  
    
    //firstStep in Neurad
    ExpertNeuRadStep* firstStep = (ExpertNeuRadStep*) fNeuRadFirstStep->At(0);
    Int_t fiber_nr = firstStep->GetFiberInBundleNb();
    fiber_nr+=1;
    Int_t x_fiber_nr = fiber_nr%8; //@todo Get from Parameter class
    Int_t y_fiber_nr = fiber_nr/8 + 1;
   
    
    fhFrontQDCcmXMinusFirst->Fill(TMath::Abs(frontQDC_cm_x - x_fiber_nr));
    fhFrontQDCcmYMinusFirst->Fill(TMath::Abs(frontQDC_cm_y - y_fiber_nr));
    fhBackQDCcmXMinusFirst->Fill(TMath::Abs(backQDC_cm_x - x_fiber_nr));
    fhBackQDCcmYMinusFirst->Fill(TMath::Abs(backQDC_cm_y - y_fiber_nr));
  }
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ExpertNeuRadDigiAnalyser::Reset()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ExpertNeuRadDigiAnalyser::Finish()
{  
  fhFrontQDCcmXMinusFirst->Write(); 
  fhFrontQDCcmYMinusFirst->Write();
  fhBackQDCcmXMinusFirst->Write(); 
  fhBackQDCcmYMinusFirst->Write();
}
// ----------------------------------------------------------------------------

ClassImp(ExpertNeuRadDigiAnalyser)