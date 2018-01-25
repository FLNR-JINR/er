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
  fSetup(NULL),
  fCsILC(1.),
  fCsIEdepErrorA(0.),
  fCsIEdepErrorB(0.),
  fCsIEdepErrorC(0.),
  fCsITimeErrorA(0.),
  fLaBrLC(1.),
  fLaBrEdepErrorA(0.),
  fLaBrEdepErrorB(0.),
  fLaBrEdepErrorC(0.),
  fLaBrTimeErrorA(0.)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERGadastDigitizer::ERGadastDigitizer(Int_t verbose)
  : FairTask("ER Gadast Digitization scheme ", verbose),
  fSetup(NULL),
  fCsILC(1.),
  fCsIEdepErrorA(0.),
  fCsIEdepErrorB(0.),
  fCsIEdepErrorC(0.),
  fCsITimeErrorA(0.),
  fLaBrLC(1.),
  fLaBrEdepErrorA(0.),
  fLaBrEdepErrorB(0.),
  fLaBrEdepErrorC(0.),
  fLaBrTimeErrorA(0.)
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

  for (const auto &itWall : pointsCsI){
    for (const auto &itBlock : itWall.second){
      for (const auto &itCell : itBlock.second){
        Float_t edep = 0; // sum edep in cell
        Float_t time = std::numeric_limits<float>::max(); // first time in cell
        for (const auto iPoint : itCell.second){
          ERGadastCsIPoint* point = (ERGadastCsIPoint*)fGadastCsIPoints->At(iPoint);
          edep += point->GetEnergyLoss();
          if (point->GetTime() < time)
            time = point->GetTime();
        }
        Float_t edepSigma = sqrt(pow(fCsIEdepErrorA,2) + pow(fCsIEdepErrorB*TMath::Sqrt(edep/1000.),2) + pow(fCsIEdepErrorC*edep,2));
        edep = gRandom->Gaus(fCsILC*edep, edepSigma);

        Float_t timeSigma = TMath::Sqrt(fCsITimeErrorA/edep);
        time = gRandom->Gaus(time, timeSigma);

        AddCsIDigi(edep,itWall.first,itBlock.first,itCell.first);
      }
    }
  }

  for (const auto &itCell : pointsLaBr){
    Float_t edep = 0; // sum edep in cell
    Float_t time = std::numeric_limits<float>::max(); // first time in cell
    for (const auto iPoint : itCell.second){
      ERGadastLaBrPoint* point = (ERGadastLaBrPoint*)fGadastLaBrPoints->At(iPoint);
      edep += point->GetEnergyLoss();
      if (point->GetTime() < time)
        time = point->GetTime();
    }
    Float_t edepSigma = sqrt(pow(fLaBrEdepErrorA,2) + pow(fLaBrEdepErrorB*TMath::Sqrt(edep/1000.),2) + pow(fLaBrEdepErrorC*edep,2));
    edep = gRandom->Gaus(fLaBrLC*edep, edepSigma);

    Float_t timeSigma = TMath::Sqrt(fLaBrTimeErrorA/edep);
    time = gRandom->Gaus(time, timeSigma);

    AddLaBrDigi(edep,itCell.first);
  }
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
