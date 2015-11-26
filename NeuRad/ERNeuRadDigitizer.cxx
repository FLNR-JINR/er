
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

using std::sort;
using std::vector;

#include "TGeoManager.h"
#include "TClonesArray.h"
#include "TVector3.h"
#include "TMath.h"
#include "TRandom3.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"

#include "ERNeuRadPoint.h"
#include "ERNeuRadStep.h"
#include "ERNeuRadDigitizer.h"

const Double_t ERNeuRadDigitizer::SciFi_LIGHT_YIELD= 8000.; // [photons/MeV]
const Double_t ERNeuRadDigitizer::SPEED_OF_LIGHT = 0.299792458e2;  //[cm/ns]
const Double_t ERNeuRadDigitizer::SPEED_OF_LIGHT_IN_MATERIAL = ERNeuRadDigitizer::SPEED_OF_LIGHT/1.58;//[cm/ns]
const Int_t    ERNeuRadDigitizer::ERROR_POINTS_IN_MODULE_COUNT = 1000;
const Double_t ERNeuRadDigitizer::LIGHT_FRACTION_IN_TOTAL_INT_REFLECTION = 0.04;
//доля света захватываемая файбером в полное внутренне отражение в каждую сторону.
const Double_t ERNeuRadDigitizer::PMT_QUANTUM_EFFICIENCY = 0.2;
const Double_t ERNeuRadDigitizer::PMT_GAIN = 5; // [mV/p.e]
const Double_t ERNeuRadDigitizer::EXCESS_NOISE_FACTOR = 1.3;
const Double_t ERNeuRadDigitizer::PMT_DELAY=6.;     //[ns] (H8500)
const Double_t ERNeuRadDigitizer::PMT_JITTER = 0.4; //[ns] (H8500)
const Int_t    ERNeuRadDigitizer::PE_COUNT_FOR_ONE_ELECTRONS_SIM = 20;
const Double_t ERNeuRadDigitizer::SCINCILATION_TAU = 3.2; //[ns]
const Double_t ERNeuRadDigitizer::SCINCILATION_dT = 0.05;  //[ns]

