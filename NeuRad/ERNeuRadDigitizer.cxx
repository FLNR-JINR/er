
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
#include "FairLogger.h"
#include "FairEventHeader.h"

#include "ERNeuRadPoint.h"
#include "ERNeuRadStep.h"
#include "ERNeuRadDigitizer.h"


const Double_t ERNeuRadDigitizer::cSciFiLightYield= 8000.; // [photons/MeV]
const Double_t ERNeuRadDigitizer::cSpeedOfLight = 0.299792458e2;  //[cm/ns]
const Double_t ERNeuRadDigitizer::cMaterialSpeedOfLight = ERNeuRadDigitizer::cSpeedOfLight/1.58;//[cm/ns]
const Int_t    ERNeuRadDigitizer::cErrorPointsInModuleCount = 1000;
const Double_t ERNeuRadDigitizer::cLightFractionInTotalIntReflection = 0.04;
//доля света захватываемая файбером в полное внутренне отражение в каждую сторону.
const Double_t ERNeuRadDigitizer::cExcessNoiseFactor = 1.3;
const Double_t ERNeuRadDigitizer::cPMTDelay=6.;     //[ns] (H8500)
const Double_t ERNeuRadDigitizer::cPMTJitter = 0.4/2.36; //[ns] (H8500)
const Int_t    ERNeuRadDigitizer::cPECountForOneElectronsSim = 20;
const Double_t ERNeuRadDigitizer::cScincilationTau = 3.2; //[ns]
const Double_t ERNeuRadDigitizer::cScincilationDT = 0.05;  //[ns]
const Double_t ERNeuRadDigitizer::cMaxPointLength = 4.; //[cm]


