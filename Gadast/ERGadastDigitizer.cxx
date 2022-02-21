#include "ERGadastDigitizer.h"

#include "TVector3.h"
#include "TMath.h"

#include "FairRun.h"
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
  fLaBrTimeErrorA(0.),
  fCsIElossThreshold(0.),
  fLaBrElossThreshold(0.)
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
  fLaBrTimeErrorA(0.),
  fCsIElossThreshold(0.),
  fLaBrElossThreshold(0.)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------

ERGadastDigitizer::~ERGadastDigitizer()
{
  delete fSetup;
}

// ----------------------------------------------------------------------------

void ERGadastDigitizer::SetCsILC(Float_t lc)
{
  fCsILCFun = [lc](BlockAddress, size_t, size_t, size_t) {return lc;};
  fCsILCGrid = [](BlockAddress) {return std::make_tuple(1,1,1);};
}

// ----------------------------------------------------------------------------

void ERGadastDigitizer::SetCsILC(std::map<BlockAddress, float>& lc)
{
  fCsILCFun = [lc](BlockAddress address, size_t, size_t, size_t) {
    return lc.at(address);
  };
  fCsILCGrid = [](BlockAddress) {return std::make_tuple(1,1,1);};
}

// ----------------------------------------------------------------------------

void ERGadastDigitizer::SetCsILC(std::map<BlockAddress, CoefficientMatrix>& lc)
{
  fCsILCFun = [&lc](BlockAddress address, size_t x, size_t y, size_t z) {
    LOG(DEBUG) << "CsILCFun: wall=" << address.first << ", block=" << address.second 
               << ", x cell=" << x << ", y cell=" << y << ", z cell=" << z << FairLogger::endl;
    return lc.at(address).at(x).at(y).at(z);
  };

  fCsILCGrid = [&lc](BlockAddress address) {
    LOG(DEBUG) << "fCsILCGrid: wall=" << address.first << ", block=" << address.second << FairLogger::endl;
    const auto& matrix = lc.at(address);
    const size_t x_size = matrix.size();
    const size_t y_size = matrix.at(0).size();
    const size_t z_size = matrix.at(0).at(0).size();
    return std::make_tuple(x_size, y_size, z_size);
  };
}

// ----------------------------------------------------------------------------

void ERGadastDigitizer::SetCsIEdepError(Float_t a, Float_t b, Float_t c)
{
  fCsILCAFun = [a](BlockAddress, size_t, size_t, size_t) {return a;};
  fCsILCAGrid = [](BlockAddress) {return std::make_tuple(1,1,1);};
  fCsILCBFun = [b](BlockAddress, size_t, size_t, size_t) {return b;};
  fCsILCBGrid = [](BlockAddress) {return std::make_tuple(1,1,1);};
  fCsILCCFun = [c](BlockAddress, size_t, size_t, size_t) {return c;};
  fCsILCCGrid = [](BlockAddress) {return std::make_tuple(1,1,1);};
}

// ----------------------------------------------------------------------------

void ERGadastDigitizer::SetCsIEdepError(std::map<BlockAddress, float>& a, 
                                        std::map<BlockAddress, float>& b,
                                        std::map<BlockAddress, float>& c) {
  fCsILCAFun = [a](BlockAddress address, size_t, size_t, size_t) {
    return a.at(address);
  };
  fCsILCAGrid = [](BlockAddress) {return std::make_tuple(1,1,1);};
  fCsILCBFun = [b](BlockAddress address, size_t, size_t, size_t) {
    return b.at(address);
  };
  fCsILCBGrid = [](BlockAddress) {return std::make_tuple(1,1,1);};
  fCsILCCFun = [c](BlockAddress address, size_t, size_t, size_t) {
    return c.at(address);
  };
  fCsILCCGrid = [](BlockAddress) {return std::make_tuple(1,1,1);};
}

// ----------------------------------------------------------------------------

