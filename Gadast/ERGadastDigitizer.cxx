#include "ERGadastDigitizer.h"

#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairEventHeader.h"

#include "ERGadastCsIPoint.h"
#include "ERGadastLaBrPoint.h"

// ----------------------------------------------------------------------------
ERGadastDigitizer::ERGadastDigitizer()
  : FairTask("ER Gadast Digitization scheme"),
  fRand(NULL)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERGadastDigitizer::ERGadastDigitizer(Int_t verbose)
  : FairTask("ER Gadast Digitization scheme ", verbose),
  fRand(NULL)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERGadastDigitizer::~ERGadastDigitizer()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERGadastDigitizer::SetParContainers()
{
  // Get run and runtime database
  FairRunAna* run = FairRunAna::Instance();
  if ( ! run ) Fatal("SetParContainers", "No analysis run");

  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  if ( ! rtdb ) Fatal("SetParContainers", "No runtime database");

  fDigiPar = (ERGadastDigiPar*)
             (rtdb->getContainer("ERGadastDigiPar"));
  if ( fVerbose && fDigiPar ) {
    std::cout << "ERGadastDigitizer::SetParContainers() "<< std::endl;
    std::cout << "ERGadastDigiPar initialized! "<< std::endl;
  }
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus ERGadastDigitizer::Init()
{
  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  
  fGadastCsIPoints = (TClonesArray*) ioman->GetObject("GadastCsIPoint");
  fGadastLaBrPoints = (TClonesArray*) ioman->GetObject("GadastLaBrPoint");
  
  // Register output array NeuRadFiberPoint and NeuRadDigi
  fGadastDigi = new TClonesArray("ERGadastDigi",1000);
  ioman->Register("GadastDigi", "Digital response in Gadast", fGadastDigi, kTRUE);
  fRand = new TRandom3();
  
  //fNeuRadSetup = ERNeuRadSetup::Instance();
  //fNeuRadSetup->Print();
  
  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERGadastDigitizer::Exec(Option_t* opt)
{
  Int_t iEvent =
			FairRunAna::Instance()->GetEventHeader()->GetMCEntryNumber();
  std::cout << "Event " << iEvent << std::endl;
  // Reset entries in output arrays
  Reset();

  for (Int_t iPoint = 0; iPoint < fGadastCsIPoints->GetEntriesFast(); iPoint++){
    ERGadastCsIPoint* point = (ERGadastCsIPoint*)fGadastCsIPoints->At(iPoint);
  }

  for (Int_t iPoint = 0; iPoint < fGadastLaBrPoints->GetEntriesFast(); iPoint++){
    ERGadastLaBrPoint* point = (ERGadastLaBrPoint*)fGadastLaBrPoints->At(iPoint);
  }
  
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERGadastDigitizer::Reset()
{
  if (fGadastDigi) {
    fGadastDigi->Delete();
  }
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERGadastDigitizer::Finish()
{ 
  std::cout << "========== Finish of ERGadastDigitizer =================="<< std::endl;
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERGadastDigi* ERGadastDigitizer::AddDigi(Float_t Edep)
{
  ERGadastDigi *digi = new((*fGadastDigi)[fGadastDigi->GetEntriesFast()])
							ERGadastDigi(fGadastDigi->GetEntriesFast(), Edep);
  return digi;
}
//-----------------------------------------------------------------------------
ClassImp(ERGadastDigitizer)
