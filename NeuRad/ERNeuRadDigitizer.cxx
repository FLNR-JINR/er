
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
const Double_t ERNeuRadDigitizer::cPMTQuantumEfficiency = 0.2;
const Double_t ERNeuRadDigitizer::cPMTGain = 5; // [mV/p.e]
const Double_t ERNeuRadDigitizer::cExcessNoiseFactor = 1.3;
const Double_t ERNeuRadDigitizer::cPMTDelay=6.;     //[ns] (H8500)
const Double_t ERNeuRadDigitizer::cPMTJitter = 0.4; //[ns] (H8500)
const Int_t    ERNeuRadDigitizer::cPECountForOneElectronsSim = 20;
const Double_t ERNeuRadDigitizer::cScincilationTau = 3.2; //[ns]
const Double_t ERNeuRadDigitizer::cScincilationDT = 0.05;  //[ns]
const Double_t ERNeuRadDigitizer::cMaxPointLength = 4.; //[cm]


//todo убрать это в отдельный класс
const Double_t fiber_length = 100.;

// ----------------------------------------------------------------------------
ERNeuRadDigitizer::ERNeuRadDigitizer()
  : FairTask("ER NeuRad Digitization scheme"),
  fDiscriminatorThreshold(0.)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadDigitizer::ERNeuRadDigitizer(Int_t verbose)
  : FairTask("ER NeuRad Digitization scheme ", verbose),
  fDiscriminatorThreshold(0.)
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

  if ( fVerbose /*&& fLandDigiPar*/ ) {
    LOG(INFO) << "ERNeuRadDigitizer::SetParContainers() "<< FairLogger::endl;
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

  fNFibers = 64;  //@todo get from parameter class 
 
  fRand = new TRandom3();
  
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
  vector<ERNeuRadFiberPoint* >* frontPointsPerFibers = new vector<ERNeuRadFiberPoint*> [fNFibers];
  vector<ERNeuRadFiberPoint* >* backPointsPerFibers = new vector<ERNeuRadFiberPoint*>  [fNFibers];
  
  Double_t fiber_length = 100.;//[cm] //@todo get from parameter class 
  
  Int_t points_count = fNeuRadPoints->GetEntries();
  
  //Формируем промежуточные сущности FiberPoints
  //LOG(INFO) << "Point count" << points_count << FairLogger::endl;
  for (Int_t iPoint=0; iPoint < points_count; iPoint++) { // цикл
    //LOG(INFO) <<"iPoint = " << iPoint << FairLogger::endl;
    ERNeuRadPoint *point = (ERNeuRadPoint*) fNeuRadPoints->At(iPoint);
    
    if (point->GetLength() < cMaxPointLength){
      FiberPointsCreating(iPoint,point,frontPointsPerFibers,backPointsPerFibers);
      continue;
    }
    
    //Дробим поинт по длине
    vector<ERNeuRadPoint*> *points = new vector<ERNeuRadPoint*>;
    LongPointSeparating(point, points);
    
    for (Int_t jPoint = 0; jPoint < points->size(); jPoint++){
      ERNeuRadPoint *sepPoint = (*points)[jPoint];
      FiberPointsCreating(iPoint,sepPoint,frontPointsPerFibers,backPointsPerFibers);
    }
  }
  
  //Формируем сигналы на ФЭУ и digi
  for (Int_t iFiber = 0; iFiber < fNFibers; iFiber++) {
    PMTSignalsAndDigiCreating(iFiber, frontPointsPerFibers,backPointsPerFibers);
  }
  
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
                        std::vector<ERNeuRadFiberPoint* >* frontPointsPerFibers,
                        std::vector<ERNeuRadFiberPoint* >* backPointsPerFibers){
    
    Int_t    point_fiber_nb  = point->GetFiberInBundleNb();
    Double_t point_eLoss	    =  point->GetEnergyLoss(); //[GeV]
    Double_t point_lightYield = point->GetLightYield();  //[GeV]
    Double_t point_x          = point->GetXIn();
    Double_t point_y          = point->GetYIn();
    Double_t point_z          = point->GetZIn();
    Double_t point_time       = point->GetTimeIn();
    Double_t point_z_in_fiber = point_z + fiber_length/2.;
    if (frontPointsPerFibers[point_fiber_nb].size() > cErrorPointsInModuleCount)
       LOG(ERROR) << "ERNeuRadDigitizer: Too many points in one fiber: "
                  << frontPointsPerFibers[point_fiber_nb].size()<< " points" << FairLogger::endl;
    
    /*
      Высвечивание энергии в фотоны происходит не мгновенно, а по exp от времени.
      Будем моделировать выход фотонов пачками во времени. Пачка фотонов выходит в отрезке времени(timeslice) равному dt.
      Каждая пачка фотонов соответствует объекту типа FiberPoint
    */
    
    //Время выхода последней пачки фотонов, которые дадут один фотоэлектрон
    Double_t finishTime =  -1*cScincilationTau*(1-fRand->Uniform());
    //Количество timeSlice
    Int_t timeSliceCount = (finishTime - point_time)/cScincilationDT;
    
    //scintillator light yield - общее число рожденных фотонов
    Double_t photon_count = point_lightYield*1000.*cSciFiLightYield;
    
    Int_t iTimeSlice = 1;
    //LOG(INFO) << "photon_count = " << photon_count << FairLogger::endl;
    
    //Разделение длинных поинтов
    //Моделируем распространнение сигнала на передние ФЭУ
    while(1){
      //ffp - front fiber point, bfp - back_fiber_point
      Double_t ffp_cathode_time  = point_time+iTimeSlice*cScincilationDT+(point_z_in_fiber)/cMaterialSpeedOfLight;
      
      //Количество оставшихся фотонов
      Double_t ffp_remainig_photons_count = photon_count*TMath::Exp(-cScincilationDT*(Double_t)iTimeSlice/cScincilationTau);
      //Количество фотонов в слайсе                              
      Double_t ffp_photon_count = photon_count*TMath::Exp(-cScincilationDT*(Double_t)(iTimeSlice)/cScincilationTau)
                                      *cScincilationDT/cScincilationTau;
      /*
        Two component attenuation for optical fibers. First term is rough 
        approximation for dirrect illumination of a cathode by the source, the 
        second one is for attrenuation of the totally reflected in the WLS. At 
        the ends - half of the light goes to the nearest photocathode 
      */
      //LOG(INFO) << "ffp_cathode_time = " << ffp_cathode_time << " ffp_remainig_photons_count =" << 
      //          ffp_remainig_photons_count << " ffp_photon_count=" << ffp_photon_count << FairLogger::endl;
      Double_t k1 = 0.5-cLightFractionInTotalIntReflection;
      Double_t k2 = cLightFractionInTotalIntReflection;
      
      ffp_photon_count =  ffp_photon_count*(k1*exp(-point_z_in_fiber/0.5) + k2*exp(-point_z_in_fiber/200.));
      ffp_remainig_photons_count = ffp_remainig_photons_count*(k1*exp(-point_z_in_fiber/0.5) + k2*exp(-point_z_in_fiber/200.));
      //Если число фотоэлектронов, оставшихся в конце высвечивания меньше заданного моделируем их как одноэлектронные импульсы
      //Моделирование хвоста высвечивания
      Double_t remainingPhotoEl = ffp_remainig_photons_count*cPMTQuantumEfficiency/cScincilationTau;
      if(remainingPhotoEl < cPECountForOneElectronsSim){
        for(Int_t iOnePESignal=0;iOnePESignal<remainingPhotoEl;iOnePESignal++){
          //Прогнозируем времена их появления в ФЭУ, через решение обратной задачи для экспоненциального распределения
          ffp_cathode_time = point_time+iTimeSlice*cScincilationDT + (-1)*cScincilationTau*TMath::Log(1-fRand->Uniform());
          Double_t ffp_amplitude = cPMTGain;
          Double_t ffp_anode_time = ffp_cathode_time + (Double_t)fRand->Gaus(cPMTDelay, cPMTJitter);
          ERNeuRadFiberPoint* ffPoint = AddFiberPoint(i_point, 0, ffp_cathode_time, ffp_anode_time, ffp_photon_count,
                              1, ffp_amplitude, 1);
          frontPointsPerFibers[point_fiber_nb].push_back(ffPoint);
        }
        break;
      }
      //Take into account quantum efficiency. Photoelectrons count
      //@todo Среднее значение квантовой эфективности должно быть разным для каждого файбера
      Double_t ffp_photoel_count = (Double_t)fRand->PoissonD( ffp_photon_count *cPMTQuantumEfficiency);
      
      if (ffp_photoel_count < 1.) continue;
      
      //Take into account pmt gain
      Double_t ffp_amplitude = fRand->Gaus(ffp_photoel_count * cPMTGain,
                                ffp_photoel_count * TMath::Sqrt(cExcessNoiseFactor));
      
      //Take into account PMT delay and jitter
      Double_t ffp_anode_time = ffp_cathode_time + (Double_t)fRand->Gaus(cPMTDelay, cPMTJitter);
      
      if (ffp_amplitude > 0.){
        ERNeuRadFiberPoint* ffPoint = AddFiberPoint(i_point, 0, ffp_cathode_time, ffp_anode_time, ffp_photon_count,
                              ffp_photoel_count, ffp_amplitude, 0);
        //Collect Fiber Points by fiber
        frontPointsPerFibers[point_fiber_nb].push_back(ffPoint);
      }
      iTimeSlice++;
    }
    
    iTimeSlice = 1;
    //Моделируем рампространнение сигнала на задние ФЭУ
    while(1){
      Double_t bfp_cathode_time  = point_time+iTimeSlice*cScincilationDT+(fiber_length-point_z_in_fiber)/cMaterialSpeedOfLight;
      
      Double_t bfp_remainig_photons_count = photon_count*TMath::Exp(-cScincilationDT*(Double_t)iTimeSlice/cScincilationTau);
      
      Double_t bfp_photon_count = photon_count*TMath::Exp(-cScincilationDT*(Double_t)(iTimeSlice+1)/cScincilationTau)
                                      *cScincilationDT/cScincilationTau;
                                    
      Double_t k1 = 0.5-cLightFractionInTotalIntReflection;
      Double_t k2 = cLightFractionInTotalIntReflection;
      bfp_photon_count =  bfp_photon_count*(k1*exp(-(fiber_length-point_z_in_fiber)/0.5) 
                                                   + k2*exp(-(fiber_length-point_z_in_fiber)/200.));
      bfp_remainig_photons_count = bfp_remainig_photons_count*(k1*exp(-point_z_in_fiber/0.5) + k2*exp(-point_z_in_fiber/200.));
      
      Double_t remainingPhotoEl = bfp_remainig_photons_count*cPMTQuantumEfficiency/cScincilationTau;
      if(remainingPhotoEl < cPECountForOneElectronsSim){
        //LOG(INFO) << remainingPhotoEl << FairLogger::endl;
        for(Int_t iOnePESignal=0;iOnePESignal<remainingPhotoEl;iOnePESignal++){
          //Прогнозируем времена их появления в ФЭУ, через решение обратной задачи для экспоненциального распределения
          bfp_cathode_time = point_time+iTimeSlice*cScincilationDT + (-1)*cScincilationTau*TMath::Log(1-fRand->Uniform());
          Double_t bfp_amplitude = cPMTGain;
          Double_t bfp_anode_time = bfp_cathode_time + (Double_t)fRand->Gaus(cPMTDelay, cPMTJitter);
          ERNeuRadFiberPoint* bfPoint = AddFiberPoint(i_point, 1, bfp_cathode_time, bfp_anode_time, bfp_photon_count,
                              1, bfp_amplitude, 1);
          backPointsPerFibers[point_fiber_nb].push_back(bfPoint);
        } 
        break;
      }
      
      Double_t bfp_photoel_count = (Double_t)fRand->PoissonD( bfp_photon_count *cPMTQuantumEfficiency);
      
      if (bfp_photoel_count < 1.) continue;
      
      Double_t bfp_amplitude = fRand->Gaus(bfp_photoel_count * cPMTGain,
                                bfp_photoel_count * TMath::Sqrt(cExcessNoiseFactor));
                                
      Double_t bfp_anode_time = bfp_cathode_time + (Double_t)fRand->Gaus(cPMTDelay, cPMTJitter);
      
      if (bfp_amplitude > 0.){
        ERNeuRadFiberPoint* bfPoint = AddFiberPoint(i_point, 1, bfp_cathode_time, bfp_anode_time, bfp_photon_count,
                              bfp_photoel_count, bfp_amplitude, 0);
        backPointsPerFibers[point_fiber_nb].push_back(bfPoint);
      }
      iTimeSlice++;
    }
}
//----------------------------------------------------------------------------
void ERNeuRadDigitizer::PMTSignalsAndDigiCreating(Int_t iFiber,
                                std::vector<ERNeuRadFiberPoint* >* frontPointsPerFibers,
                                std::vector<ERNeuRadFiberPoint* >* backPointsPerFibers){
 
  if( frontPointsPerFibers[iFiber].size() > 0){
    ERNeuRadPMTSignal* pmtFSignal = AddPMTSignal(iFiber);
    for(Int_t iFPoint = 0; iFPoint < frontPointsPerFibers[iFiber].size(); iFPoint++){
      ERNeuRadFiberPoint* FPoint = frontPointsPerFibers[iFiber][iFPoint];
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
  
  if (backPointsPerFibers[iFiber].size() > 0){
    ERNeuRadPMTSignal* pmtBSignal = AddPMTSignal(iFiber);
    for(Int_t iFPoint = 0; iFPoint < backPointsPerFibers[iFiber].size(); iFPoint++){
      ERNeuRadFiberPoint* FPoint = backPointsPerFibers[iFiber][iFPoint];
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
    fNeuRadDigi->Clear();
  }
  if (fNeuRadFiberPoint){
    fNeuRadFiberPoint->Clear();
  }
  if (fNeuRadPMTSignal){
    fNeuRadPMTSignal->Clear();
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
ERNeuRadPMTSignal* ERNeuRadDigitizer::AddPMTSignal(Int_t iFiber){
  ERNeuRadPMTSignal *pmtSignal = new((*fNeuRadPMTSignal)[PMTSignalCount()])
							ERNeuRadPMTSignal();            
  return pmtSignal;
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadFiberPoint* ERNeuRadDigitizer::AddFiberPoint(Int_t i_point, Int_t side, Double_t cathode_time, Double_t anode_time, 
									Int_t photon_count, Int_t photoel_count, 
									Double_t amplitude, Int_t onePE){
  ERNeuRadFiberPoint *fp = new ((*fNeuRadFiberPoint)[FiberPointCount()])
								ERNeuRadFiberPoint(FiberPointCount(),side, cathode_time, anode_time, photon_count, 
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