// ----------------------------------------------------------------------------
ERNeuRadDigitizer::ERNeuRadDigitizer()
  : FairTask("ER NeuRad Digitization scheme"),
  fDiscriminatorThreshold(0.),
  fPMTJitter(cPMTJitter),
  fPMTDelay(cPMTDelay),
  fPECountForOneElectronsSim(cPECountForOneElectronsSim),
  fExcessNoiseFactor(cExcessNoiseFactor),
  fScincilationTau(cScincilationTau),
  fScincilationDT(cScincilationDT)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadDigitizer::ERNeuRadDigitizer(Int_t verbose)
  : FairTask("ER NeuRad Digitization scheme ", verbose),
  fDiscriminatorThreshold(0.),
  fPMTJitter(cPMTJitter),
  fPMTDelay(cPMTDelay),
  fPECountForOneElectronsSim(cPECountForOneElectronsSim),
  fExcessNoiseFactor(cExcessNoiseFactor),
  fScincilationTau(cScincilationTau),
  fScincilationDT(cScincilationDT)
{
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
    LOG(INFO) << "ERNeuRadDigitizer::SetParContainers() "<< FairLogger::endl;
    LOG(INFO) << "ERNeuRadDigiPar initialized! "<< FairLogger::endl;
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
  ioman->Register("NeuRadPMTSignal", "Signal after PMT", fNeuRadPMTSignal, kTRUE);
  fNeuRadDigi = new TClonesArray("ERNeuRadDigi",1000);
  ioman->Register("NeuRadDigi", "Digital response in NeuRad", fNeuRadDigi, kTRUE);
  
  fRand = new TRandom3();
  
  fDigiPar->print();
  
  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERNeuRadDigitizer::Exec(Option_t* opt)
{
  Int_t iEvent =
			FairRunAna::Instance()->GetEventHeader()->GetMCEntryNumber();
  LOG(INFO) << "Event " << iEvent << FairLogger::endl;
  // Reset entries in output arrays
  Reset();
  Int_t nofFibers = fDigiPar->NofFibers();
  Int_t nofBundles = fDigiPar->NofBundles();
  //выделяем память под хранение поинтов по файберам
  vector<ERNeuRadFiberPoint* >** frontPointsPerFibers = new vector<ERNeuRadFiberPoint*>* [nofBundles];
  vector<ERNeuRadFiberPoint* >** backPointsPerFibers = new vector<ERNeuRadFiberPoint*>*  [nofBundles];
  for (Int_t i = 0; i<nofBundles; i++){
    frontPointsPerFibers[i] = new vector<ERNeuRadFiberPoint*>  [nofFibers];
  }
  
  Int_t points_count = fNeuRadPoints->GetEntries();
  
  //Формируем промежуточные сущности FiberPoints
  for (Int_t iPoint=0; iPoint < points_count; iPoint++) { // цикл
    ERNeuRadPoint *point = (ERNeuRadPoint*) fNeuRadPoints->At(iPoint);
    
    FiberPointsCreating(iPoint,point,frontPointsPerFibers,backPointsPerFibers);
  }
  
  //Формируем сигналы на ФЭУ и digi
  for (Int_t iBundle = 0; iBundle < nofBundles; iBundle++){
    for (Int_t iFiber = 0; iFiber < nofFibers; iFiber++) {
      PMTSignalsAndDigiCreating(iBundle, iFiber, frontPointsPerFibers,backPointsPerFibers);
    }
  }

  //освобождаем память
  for (Int_t i = 0; i<nofBundles; i++){
    delete [] frontPointsPerFibers[i];
  }
  delete [] frontPointsPerFibers;
  delete [] backPointsPerFibers;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERNeuRadDigitizer::LongPointSeparating(ERNeuRadPoint* point, vector<ERNeuRadPoint*> * points){
  Double_t pointCount, lastPointLen;
  if ((lastPointLen =  modf(point->GetLength()/cMaxPointLength, &pointCount)*cMaxPointLength) > 0.0001){
    pointCount+=1;
  }
  else
    lastPointLen = cMaxPointLength;
  
  Double_t curLen = 0.;
  
  LOG(INFO) << "New Long Point "<<FairLogger::endl;
  LOG(INFO) <<"len = " << point->GetLength() << " pointCount = " << pointCount << " EnergyLoss = " << point->GetEnergyLoss() << 
     " time = " << point->GetTime() << " zin = " <<  point->GetZIn() <<FairLogger::endl;
  
  for(Int_t iPoint=0; iPoint<(Int_t)pointCount; iPoint++){
    ERNeuRadPoint* sepPoint = new ERNeuRadPoint(*point);
    //Длина поинта
    Double_t pLength;
    if (iPoint == pointCount-1)
      pLength = lastPointLen;
    else
      pLength = cMaxPointLength;
    sepPoint->SetEnergyLoss( point->GetEnergyLoss()*pLength/point->GetLength() );
    sepPoint->SetLightYield( point->GetLightYield()*pLength/point->GetLength() );
    sepPoint->SetTimeIn(point->GetTimeIn() + (point->GetTimeOut() - point->GetTimeIn())*(curLen)/point->GetLength());
    sepPoint->SetXOut( point->GetXIn() + ((point->GetXOut() - point->GetXIn())*(curLen+pLength)/point->GetLength()));
    sepPoint->SetXIn( point->GetXIn() + ((point->GetXOut() - point->GetXIn())*curLen/point->GetLength()));
    sepPoint->SetYOut( point->GetYIn() + ((point->GetYOut() - point->GetYIn())*(curLen+pLength)/point->GetLength()));
    sepPoint->SetYIn( point->GetYIn() + ((point->GetYOut() - point->GetYIn())*curLen/point->GetLength()));
    sepPoint->SetZOut( point->GetZIn() + ((point->GetZOut() - point->GetZIn())*(curLen+pLength)/point->GetLength()));
    sepPoint->SetZIn( point->GetZIn() + ((point->GetZOut() - point->GetZIn())*curLen/point->GetLength()));
    //@bug т.к. это промежуточная сущность, заполенены только поля необходимые для расчета на данный момент
    
    LOG(INFO) <<"len = " << sepPoint->GetLength() << " EnergyLoss = " << sepPoint->GetEnergyLoss() << 
     " time = " << sepPoint->GetTimeIn() << " zin = " <<  sepPoint->GetZIn()  <<FairLogger::endl;
    curLen+=pLength;
    points->push_back(sepPoint);
  }
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERNeuRadDigitizer::FiberPointsCreating(Int_t i_point, ERNeuRadPoint *point,
                        std::vector<ERNeuRadFiberPoint* >** frontPointsPerFibers,
                        std::vector<ERNeuRadFiberPoint* >** backPointsPerFibers)
{

    Double_t fiber_length = fDigiPar->FiberLength();
    Int_t    point_bundle = point->GetBundleNb();
    Int_t    point_fiber_nb  = point->GetFiberInBundleNb();
    Double_t point_eLoss      =  point->GetEnergyLoss(); //[GeV]
    Double_t point_lightYield = point->GetLightYield();  //[GeV]
    Double_t point_x          = point->GetXIn();
    Double_t point_y          = point->GetYIn();
    Double_t point_z          = point->GetZIn();
    Double_t point_time       = point->GetTimeIn();
    Double_t point_z_in_fiber = point_z + fiber_length/2.;
    
    /*
    if (frontPointsPerFibers[point_fiber_nb].size() > cErrorPointsInModuleCount)
       LOG(ERROR) << "ERNeuRadDigitizerFullMC: Too many points in one fiber: "
                  << frontPointsPerFibers[point_fiber_nb].size()<< " points" << FairLogger::endl;
    */
    Double_t PMTQuantumEfficiency = fDigiPar->PMTQuantumEfficiency(point_fiber_nb);
    Double_t PMTGain = fDigiPar->PMTGain(point_fiber_nb);
    
    //scintillator light yield - общее число рожденных фотонов
    Double_t photon_count = point_lightYield*1000.*cSciFiLightYield;
    
    //Моделируем распространнение сигнала на передние ФЭУ
   
    Double_t k1 = 0.5-cLightFractionInTotalIntReflection;
    Double_t k2 = cLightFractionInTotalIntReflection;
    
    Double_t ffp_photon_count =  photon_count*(k1*exp(-point_z_in_fiber/0.5) + k2*exp(-point_z_in_fiber/200.));

    Double_t remainingPhotoEl = ffp_photon_count*PMTQuantumEfficiency/fScincilationTau;
    //LOG(INFO) << "LY " << point_lightYield << " PC " << photon_count << " FPC " << ffp_photon_count << " RPE " << remainingPhotoEl << FairLogger::endl; 
    for(Int_t iOnePESignal=0;iOnePESignal<remainingPhotoEl;iOnePESignal++){
      //Прогнозируем времена их появления в ФЭУ, через решение обратной задачи для экспоненциального распределения
      Double_t ffp_lytime = point_time + (-1)*fScincilationTau*TMath::Log(1-fRand->Uniform());
      Double_t ffp_cathode_time = ffp_lytime + (point_z_in_fiber)/cMaterialSpeedOfLight;
      Double_t onePESigma = TMath::Sqrt(fExcessNoiseFactor-1)*PMTGain;
      Double_t ffp_amplitude = TMath::Abs(fRand->Gaus(PMTGain, onePESigma));
      Double_t ffp_anode_time = ffp_cathode_time + (Double_t)fRand->Gaus(fPMTDelay, fPMTJitter);
      //LOG(INFO) << "LYT " << ffp_lytime << " CT " << ffp_cathode_time << " AT " << ffp_anode_time << " A " << ffp_amplitude << endl;
      ERNeuRadFiberPoint* ffPoint = AddFiberPoint(i_point, 0, ffp_lytime - point_time, ffp_cathode_time, ffp_anode_time, ffp_photon_count,
                          1, ffp_amplitude, 1);
      frontPointsPerFibers[point_bundle][point_fiber_nb].push_back(ffPoint);
    }

    Double_t bfp_photon_count =  photon_count*(k1*exp(-(fiber_length-point_z_in_fiber)/0.5) 
                                                   + k2*exp(-(fiber_length-point_z_in_fiber)/200.));
    
    remainingPhotoEl = bfp_photon_count*PMTQuantumEfficiency/fScincilationTau;

    for(Int_t iOnePESignal=0;iOnePESignal<remainingPhotoEl;iOnePESignal++){
      //Прогнозируем времена их появления в ФЭУ, через решение обратной задачи для экспоненциального распределения
      Double_t bfp_lytime = point_time + (-1)*fScincilationTau*TMath::Log(1-fRand->Uniform());
      Double_t bfp_cathode_time = bfp_lytime + (point_z_in_fiber)/cMaterialSpeedOfLight;
      Double_t onePESigma = TMath::Sqrt(fExcessNoiseFactor-1)*PMTGain;
      Double_t bfp_amplitude = TMath::Abs(fRand->Gaus(PMTGain, onePESigma));
      Double_t bfp_anode_time = bfp_cathode_time + (Double_t)fRand->Gaus(fPMTDelay, fPMTJitter);
      ERNeuRadFiberPoint* bfPoint = AddFiberPoint(i_point, 1, bfp_lytime - point_time, bfp_cathode_time, bfp_anode_time, bfp_photon_count,
                          1, bfp_amplitude, 1);
      backPointsPerFibers[point_bundle][point_fiber_nb].push_back(bfPoint);
    }

}
//----------------------------------------------------------------------------
void ERNeuRadDigitizer::PMTSignalsAndDigiCreating(Int_t iBundle, Int_t iFiber,
                                std::vector<ERNeuRadFiberPoint* >** frontPointsPerFibers,
                                std::vector<ERNeuRadFiberPoint* >** backPointsPerFibers)
{
  if( frontPointsPerFibers[iBundle][iFiber].size() > 0){
    ERNeuRadPMTSignal* pmtFSignal = AddPMTSignal(iBundle, iFiber);
    for(Int_t iFPoint = 0; iFPoint < frontPointsPerFibers[iBundle][iFiber].size(); iFPoint++){
      ERNeuRadFiberPoint* FPoint = frontPointsPerFibers[iBundle][iFiber][iFPoint];
      pmtFSignal->AddFiberPoint(FPoint);
      pmtFSignal->AddLink(FairLink("NeuRadFiberPoint",FPoint->Index()));
    }
    if (pmtFSignal->Exist()){
      vector<Double_t> intersections = pmtFSignal->GetIntersections(fDiscriminatorThreshold);
      for (Int_t iInter = 0; iInter < intersections.size(); iInter+=2){
        AddDigi(intersections[iInter], intersections[iInter+1],0., 0.,0.,0.,iFiber);
      }
    }
  }
  
  if (backPointsPerFibers[iBundle][iFiber].size() > 0){
    ERNeuRadPMTSignal* pmtBSignal = AddPMTSignal(iBundle, iFiber);
    for(Int_t iFPoint = 0; iFPoint < backPointsPerFibers[iBundle][iFiber].size(); iFPoint++){
      ERNeuRadFiberPoint* FPoint = backPointsPerFibers[iBundle][iFiber][iFPoint];
      pmtBSignal->AddFiberPoint(FPoint);
      pmtBSignal->AddLink(FairLink("NeuRadFiberPoint",FPoint->Index()));
    }
    if (pmtBSignal->Exist()){
      vector<Double_t> intersections = pmtBSignal->GetIntersections(fDiscriminatorThreshold);
      for (Int_t iInter = 0; iInter < intersections.size(); iInter+=2){
        AddDigi(intersections[iInter], intersections[iInter+1],0., 0.,0.,0.,iFiber);
      }
    }
  }
  
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

// ----------------------------------------------------------------------------
void ERNeuRadDigitizer::Finish()
{   

}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadDigi* ERNeuRadDigitizer::AddDigi(Double_t frontTDC, Double_t backTDC,
                                      Double_t TDC, Double_t frontQDC, Double_t backQDC, Double_t QDC,
                                      Int_t fiber_nr)
{
  ERNeuRadDigi *digi = new((*fNeuRadDigi)[DigiCount()])
							ERNeuRadDigi(fNeuRadDigi->GetEntriesFast(), frontTDC, backTDC, TDC, 
                                         frontQDC, backQDC, QDC, fiber_nr);
  return digi;
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadPMTSignal* ERNeuRadDigitizer::AddPMTSignal(Int_t iBundle, Int_t iFiber){
  ERNeuRadPMTSignal *pmtSignal = new((*fNeuRadPMTSignal)[PMTSignalCount()])
							ERNeuRadPMTSignal(iBundle, iFiber);            
  return pmtSignal;
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

//-----------------------------------------------------------------------------
ClassImp(ERNeuRadDigitizer)
