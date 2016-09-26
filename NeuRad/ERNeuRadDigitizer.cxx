
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cmath>

using std::sort;
using std::vector;
using std::cerr;
using std::endl;

#include "TGeoManager.h"
#include "TClonesArray.h"
#include "TVector3.h"
#include "TMath.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include<iostream>
#include "FairEventHeader.h"

#include "ERNeuRadPoint.h"
#include "ERNeuRadStep.h"
#include "ERNeuRadDigitizer.h"
#include "ERNeuRadPMTSignal.h"


const Double_t ERNeuRadDigitizer::cSciFiLightYield= 8000.; // [photons/MeV]
const Double_t ERNeuRadDigitizer::cSpeedOfLight = 0.299792458e2;  //[cm/ns]
const Double_t ERNeuRadDigitizer::cMaterialSpeedOfLight = ERNeuRadDigitizer::cSpeedOfLight/1.58;//[cm/ns]
const Int_t    ERNeuRadDigitizer::cErrorPointsInModuleCount = 1000;
const Double_t ERNeuRadDigitizer::cLightFractionInTotalIntReflection = 0.04;
//доля света захватываемая файбером в полное внутренне отражение в каждую сторону.
const Double_t ERNeuRadDigitizer::cPMTDelay=6.;     //[ns] (H8500)
const Double_t ERNeuRadDigitizer::cPMTJitter = 0.4/2.36; //[ns] (H8500)
const Double_t ERNeuRadDigitizer::cScincilationTau = 3.2; //[ns]
const Double_t ERNeuRadDigitizer::cScincilationDT = 0.05;  //[ns]
const Double_t ERNeuRadDigitizer::cMaxPointLength = 4.; //[cm]

