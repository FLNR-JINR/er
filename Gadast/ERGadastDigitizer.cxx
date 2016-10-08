#include "ERGadastDigitizer.h"

#include "TVector3.h"

#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairEventHeader.h"

#include "ERGadastCsIPoint.h"
#include "ERGadastLaBrPoint.h"

// ----------------------------------------------------------------------------
ERGadastDigitizer::ERGadastDigitizer()
  : FairTask("ER Gadast Digitization scheme"),
  fHCsIElossInEvent(NULL),
  fHLaBrElossInEvent(NULL),
  fSetup(NULL)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERGadastDigitizer::ERGadastDigitizer(Int_t verbose)
  : FairTask("ER Gadast Digitization scheme ", verbose),
  fHCsIElossInEvent(NULL),
  fHLaBrElossInEvent(NULL),
  fSetup(NULL)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERGadastDigitizer::~ERGadastDigitizer()
{
  delete fSetup;
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

//----------------------------------------------------------------------------
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

  fHCsIElossInEvent = new TH1F("fHCsIElossInEvent", "fHCsIElossInEvent",1000, 0., 0.005);
  fHLaBrElossInEvent = new TH1F("fHLaBrElossInEvent", "fHLaBrElossInEvent",1000, 0., 0.01);

  fSetup = ERGadastSetup::Instance();
  if (!fSetup->Init()){
    std::cerr << "Problems with ERGadastSetup initialization!" << std::endl;
  }
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
    Float_t edep = point->GetEnergyLoss();
    TVector3* pos = new TVector3(point->GetXIn(), point->GetYIn(), point->GetZIn());

    Float_t LC = fSetup->CsILC(pos);
    Float_t a = fSetup->CsIDispA(pos);
    Float_t b = fSetup->CsIDispB(pos);

    Float_t disp = a*a*edep + b*b*edep*edep;
    edep = gRandom->Gaus(edep*LC, disp);
    fCsIElossInEvent += edep;
    AddDigi(edep);
  }

  

  for (Int_t iPoint = 0; iPoint < fGadastLaBrPoints->GetEntriesFast(); iPoint++){
    ERGadastLaBrPoint* point = (ERGadastLaBrPoint*)fGadastLaBrPoints->At(iPoint);
    Float_t edep = point->GetEnergyLoss();

    Float_t LC = 0.8;
    Float_t a = 0.01343;
    Float_t b = 0.004;

    Float_t disp = a*a*edep + b*b*edep*edep;
    edep = gRandom->Gaus(edep*LC, disp);
    fLaBrElossInEvent += edep;
    AddDigi(edep);
  }
  if (fCsIElossInEvent > 0)
    fHCsIElossInEvent->Fill(fCsIElossInEvent);
  if (fLaBrElossInEvent > 0)
    fHLaBrElossInEvent->Fill(fLaBrElossInEvent);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERGadastDigitizer::Reset()
{
  fCsIElossInEvent = 0;
  fLaBrElossInEvent = 0;

  if (fGadastDigi) {
    fGadastDigi->Delete();
  }
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERGadastDigitizer::Finish()
{ 
  fHCsIElossInEvent->Write();
  fHLaBrElossInEvent->Write();
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