// ----------------------------------------------------------------------------
ERNeuRadDigitizer::ERNeuRadDigitizer()
  : FairTask("ER NeuRad Digitization scheme"),
  fFiberThreshold(0.),
  fDiscriminatorThreshold(0.)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadDigitizer::ERNeuRadDigitizer(Int_t verbose)
  : FairTask("ER NeuRad Digitization scheme ", verbose),
  fFiberThreshold(0.),
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
  
  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERNeuRadDigitizer::Exec(Option_t* opt)
{
  // Reset entries in output arrays
  Reset();
  TRandom3  *rand = new TRandom3();
  vector<ERNeuRadFiberPoint* >* frontPointsPerFibers = new vector<ERNeuRadFiberPoint*> [fNFibers];
  vector<ERNeuRadFiberPoint* >* backPointsPerFibers = new vector<ERNeuRadFiberPoint*>  [fNFibers];
  
  Double_t fiber_length = 100.;//[cm] //@todo get from parameter class 
  
  Int_t points_count = fNeuRadPoints->GetEntries();
  
  for (Int_t i_point=0; i_point < points_count; i_point++) {
    ERNeuRadPoint *point = (ERNeuRadPoint*) fNeuRadPoints->At(i_point);
    
    Int_t    point_fiber_nb  = point->GetFiberInBundleNb();
    Double_t point_eLoss	    =  point->GetEnergyLoss(); //[GeV]
    Double_t point_lightYield = point->GetLightYield();  //[GeV]
    Double_t point_x          = point->GetXIn();
    Double_t point_y          = point->GetYIn();
    Double_t point_z          = point->GetZIn();
    Double_t point_time       = point->GetTime();
    Double_t point_z_in_fiber = point_z + fiber_length/2.;
    
    if (frontPointsPerFibers[point_fiber_nb].size() > ERROR_POINTS_IN_MODULE_COUNT)
       LOG(ERROR) << "ERNeuRadDigitizer: Too many points in one fiber: "
                  << frontPointsPerFibers[point_fiber_nb].size()<< " points" << FairLogger::endl;
    
    /*
      Высвечивание энергии в фотоны происходит не мгновенно, а по exp от времени.
      Будем моделировать выход фотонов пачками во времени. Пачка фотонов выходит в отрезке времени(timeslice) равному dt.
      Каждая пачка фотонов соответствует объекту типа FiberPoint
    */
    
    //Время выхода последней пачки фотонов, которые дадут один фотоэлектрон
    Double_t finishTime =  -1*SCINCILATION_TAU*(1-rand->Uniform());
    //Количество timeSlice
    Int_t timeSliceCount = (finishTime - point_time)/SCINCILATION_dT;
    
    //scintillator light yield - общее число рожденных фотонов
    Double_t photon_count = point_lightYield*1000.*SciFi_LIGHT_YIELD;
    
    Int_t iTimeSlice = 1;
    LOG(INFO) << "photon_count = " << photon_count << FairLogger::endl;
    //Моделируем распространнение сигнала на передние ФЭУ
    while(1){
      //ffp - front fiber point, bfp - back_fiber_point
      Double_t ffp_cathode_time  = point_time+iTimeSlice*SCINCILATION_dT+(point_z_in_fiber)/SPEED_OF_LIGHT_IN_MATERIAL;
      
      //Количество оставшихся фотонов
      Double_t ffp_remainig_photons_count = photon_count*TMath::Exp(-SCINCILATION_dT*(Double_t)iTimeSlice/SCINCILATION_TAU);
      //Количество фотонов в слайсе                              
      Double_t ffp_photon_count = photon_count*TMath::Exp(-SCINCILATION_dT*(Double_t)(iTimeSlice)/SCINCILATION_TAU)
                                      *SCINCILATION_dT/SCINCILATION_TAU;
      /*
        Two component attenuation for optical fibers. First term is rough 
        approximation for dirrect illumination of a cathode by the source, the 
        second one is for attrenuation of the totally reflected in the WLS. At 
        the ends - half of the light goes to the nearest photocathode 
      */
      LOG(INFO) << "ffp_cathode_time = " << ffp_cathode_time << " ffp_remainig_photons_count =" << 
                ffp_remainig_photons_count << " ffp_photon_count=" << ffp_photon_count << FairLogger::endl;
      Double_t k1 = 0.5-LIGHT_FRACTION_IN_TOTAL_INT_REFLECTION;
      Double_t k2 = LIGHT_FRACTION_IN_TOTAL_INT_REFLECTION;
      
      ffp_photon_count =  ffp_photon_count*(k1*exp(-point_z_in_fiber/0.5) + k2*exp(-point_z_in_fiber/200.));
      ffp_remainig_photons_count = ffp_remainig_photons_count*(k1*exp(-point_z_in_fiber/0.5) + k2*exp(-point_z_in_fiber/200.));
      //Если число фотоэлектронов, оставшихся в конце высвечивания меньше заданного моделируем их как одноэлектронные импульсы
      //Моделирование хвоста высвечивания
      Double_t remainingPhotoEl = ffp_remainig_photons_count*PMT_QUANTUM_EFFICIENCY/SCINCILATION_TAU;
      if(remainingPhotoEl < PE_COUNT_FOR_ONE_ELECTRONS_SIM){
        for(Int_t iOnePESignal=0;iOnePESignal<remainingPhotoEl;iOnePESignal++){
          //Прогнозируем времена их появления в ФЭУ, через решение обратной задачи для экспоненциального распределения
          ffp_cathode_time = point_time+iTimeSlice*SCINCILATION_dT + (-1)*SCINCILATION_TAU*TMath::Log(1-rand->Uniform());
          Double_t ffp_amplitude = PMT_GAIN;
          Double_t ffp_anode_time = ffp_cathode_time + (Double_t)rand->Gaus(PMT_DELAY, PMT_JITTER);
          ERNeuRadFiberPoint* ffPoint = AddFiberPoint(0, ffp_cathode_time, ffp_anode_time, ffp_photon_count,
                              1, ffp_amplitude);
          frontPointsPerFibers[point_fiber_nb].push_back(ffPoint);
        }
        break;
      }
      //Take into account quantum efficiency. Photoelectrons count
      //@todo Среднее значение квантовой эфективности должно быть разным для каждого файбера
      Double_t ffp_photoel_count = (Double_t)rand->PoissonD( ffp_photon_count *PMT_QUANTUM_EFFICIENCY);
      
      if (ffp_photoel_count < 1.) continue;
      
      //Take into account pmt gain
      Double_t ffp_amplitude = rand->Gaus(ffp_photoel_count * PMT_GAIN,
                                ffp_photoel_count * TMath::Sqrt(EXCESS_NOISE_FACTOR));
      
      //Take into account PMT delay and jitter
      Double_t ffp_anode_time = ffp_cathode_time + (Double_t)rand->Gaus(PMT_DELAY, PMT_JITTER);
      
      if (ffp_amplitude > 0.){
        ERNeuRadFiberPoint* ffPoint = AddFiberPoint(0, ffp_cathode_time, ffp_anode_time, ffp_photon_count,
                              ffp_photoel_count, ffp_amplitude);
        //Collect Fiber Points by fiber
        frontPointsPerFibers[point_fiber_nb].push_back(ffPoint);
      }
      iTimeSlice++;
    }
    
    iTimeSlice = 1;
    //Моделируем рампространнение сигнала на задние ФЭУ
    while(1){
      Double_t bfp_cathode_time  = point_time+iTimeSlice*SCINCILATION_dT+(fiber_length-point_z_in_fiber)/SPEED_OF_LIGHT_IN_MATERIAL;
      
      Double_t bfp_remainig_photons_count = photon_count*TMath::Exp(-SCINCILATION_dT*(Double_t)iTimeSlice/SCINCILATION_TAU);
      
      Double_t bfp_photon_count = photon_count*TMath::Exp(-SCINCILATION_dT*(Double_t)(iTimeSlice+1)/SCINCILATION_TAU)
                                      *SCINCILATION_dT/SCINCILATION_TAU;
                                    
      Double_t k1 = 0.5-LIGHT_FRACTION_IN_TOTAL_INT_REFLECTION;
      Double_t k2 = LIGHT_FRACTION_IN_TOTAL_INT_REFLECTION;
      bfp_photon_count =  bfp_photon_count*(k1*exp(-(fiber_length-point_z_in_fiber)/0.5) 
                                                   + k2*exp(-(fiber_length-point_z_in_fiber)/200.));
      bfp_remainig_photons_count = bfp_remainig_photons_count*(k1*exp(-point_z_in_fiber/0.5) + k2*exp(-point_z_in_fiber/200.));
      
      Double_t remainingPhotoEl = bfp_remainig_photons_count*PMT_QUANTUM_EFFICIENCY/SCINCILATION_TAU;
      if(remainingPhotoEl < PE_COUNT_FOR_ONE_ELECTRONS_SIM){
        for(Int_t iOnePESignal=0;iOnePESignal<remainingPhotoEl;iOnePESignal++){
          //Прогнозируем времена их появления в ФЭУ, через решение обратной задачи для экспоненциального распределения
          bfp_cathode_time = point_time+iTimeSlice*SCINCILATION_dT + (-1)*SCINCILATION_TAU*TMath::Log(1-rand->Uniform());
          Double_t bfp_amplitude = PMT_GAIN;
          Double_t bfp_anode_time = bfp_cathode_time + (Double_t)rand->Gaus(PMT_DELAY, PMT_JITTER);
          ERNeuRadFiberPoint* bfPoint = AddFiberPoint(1, bfp_cathode_time, bfp_anode_time, bfp_photon_count,
                              1, bfp_amplitude);
          backPointsPerFibers[point_fiber_nb].push_back(bfPoint);
        } 
        break;
      }
      
      Double_t bfp_photoel_count = (Double_t)rand->PoissonD( bfp_photon_count *PMT_QUANTUM_EFFICIENCY);
      
      if (bfp_photoel_count < 1.) continue;
      
      Double_t bfp_amplitude = rand->Gaus(bfp_photoel_count * PMT_GAIN,
                                bfp_photoel_count * TMath::Sqrt(EXCESS_NOISE_FACTOR));
                                
      Double_t bfp_anode_time = bfp_cathode_time + (Double_t)rand->Gaus(PMT_DELAY, PMT_JITTER);
      
      if (bfp_amplitude > 0.){
        ERNeuRadFiberPoint* bfPoint = AddFiberPoint(1, bfp_cathode_time, bfp_anode_time, bfp_photon_count,
                              bfp_photoel_count, bfp_amplitude);
        backPointsPerFibers[point_fiber_nb].push_back(bfPoint);
      }
      iTimeSlice++;
    }
  }
  /*
    Implementation of piecewise linear approximation of PMT fiber point impuls from 
    Румянцев "Алгоритм пространственно-временного поиска импульсных сигналов в режиме одноканальной
    регистрации однофотонных импульсов"
  */
  
  Int_t digiCount = 0;
  //Формируем кусочно линейные сигналы для каждого файбера
  for (Int_t iFiber = 0; iFiber < fNFibers; iFiber++) {
    ERNeuRadPMTSignal* pmtFSignal = AddPMTSignal();
    for(Int_t iFPoint = 0; iFPoint < frontPointsPerFibers[iFiber].size(); iFPoint++){
      ERNeuRadFiberPoint* FPoint = frontPointsPerFibers[iFiber][iFPoint];
      pmtFSignal->AddFiberPoint(FPoint);
    }
    if (pmtFSignal->Exist()){
      vector<Double_t> intersections = pmtFSignal->GetIntersections(fDiscriminatorThreshold);
      for (Int_t iInter = 0; iInter < intersections.size(); iInter+=2){
        AddDigi(digiCount++, intersections[iInter], intersections[iInter+1],0., 0.,0.,0.,iFiber);
      }
    }
    ERNeuRadPMTSignal* pmtBSignal = AddPMTSignal();
    for(Int_t iFPoint = 0; iFPoint < backPointsPerFibers[iFiber].size(); iFPoint++){
      ERNeuRadFiberPoint* FPoint = backPointsPerFibers[iFiber][iFPoint];
      pmtBSignal->AddFiberPoint(FPoint);
    }
    if (pmtBSignal->Exist()){
      vector<Double_t> intersections = pmtBSignal->GetIntersections(fDiscriminatorThreshold);
      for (Int_t iInter = 0; iInter < intersections.size(); iInter+=2){
        AddDigi(digiCount++, intersections[iInter], intersections[iInter+1],0., 0.,0.,0.,iFiber);
      }
    }
  }
  
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERNeuRadDigitizer::Reset()
{
  if (fNeuRadDigi) {
    fNeuRadFiberPoint->Clear();
    fNeuRadDigi->Clear();
  }
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERNeuRadDigitizer::Finish()
{   

}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadDigi* ERNeuRadDigitizer::AddDigi(Int_t digi_nr, Double_t frontTDC, Double_t backTDC,
                                      Double_t TDC, Double_t frontQDC, Double_t backQDC, Double_t QDC,
                                      Int_t fiber_nr)
{
  ERNeuRadDigi *digi = new((*fNeuRadDigi)[fNeuRadDigi->GetEntriesFast()])
							ERNeuRadDigi(digi_nr, frontTDC, backTDC, TDC, 
                                         frontQDC, backQDC, QDC, fiber_nr);
  return digi;
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadPMTSignal* ERNeuRadDigitizer::AddPMTSignal(){
  ERNeuRadPMTSignal *pmtSignal = new((*fNeuRadPMTSignal)[fNeuRadPMTSignal->GetEntriesFast()])
							ERNeuRadPMTSignal();
  return pmtSignal;
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadFiberPoint* ERNeuRadDigitizer::AddFiberPoint(Int_t side, Double_t cathode_time, Double_t anode_time, 
									Int_t photon_count, Int_t photoel_count, 
									Double_t amplitude){
  ERNeuRadFiberPoint *fp = new ((*fNeuRadFiberPoint)[fNeuRadFiberPoint->GetEntriesFast()])
								ERNeuRadFiberPoint(side, cathode_time, anode_time, photon_count, 
                                    photoel_count, amplitude);
  return fp;
}
//-----------------------------------------------------------------------------
ClassImp(ERNeuRadDigitizer)