// ----------------------------------------------------------------------------
ERNeuRadDigitizer::ERNeuRadDigitizer()
  : FairTask("ER NeuRad Digitization scheme"),
  fPMTJitter(cPMTJitter),
  fPMTDelay(cPMTDelay),
  fScincilationTau(cScincilationTau),
  fScincilationDT(cScincilationDT),
  fFiberPointsCreatingTime(0),
  fPMTSignalCreatingTime(0),
  fOnePEInteg(4.8),
  fDigiPar(NULL),
  fNeuRadSetup(NULL),
  fNeuRadPoints(NULL),
  fNeuRadFirstStep(NULL),
  fNeuRadFiberPoint(NULL),
  fNeuRadPMTSignal(NULL),
  fNeuRadDigi(NULL),
  fHPECountF(NULL),
  fHPECountB(NULL),
  fCurBundleDigis(NULL),
  fRand(NULL),
  fHAmplitudesB(NULL),
  fSumAmplitudeB(0)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadDigitizer::ERNeuRadDigitizer(Int_t verbose)
  : FairTask("ER NeuRad Digitization scheme ", verbose),
  fPMTJitter(cPMTJitter),
  fPMTDelay(cPMTDelay),
  fScincilationTau(cScincilationTau),
  fScincilationDT(cScincilationDT),
  fFiberPointsCreatingTime(0),
  fPMTSignalCreatingTime(0),
  fOnePEInteg(4.8),
  fDigiPar(NULL),
  fNeuRadSetup(NULL),
  fNeuRadPoints(NULL),
  fNeuRadFirstStep(NULL),
  fNeuRadFiberPoint(NULL),
  fNeuRadPMTSignal(NULL),
  fNeuRadDigi(NULL),
  fHPECountF(NULL),
  fHPECountB(NULL),
  fCurBundleDigis(NULL),
  fRand(NULL),
  fHAmplitudesB(NULL),
  fSumAmplitudeB(0)
{
  fFpeCount = 0;
  fBpeCount = 0;
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadDigitizer::~ERNeuRadDigitizer()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERNeuRadDigitizer::SetParContainers()
{
  // Get run and runtime database
  FairRunAna* run = FairRunAna::Instance();
  if ( ! run ) Fatal("SetParContainers", "No analysis run");

  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  if ( ! rtdb ) Fatal("SetParContainers", "No runtime database");

  fDigiPar = (ERNeuRadDigiPar*)
             (rtdb->getContainer("ERNeuRadDigiPar"));
  if ( fVerbose && fDigiPar ) {
    std::cout << "ERNeuRadDigitizer::SetParContainers() "<< std::endl;
    std::cout << "ERNeuRadDigiPar initialized! "<< std::endl;
  }
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus ERNeuRadDigitizer::Init()
{
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
  fNeuRadPMTSignal = new TClonesArray("ERNeuRadPMTSignal", 1000);
  std::cout << "ERNeuRadDigitizer: Use a ERNeuRadPMTSignal type of signal!" << endl;
  ioman->Register("NeuRadPMTSignal", "Signal after PMT", fNeuRadPMTSignal, kTRUE);
  fNeuRadDigi = new TClonesArray("ERNeuRadDigi",1000);
  ioman->Register("NeuRadDigi", "Digital response in NeuRad", fNeuRadDigi, kTRUE);
  fCurBundleDigis = new TClonesArray("ERNeuRadDigi",1000);
  fRand = new TRandom3();
  
  fNeuRadSetup = ERNeuRadSetup::Instance();
  fNeuRadSetup->Print();

  fHPECountF = new TH1F("fHPECountF", "PE count front",1000.,0.,1000.);
  fHPECountF->GetXaxis()->SetTitle("pe count");
  fHPECountB = new TH1F("fHPECountB", "PE count back",1000.,0.,1000.);
  fHPECountB->GetXaxis()->SetTitle("pe count");

  fHAmplitudesB = new TH1F("fHAmplitudesB", "Sum PE Amplitude",1000.,0.,1000.);
  
  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERNeuRadDigitizer::Exec(Option_t* opt)
{
  fFpeCount = 0;
  fBpeCount  = 0;
  fSumAmplitudeB = 0;
  Int_t iEvent =
			FairRunAna::Instance()->GetEventHeader()->GetMCEntryNumber();
  std::cout << "Event " << iEvent << std::endl;
  // Reset entries in output arrays
  Reset();
  Int_t nofFibers = fNeuRadSetup->NofFibers();
  Int_t nofBundles = fNeuRadSetup->NofBundles();
  //выделяем память под хранение поинтов по файберам
  vector<ERNeuRadFiberPoint* >** frontPointsPerFibers = new vector<ERNeuRadFiberPoint*>* [nofBundles];
  vector<ERNeuRadFiberPoint* >** backPointsPerFibers = new vector<ERNeuRadFiberPoint*>*  [nofBundles];
  for (Int_t i = 0; i<nofBundles; i++){
    frontPointsPerFibers[i] = new vector<ERNeuRadFiberPoint*>  [nofFibers];
    backPointsPerFibers[i] = new vector<ERNeuRadFiberPoint*>  [nofFibers];
  }
  
  Int_t points_count = fNeuRadPoints->GetEntries();
  //Формируем промежуточные сущности FiberPoints
  fFiberPointsCreatingTimer.Start();
  for (Int_t iPoint=0; iPoint < points_count; iPoint++) { // цикл
    ERNeuRadPoint *point = (ERNeuRadPoint*) fNeuRadPoints->At(iPoint);
    FiberPointsCreating(iPoint,point,frontPointsPerFibers,backPointsPerFibers);
  }
  fFiberPointsCreatingTimer.Stop();
  fFiberPointsCreatingTime += fFiberPointsCreatingTimer.RealTime();
  //Формируем сигналы на ФЭУ и digi
  fPMTSignalCreatingTimer.Start();
  for (Int_t iBundle = 0; iBundle < nofBundles; iBundle++){
    //Создаем signals and digi во временной коллекции и накапливаем для текущего бандла суммарный заряд с обеих сторон
    Float_t sumFrontQDC = 0, sumBackQDC = 0;
    for (Int_t iFiber = 0; iFiber < nofFibers; iFiber++) {
      PMTSignalsAndDigiCreating(iBundle, iFiber, frontPointsPerFibers,backPointsPerFibers, sumFrontQDC, sumBackQDC);
    }
    if (fCurBundleDigis->GetEntriesFast() > 0)
        StoreCurBundle(); //@todo
    ClearCurBundle();
  }
  fPMTSignalCreatingTimer.Stop();
  fPMTSignalCreatingTime += fPMTSignalCreatingTimer.RealTime();
  
  //освобождаем память
  for (Int_t i = 0; i<nofBundles; i++){
    delete [] frontPointsPerFibers[i];
    delete [] backPointsPerFibers[i];
  }
  delete [] frontPointsPerFibers;
  delete [] backPointsPerFibers;

  fHPECountF->Fill(fFpeCount);
  fHPECountB->Fill(fBpeCount);
  fHAmplitudesB->Fill(fSumAmplitudeB);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERNeuRadDigitizer::FiberPointsCreating(Int_t i_point, ERNeuRadPoint *point,
                        std::vector<ERNeuRadFiberPoint* >** frontPointsPerFibers,
                        std::vector<ERNeuRadFiberPoint* >** backPointsPerFibers)
{
    Double_t fiber_length = fNeuRadSetup->FiberLength();
    Int_t    point_bundle = point->GetBundleNb();
    Int_t    point_fiber_nb  = point->GetFiberInBundleNb();
    Double_t point_eLoss      =  point->GetEnergyLoss(); //[GeV]
    Double_t point_lightYield = point->GetLightYield();  //[GeV]
    Double_t point_x          = point->GetXIn();
    Double_t point_y          = point->GetYIn();
    Double_t point_z          = point->GetZIn();
    Double_t point_time       = point->GetTimeIn();
    Double_t point_z_in_fiber = point_z + fiber_length/2. - fNeuRadSetup->Z();

    Double_t PMTQuantumEfficiency = fNeuRadSetup->PMTQuantumEfficiency(point_bundle,point_fiber_nb);
    //scintillator light yield - общее число рожденных фотонов
    Double_t photon_count = point_lightYield*1000.*cSciFiLightYield;
    //Моделируем распространнение сигнала на передние ФЭУ
    Double_t k1 = 0.5-cLightFractionInTotalIntReflection;
    Double_t k2 = cLightFractionInTotalIntReflection;
    
    Double_t ffp_photon_count =  photon_count*(k1*exp(-point_z_in_fiber/0.5) + k2*exp(-point_z_in_fiber/200.));

    Double_t remainingPhotoEl = fRand->Poisson(ffp_photon_count*PMTQuantumEfficiency);
    //fHPECountF->Fill(remainingPhotoEl);
    fFpeCount+=remainingPhotoEl;
    if (remainingPhotoEl > 1){
      for(Int_t iOnePESignal=0;iOnePESignal<(Int_t)remainingPhotoEl;iOnePESignal++){
        //Прогнозируем времена их появления в ФЭУ, через решение обратной задачи для экспоненциального распределения
        Double_t ffp_lytime = point_time + (-1)*fScincilationTau*TMath::Log(1-fRand->Uniform());
        Double_t ffp_cathode_time = ffp_lytime + (point_z_in_fiber)/cMaterialSpeedOfLight;
         if (fNeuRadSetup->UseCrosstalks())
          point_fiber_nb = Crosstalks(point_bundle,point_fiber_nb);
        Double_t PMTGain = fNeuRadSetup->PMTGain(point_bundle,point_fiber_nb);
        Double_t PMTSigma = fNeuRadSetup->PMTSigma(point_bundle,point_fiber_nb);
        Double_t ffp_amplitude = TMath::Abs(fRand->Gaus(PMTGain, PMTSigma));
        Double_t ffp_anode_time = ffp_cathode_time + (Double_t)fRand->Gaus(fPMTDelay, fPMTJitter);
        ERNeuRadFiberPoint* ffPoint = AddFiberPoint(i_point, 0, ffp_lytime - point_time, ffp_cathode_time, ffp_anode_time, ffp_photon_count,
                            1, ffp_amplitude, 1);
        
        frontPointsPerFibers[point_bundle][point_fiber_nb].push_back(ffPoint);
      }
    }
 
    Double_t bfp_photon_count =  photon_count*(k1*exp(-(fiber_length-point_z_in_fiber)/0.5) 
                                                   + k2*exp(-(fiber_length-point_z_in_fiber)/200.));
    
    remainingPhotoEl = fRand->Poisson(bfp_photon_count*PMTQuantumEfficiency);
    //fHPECountB->Fill(remainingPhotoEl);
    fBpeCount+=remainingPhotoEl;
    if (remainingPhotoEl > 1){
      for(Int_t iOnePESignal=0;iOnePESignal<(Int_t)remainingPhotoEl;iOnePESignal++){
        //Прогнозируем времена их появления в ФЭУ, через решение обратной задачи для экспоненциального распределения
        Double_t bfp_lytime = point_time + (-1)*fScincilationTau*TMath::Log(1-fRand->Uniform());
        Double_t bfp_cathode_time = bfp_lytime + (point_z_in_fiber)/cMaterialSpeedOfLight;
        if (fNeuRadSetup->UseCrosstalks())
          point_fiber_nb = Crosstalks(point_bundle,point_fiber_nb);
        Double_t PMTGain = fNeuRadSetup->PMTGain(point_bundle,point_fiber_nb);
        Double_t PMTSigma = fNeuRadSetup->PMTSigma(point_bundle,point_fiber_nb);
        Double_t bfp_amplitude = TMath::Abs(fRand->Gaus(PMTGain, PMTSigma));
        Double_t bfp_anode_time = bfp_cathode_time + (Double_t)fRand->Gaus(fPMTDelay, fPMTJitter);
        ERNeuRadFiberPoint* bfPoint = AddFiberPoint(i_point, 1, bfp_lytime - point_time, bfp_cathode_time, bfp_anode_time, bfp_photon_count,
                            1, bfp_amplitude, 1);
        fSumAmplitudeB+=bfp_amplitude;
        backPointsPerFibers[point_bundle][point_fiber_nb].push_back(bfPoint);
      }
    }
}

//----------------------------------------------------------------------------
Int_t ERNeuRadDigitizer::Crosstalks(Int_t iBundle, Int_t iFiber){
  Int_t newFiber = iFiber;
  TArrayF crosstalks;
  fNeuRadSetup->PMTCrosstalks(iFiber, crosstalks);
  Float_t prob = gRandom->Uniform();
  Float_t curProb = 0;
  Int_t csI = -1;
  Int_t csJ = -1;
  for (Int_t i = 0; i < 3; i++){
    for (Int_t j = 0; j < 3; j++){
      if (crosstalks[i*3+j] == 0)
        continue;
      
      curProb += crosstalks[i*3+j];

      if (curProb > prob){
        csI = i;
        csJ = j;
        break;
      }
    }
    if (csI != -1)
      break;
  }
  if (csI == 0){
    newFiber-= fNeuRadSetup->RowNofFibers();
  }
  if (csI == 2){
    newFiber+=fNeuRadSetup->RowNofFibers();
  }
  if (csJ == 0){
    newFiber-=1;
  }
  if (csJ == 2){
    newFiber+=1;
  }
  return newFiber;
}

//----------------------------------------------------------------------------
void ERNeuRadDigitizer::Reset()
{
  if (fNeuRadDigi) {
    fNeuRadDigi->Delete();
  }
  if (fNeuRadFiberPoint){
    fNeuRadFiberPoint->Delete();
  }
  if (fNeuRadPMTSignal){
    fNeuRadPMTSignal->Delete();
  }
}
// ----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void ERNeuRadDigitizer::PMTSignalsAndDigiCreating(Int_t iBundle, Int_t iFiber,
                                std::vector<ERNeuRadFiberPoint* >** frontPointsPerFibers,
                                std::vector<ERNeuRadFiberPoint* >** backPointsPerFibers,
                                Float_t& sumFrontQDC, Float_t& sumBackQDC)
{
  if( frontPointsPerFibers[iBundle][iFiber].size() > 0){
    //fFpeCount+=frontPointsPerFibers[iBundle][iFiber].size();
    ERNeuRadPMTSignal* pmtFSignal = AddPMTSignal(iBundle, iFiber,frontPointsPerFibers[iBundle][iFiber].size(),0);
    for(Int_t iFPoint = 0; iFPoint < frontPointsPerFibers[iBundle][iFiber].size(); iFPoint++){
      ERNeuRadFiberPoint* FPoint = frontPointsPerFibers[iBundle][iFiber][iFPoint];
      pmtFSignal->AddFiberPoint(FPoint);
      //pmtFSignal->AddLink(FairLink("NeuRadFiberPoint",FPoint->Index()));
    }
    pmtFSignal->Generate();
    if (pmtFSignal->Exist()){
        Float_t  frontTDC = pmtFSignal->GetThresholdTime(fPixelThreshold);
        if (frontTDC < 0.)//если не  прошел порог fPixelThreshold
          frontTDC = pmtFSignal->GetStartTime();
        Float_t backTDC = pmtFSignal->GetFinishTime();
        Float_t QDC = pmtFSignal->GetInteg(frontTDC,backTDC);
        sumFrontQDC += QDC;
        AddTempDigi(frontTDC, backTDC,backTDC-frontTDC, pmtFSignal->PECount(),iBundle,iFiber,0);
    }
  }

  if (backPointsPerFibers[iBundle][iFiber].size() > 0){
    //fBpeCount += backPointsPerFibers[iBundle][iFiber].size();
    ERNeuRadPMTSignal* pmtBSignal =  AddPMTSignal(iBundle, iFiber,backPointsPerFibers[iBundle][iFiber].size(),1);
    for(Int_t iFPoint = 0; iFPoint < backPointsPerFibers[iBundle][iFiber].size(); iFPoint++){
      ERNeuRadFiberPoint* FPoint = backPointsPerFibers[iBundle][iFiber][iFPoint];
      pmtBSignal->AddFiberPoint(FPoint);
      //pmtBSignal->AddLink(FairLink("NeuRadFiberPoint",FPoint->Index()));
    }
    pmtBSignal->Generate();
    if (pmtBSignal->Exist()){
        Float_t frontTDC = pmtBSignal->GetThresholdTime(fPixelThreshold);
        if (frontTDC < 0.) //если прошел порог fPixelThreshol
          frontTDC = pmtBSignal->GetStartTime();
        Float_t backTDC = pmtBSignal->GetFinishTime();
        Float_t QDC = pmtBSignal->GetInteg(frontTDC,backTDC);
        sumBackQDC+= QDC;
        AddTempDigi(frontTDC, backTDC,backTDC-frontTDC, pmtBSignal->PECount(),iBundle,iFiber,1);
    }
  }
}
// ----------------------------------------------------------------------------
void ERNeuRadDigitizer::Finish()
{   
  fHPECountF->Write();
  fHPECountB->Write();
  fHAmplitudesB->Write();
  std::cout << "========== Finish of ERNeuRadDigitizer =================="<< std::endl;
  std::cout << "=====  Time on FiberPoints creating : " <<  fFiberPointsCreatingTime << " s" << std::endl;
  std::cout << "=====  Time on PMT signal creating : " <<  fPMTSignalCreatingTime << " s" << std::endl;
  std::cout << "=========================================================" << std::endl;
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadDigi* ERNeuRadDigitizer::AddTempDigi(Double_t frontTDC, Double_t backTDC, Double_t TDC, 
                                            Double_t QDC, Int_t bundle, Int_t fiber, Int_t side)
{
  ERNeuRadDigi *digi = new((*fCurBundleDigis)[fCurBundleDigis->GetEntriesFast()])
							ERNeuRadDigi(fCurBundleDigis->GetEntriesFast(), frontTDC, backTDC, TDC, QDC, bundle, fiber, side);
  return digi;
}
// ----------------------------------------------------------------------------
ERNeuRadDigi* ERNeuRadDigitizer::AddDigi(ERNeuRadDigi* digi)
{
  ERNeuRadDigi *new_digi = new((*fNeuRadDigi)[fNeuRadDigi->GetEntriesFast()])
              ERNeuRadDigi(fNeuRadDigi->GetEntriesFast(), digi->FrontTDC(), digi->BackTDC(), digi->TDC(),
                               digi->QDC(), digi->BundleIndex(), digi->FiberIndex(), digi->Side());
  return new_digi;
}
// ----------------------------------------------------------------------------
ERNeuRadPMTSignal* ERNeuRadDigitizer::AddPMTSignal(Int_t iBundle, Int_t iFiber, Int_t fpoints_count, Int_t side){
  ERNeuRadPMTSignal *pmtSignal = new((*fNeuRadPMTSignal)[PMTSignalCount()])
              ERNeuRadPMTSignal(iBundle, iFiber,fpoints_count, side);
  return  pmtSignal;
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadFiberPoint* ERNeuRadDigitizer::AddFiberPoint(Int_t i_point, Int_t side, Double_t lytime, Double_t cathode_time, Double_t anode_time, 
									Int_t photon_count, Int_t photoel_count, 
									Double_t amplitude, Int_t onePE){
  ERNeuRadFiberPoint *fp = new ((*fNeuRadFiberPoint)[FiberPointCount()])
								ERNeuRadFiberPoint(FiberPointCount(),side, lytime, cathode_time, anode_time, photon_count, 
                                    photoel_count, amplitude, onePE);
  fp->AddLink(FairLink("NeuRadPoint",i_point));
  return fp;
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
Int_t ERNeuRadDigitizer::FiberPointCount()  const {
  return fNeuRadFiberPoint->GetEntriesFast();
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
Int_t ERNeuRadDigitizer::PMTSignalCount()   const {
  return fNeuRadPMTSignal->GetEntriesFast();
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
Int_t ERNeuRadDigitizer::DigiCount()        const {
  return fNeuRadDigi->GetEntriesFast();
}
//-----------------------------------------------------------------------------
void ERNeuRadDigitizer::StoreCurBundle(){
  for (Int_t i = 0; i < fCurBundleDigis->GetEntriesFast(); i++){
    ERNeuRadDigi* digi = (ERNeuRadDigi*)fCurBundleDigis->At(i);
    AddDigi(digi);
  }
}
//-----------------------------------------------------------------------------
void ERNeuRadDigitizer::ClearCurBundle(){
  fCurBundleDigis->Delete();
}
//-----------------------------------------------------------------------------
ClassImp(ERNeuRadDigitizer)
