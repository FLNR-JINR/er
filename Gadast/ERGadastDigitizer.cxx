#include "ERGadastDigitizer.h"

#include "TVector3.h"

#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairEventHeader.h"

#include "ERGadastCsIPoint.h"
#include "ERGadastLaBrPoint.h"

using namespace std;

// ----------------------------------------------------------------------------
ERGadastDigitizer::ERGadastDigitizer()
  : FairTask("ER Gadast Digitization scheme"),
  fSetup(NULL)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERGadastDigitizer::ERGadastDigitizer(Int_t verbose)
  : FairTask("ER Gadast Digitization scheme ", verbose),
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
  
  // Register output arrays
  fGadastCsIDigi = new TClonesArray("ERGadastCsIDigi",1000);
  fGadastLaBrDigi = new TClonesArray("ERGadastLaBrDigi",1000);
  ioman->Register("GadastCsIDigi", "Digital response in Gadast CsI", fGadastCsIDigi, kTRUE);
  ioman->Register("GadastLaBrDigi", "Digital response in Gadast LaBr", fGadastLaBrDigi, kTRUE);

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

  // Sort points by sensentive volumes
  // Map points by cells: pointsCsI[iWall][iBlock][iCell]
  map<Int_t, map<Int_t, map <Int_t, vector<Int_t> > > > pointsCsI;
  for (Int_t iPoint = 0; iPoint < fGadastCsIPoints->GetEntriesFast(); iPoint++){
    ERGadastCsIPoint* point = (ERGadastCsIPoint*)fGadastCsIPoints->At(iPoint);
    pointsCsI[point->GetWall()][point->GetBlock()][point->GetCell()].push_back(iPoint);
  }

  // Map points by cells: pointsLaBr[iCell]
  map<Int_t, vector<Int_t> > pointsLaBr;
  for (Int_t iPoint = 0; iPoint < fGadastLaBrPoints->GetEntriesFast(); iPoint++){
    ERGadastLaBrPoint* point = (ERGadastLaBrPoint*)fGadastLaBrPoints->At(iPoint);
    pointsLaBr[point->GetCell()].push_back(iPoint);
  }
  /*
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
  */
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERGadastDigitizer::Reset()
{
  if (fGadastCsIDigi) {
    fGadastCsIDigi->Delete();
  }
  if (fGadastLaBrDigi) {
    fGadastLaBrDigi->Delete();
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
ERGadastCsIDigi* ERGadastDigitizer::AddCsIDigi(Float_t Edep,Int_t wall,Int_t block, Int_t cell)
{
  ERGadastCsIDigi *digi = new((*fGadastCsIDigi)[fGadastCsIDigi->GetEntriesFast()])
							ERGadastCsIDigi(fGadastCsIDigi->GetEntriesFast(), Edep, wall, block, cell);
  return digi;
}
// ----------------------------------------------------------------------------
ERGadastLaBrDigi* ERGadastDigitizer::AddLaBrDigi(Float_t Edep, Int_t cell)
{
  ERGadastLaBrDigi *digi = new((*fGadastLaBrDigi)[fGadastLaBrDigi->GetEntriesFast()])
              ERGadastLaBrDigi(fGadastLaBrDigi->GetEntriesFast(), Edep, cell);
  return digi;
}
ClassImp(ERGadastDigitizer)