void ERGadastDigitizer::SetCsIEdepError(std::map<BlockAddress, CoefficientMatrix>& a, 
                                        std::map<BlockAddress, CoefficientMatrix>& b,
                                        std::map<BlockAddress, CoefficientMatrix>& c) {
  fCsILCAFun = [&a](BlockAddress address, size_t x, size_t y, size_t z) {
    LOG(DEBUG) << "fCsILCAFun: wall=" << address.first << ", block=" << address.second 
               << ", x cell=" << x << ", y cell=" << y << ", z cell=" << z << FairLogger::endl;
    return a.at(address).at(x).at(y).at(z);
  };

  fCsILCAGrid = [&a](BlockAddress address) {
    LOG(DEBUG) << "fCsILCAGrid: wall=" << address.first << ", block=" << address.second << FairLogger::endl;
    const auto& matrix = a.at(address);
    const size_t x_size = matrix.size();
    const size_t y_size = matrix.at(0).size();
    const size_t z_size = matrix.at(0).at(0).size();
    return std::make_tuple(x_size, y_size, z_size);
  };

  fCsILCBFun = [&b](BlockAddress address, size_t x, size_t y, size_t z) {
    LOG(DEBUG) << "fCsILCAFun: wall=" << address.first << ", block=" << address.second 
               << ", x cell=" << x << ", y cell=" << y << ", z cell=" << z << FairLogger::endl;
    return b.at(address).at(x).at(y).at(z);
  };

  fCsILCBGrid = [&b](BlockAddress address) {
    LOG(DEBUG) << "fCsILCBGrid: wall=" << address.first << ", block=" << address.second << FairLogger::endl;
    const auto& matrix = b.at(address);
    const size_t x_size = matrix.size();
    const size_t y_size = matrix.at(0).size();
    const size_t z_size = matrix.at(0).at(0).size();
    return std::make_tuple(x_size, y_size, z_size);
  };

  fCsILCCFun = [&c](BlockAddress address, size_t x, size_t y, size_t z) {
    LOG(DEBUG) << "fCsILCAFun: wall=" << address.first << ", block=" << address.second 
               << ", x cell=" << x << ", y cell=" << y << ", z cell=" << z << FairLogger::endl;
    return c.at(address).at(x).at(y).at(z);
  };

  fCsILCCGrid = [&c](BlockAddress address) {
    LOG(DEBUG) << "fCsILCAGrid: wall=" << address.first << ", block=" << address.second << FairLogger::endl;
    const auto& matrix = c.at(address);
    const size_t x_size = matrix.size();
    const size_t y_size = matrix.at(0).size();
    const size_t z_size = matrix.at(0).at(0).size();
    return std::make_tuple(x_size, y_size, z_size);
  };

}

// ----------------------------------------------------------------------------
void ERGadastDigitizer::SetParContainers()
{
  // Get run and runtime database
  FairRun* run = FairRun::Instance();
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
      BlockAddress address = std::make_pair(itWall.first, itBlock.first);
      for (const auto &itCell : itBlock.second){
        Float_t edep = 0; // sum edep in cell
        Float_t edepSigma = 0; // sum edep in cell
        Float_t time = std::numeric_limits<float>::max(); // first time in cell
        for (const auto iPoint : itCell.second){
          ERGadastCsIPoint* point = (ERGadastCsIPoint*)fGadastCsIPoints->At(iPoint);
          if (point->GetTime() < time)
            time = point->GetTime();
          TVector3 pos;
          point->PositionIn(pos);

          size_t x_counts, y_counts, z_counts;
          std::tie(x_counts, y_counts, z_counts) = fCsILCGrid(address);
          size_t x_bin, y_bin, z_bin;
          std::tie(x_bin, y_bin, z_bin) = fSetup->GetCsIMeshElement(&pos, x_counts, y_counts, z_counts);
          edep += fCsILCFun(address, x_bin, y_bin, z_bin) * point->GetEnergyLoss();
          
          std::tie(x_counts, y_counts, z_counts) = fCsILCAGrid(address);
          std::tie(x_bin, y_bin, z_bin) = fSetup->GetCsIMeshElement(&pos, x_counts, y_counts, z_counts);
          const float A = fCsILCAFun(address, x_bin, y_bin, z_bin);

          std::tie(x_counts, y_counts, z_counts) = fCsILCBGrid(address);
          std::tie(x_bin, y_bin, z_bin) = fSetup->GetCsIMeshElement(&pos, x_counts, y_counts, z_counts);
          const float B = fCsILCBFun(address, x_bin, y_bin, z_bin);

          std::tie(x_counts, y_counts, z_counts) = fCsILCCGrid(address);
          std::tie(x_bin, y_bin, z_bin) = fSetup->GetCsIMeshElement(&pos, x_counts, y_counts, z_counts);
          const float C = fCsILCCFun(address, x_bin, y_bin, z_bin);
          
          edepSigma += sqrt(pow(A, 2) + pow(B * TMath::Sqrt(edep), 2) + pow(C * edep, 2));
        }
        
        edep = gRandom->Gaus(edep, edepSigma);
        if (edep < fCsIElossThreshold)
          continue;
        Float_t timeSigma = TMath::Sqrt(fCsITimeErrorA/edep);
        time = gRandom->Gaus(time, timeSigma);

        AddCsIDigi(edep, itWall.first, itBlock.first, itCell.first);
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
    Float_t edepSigma = sqrt(pow(fLaBrEdepErrorA,2) + pow(fLaBrEdepErrorB*TMath::Sqrt(edep),2) + pow(fLaBrEdepErrorC*edep,2));
    edep = gRandom->Gaus(fLaBrLC*edep, edepSigma);
    if (edep < fLaBrElossThreshold)
      continue;
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
