
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
#include "ERNeuRadDigitizerTS.h"
#include "ERNeuRadPMTSignalLP.h" 

// ----------------------------------------------------------------------------
ERNeuRadDigitizerTS::ERNeuRadDigitizerTS()
  : ERNeuRadDigitizer()
{
  fOnePEInteg = 3.5;
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadDigitizerTS::ERNeuRadDigitizerTS(Int_t verbose)
  : ERNeuRadDigitizer(verbose)
{
  fOnePEInteg = 3.5;
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadDigitizerTS::~ERNeuRadDigitizerTS()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus ERNeuRadDigitizerTS::Init()
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
  fNeuRadPMTSignal = new TClonesArray("ERNeuRadPMTSignalLP", 1000);
  LOG(INFO) << "ERNeuRadDigitizer: Use a ERNeuRadPMTSignalLP type of signal!" << endl;
  ioman->Register("NeuRadPMTSignal", "Signal after PMT", fNeuRadPMTSignal, kTRUE);
  fNeuRadDigi = new TClonesArray("ERNeuRadDigi",1000);
  ioman->Register("NeuRadDigi", "Digital response in NeuRad", fNeuRadDigi, kTRUE);
  
  fRand = new TRandom3();
  
  fNeuRadSetup = ERNeuRadSetup::Instance();
  fNeuRadSetup->Print();
  
  return kSUCCESS;
}
// -------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERNeuRadDigitizerTS::FiberPointsCreating(Int_t i_point, ERNeuRadPoint *point,
                        std::vector<ERNeuRadFiberPoint* >** frontPointsPerFibers,
                        std::vector<ERNeuRadFiberPoint* >** backPointsPerFibers)
{
  
    Double_t fiber_length = fDigiPar->FiberLength();
    Int_t    point_fiber_nb  = point->GetFiberInBundleNb();
    Int_t    point_bundle = point->GetBundleNb();
    Double_t point_eLoss	    =  point->GetEnergyLoss(); //[GeV]
    Double_t point_lightYield = point->GetLightYield();  //[GeV]
    Double_t point_x          = point->GetXIn();
    Double_t point_y          = point->GetYIn();
    Double_t point_z          = point->GetZIn();
    Double_t point_time       = point->GetTimeIn();
    Double_t point_z_in_fiber = point_z + fiber_length/2. - fNeuRadSetup->Z();
    cerr << "point_eLoss = " << point_eLoss << endl;
    cerr << "point_time = " << point_time << endl;
    /*
    if (frontPointsPerFibers[point_fiber_nb].size() > cErrorPointsInModuleCount)
       LOG(ERROR) << "ERNeuRadDigitizer: Too many points in one fiber: "
                  << frontPointsPerFibers[point_fiber_nb].size()<< " points" << FairLogger::endl;
    */
    Double_t PMTQuantumEfficiency = fDigiPar->PMTQuantumEfficiency(point_fiber_nb);
    Double_t PMTGain = fDigiPar->PMTGain(point_fiber_nb);
    /*
      Высвечивание энергии в фотоны происходит не мгновенно, а по exp от времени.
      Будем моделировать выход фотонов пачками во времени. Пачка фотонов выходит в отрезке времени(timeslice) равному dt.
      Каждая пачка фотонов соответствует объекту типа FiberPoint
    */
    
    //Время выхода последней пачки фотонов, которые дадут один фотоэлектрон
    Double_t finishTime =  -1*fScincilationTau*(1-fRand->Uniform());
    //Количество timeSlice
    Int_t timeSliceCount = (finishTime - point_time)/fScincilationDT;
    
    //scintillator light yield - общее число рожденных фотонов
    Double_t photon_count = point_lightYield*1000.*cSciFiLightYield;
    
    Int_t iTimeSlice = 1;
    //LOG(INFO) << "photon_count = " << photon_count << FairLogger::endl;
    
    //Разделение длинных поинтов
    //Моделируем распространнение сигнала на передние ФЭУ
    while(1){
      //ffp - front fiber point, bfp - back_fiber_point
      Double_t ffp_lytime = point_time+iTimeSlice*fScincilationDT;
      Double_t ffp_cathode_time  = ffp_lytime + (point_z_in_fiber)/cMaterialSpeedOfLight;

      //cerr << "ffp_lytime = " << ffp_lytime << endl;  
      //Количество оставшихся фотонов
      Double_t ffp_remainig_photons_count = photon_count*TMath::Exp(-fScincilationDT*(Double_t)iTimeSlice/fScincilationTau);
      //Количество фотонов в слайсе                              
      Double_t ffp_photon_count = photon_count*TMath::Exp(-fScincilationDT*(Double_t)(iTimeSlice)/fScincilationTau)
                                      *fScincilationDT/fScincilationTau;
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
      Double_t remainingPhotoEl = ffp_remainig_photons_count*PMTQuantumEfficiency/fScincilationTau;
      if(remainingPhotoEl < fPECountForOneElectronsSim){
        for(Int_t iOnePESignal=0;iOnePESignal<remainingPhotoEl;iOnePESignal++){
          //Прогнозируем времена их появления в ФЭУ, через решение обратной задачи для экспоненциального распределения
          ffp_lytime = point_time+iTimeSlice*fScincilationDT + (-1)*fScincilationTau*TMath::Log(1-fRand->Uniform());
          ffp_cathode_time = ffp_lytime + (point_z_in_fiber)/cMaterialSpeedOfLight;
          Double_t onePESigma = TMath::Sqrt(fExcessNoiseFactor-1)*PMTGain;
          Double_t ffp_amplitude = TMath::Abs(fRand->Gaus(PMTGain, onePESigma));
          Double_t ffp_anode_time = ffp_cathode_time + (Double_t)fRand->Gaus(fPMTDelay, fPMTJitter);
          ERNeuRadFiberPoint* ffPoint = AddFiberPoint(i_point, 0, ffp_lytime, ffp_cathode_time, ffp_anode_time, ffp_photon_count,
                              1, ffp_amplitude, 1);
          frontPointsPerFibers[point_bundle][point_fiber_nb].push_back(ffPoint);
        }
        break;
      }
      //Take into account quantum efficiency. Photoelectrons count
      //@todo Среднее значение квантовой эфективности должно быть разным для каждого файбера
      Double_t ffp_photoel_count = (Double_t)fRand->PoissonD( ffp_photon_count *PMTQuantumEfficiency);
      
      if (ffp_photoel_count < 1.) continue;
      
      //Take into account pmt gain
      Double_t ffp_amplitude = fRand->Gaus(ffp_photoel_count * PMTGain,
                                 TMath::Sqrt(fExcessNoiseFactor*ffp_photoel_count));
      
      //Take into account PMT delay and jitter
      Double_t ffp_anode_time = ffp_cathode_time + (Double_t)fRand->Gaus(fPMTDelay, fPMTJitter/TMath::Sqrt(ffp_photoel_count));
      
      if (ffp_amplitude > 0.){
        ERNeuRadFiberPoint* ffPoint = AddFiberPoint(i_point, 0, ffp_lytime, ffp_cathode_time, ffp_anode_time, ffp_photon_count,
                              ffp_photoel_count, ffp_amplitude, 0);
        //Collect Fiber Points by fiber
        frontPointsPerFibers[point_bundle][point_fiber_nb].push_back(ffPoint);
      }
      iTimeSlice++;
    }
    
    iTimeSlice = 1;
    //Моделируем рампространнение сигнала на задние ФЭУ
    while(1){
      Double_t bfp_lytime = point_time+iTimeSlice*fScincilationDT;
      Double_t bfp_cathode_time  = bfp_lytime+(fiber_length-point_z_in_fiber)/cMaterialSpeedOfLight;
      
      Double_t bfp_remainig_photons_count = photon_count*TMath::Exp(-fScincilationDT*(Double_t)iTimeSlice/fScincilationTau);
      
      Double_t bfp_photon_count = photon_count*TMath::Exp(-fScincilationDT*(Double_t)(iTimeSlice+1)/fScincilationTau)
                                      *fScincilationDT/fScincilationTau;
                                    
      Double_t k1 = 0.5-cLightFractionInTotalIntReflection;
      Double_t k2 = cLightFractionInTotalIntReflection;
      bfp_photon_count =  bfp_photon_count*(k1*exp(-(fiber_length-point_z_in_fiber)/0.5) 
                                                   + k2*exp(-(fiber_length-point_z_in_fiber)/200.));
      bfp_remainig_photons_count = bfp_remainig_photons_count*(k1*exp(-point_z_in_fiber/0.5) + k2*exp(-point_z_in_fiber/200.));
      
      Double_t remainingPhotoEl = bfp_remainig_photons_count*PMTQuantumEfficiency/fScincilationTau;
      if(remainingPhotoEl < fPECountForOneElectronsSim){
        //LOG(INFO) << remainingPhotoEl << FairLogger::endl;
        for(Int_t iOnePESignal=0;iOnePESignal<remainingPhotoEl;iOnePESignal++){
          //Прогнозируем времена их появления в ФЭУ, через решение обратной задачи для экспоненциального распределения
          bfp_lytime = point_time+iTimeSlice*fScincilationDT + (-1)*fScincilationTau*TMath::Log(1-fRand->Uniform());
          bfp_cathode_time = bfp_lytime + (fiber_length-point_z_in_fiber)/cMaterialSpeedOfLight;
          Double_t onePESigma = TMath::Sqrt(fExcessNoiseFactor-1)*PMTGain;
          Double_t bfp_amplitude = TMath::Abs(fRand->Gaus(PMTGain,onePESigma));
          Double_t bfp_anode_time = bfp_cathode_time + (Double_t)fRand->Gaus(fPMTDelay, fPMTJitter);
          ERNeuRadFiberPoint* bfPoint = AddFiberPoint(i_point, 1,bfp_lytime, bfp_cathode_time, bfp_anode_time, bfp_photon_count,
                              1, bfp_amplitude, 1);
          backPointsPerFibers[point_bundle][point_fiber_nb].push_back(bfPoint);
        } 
        break;
      }
      
      Double_t bfp_photoel_count = (Double_t)fRand->PoissonD( bfp_photon_count *PMTQuantumEfficiency);
      
      if (bfp_photoel_count < 1.) continue;
      
      Double_t bfp_amplitude = fRand->Gaus(bfp_photoel_count * PMTGain,
                                 TMath::Sqrt(bfp_photoel_count *fExcessNoiseFactor));
                                
      Double_t bfp_anode_time = bfp_cathode_time + (Double_t)fRand->Gaus(fPMTDelay, fPMTJitter/TMath::Sqrt(bfp_photoel_count));
      
      if (bfp_amplitude > 0.){
        ERNeuRadFiberPoint* bfPoint = AddFiberPoint(i_point, 1,bfp_lytime,  bfp_cathode_time, bfp_anode_time, bfp_photon_count,
                              bfp_photoel_count, bfp_amplitude, 0);
        backPointsPerFibers[point_bundle][point_fiber_nb].push_back(bfPoint);
      }
      iTimeSlice++;
    }
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void ERNeuRadDigitizerTS::PMTSignalsAndDigiCreating(Int_t iBundle, Int_t iFiber,
                                std::vector<ERNeuRadFiberPoint* >** frontPointsPerFibers,
                                std::vector<ERNeuRadFiberPoint* >** backPointsPerFibers,
                                Float_t& sumFrontQDC, Float_t& sumBackQDC)
{
  if( frontPointsPerFibers[iBundle][iFiber].size() > 0){
    ERNeuRadPMTSignal* pmtFSignal = AddPMTSignal(iBundle, iFiber);
    for(Int_t iFPoint = 0; iFPoint < frontPointsPerFibers[iBundle][iFiber].size(); iFPoint++){
      ERNeuRadFiberPoint* FPoint = frontPointsPerFibers[iBundle][iFiber][iFPoint];
      pmtFSignal->AddFiberPoint(FPoint);
      //pmtFSignal->AddLink(FairLink("NeuRadFiberPoint",FPoint->Index()));
    }
    pmtFSignal->Generate();
    /*
    if (pmtFSignal->Exist()){
      vector<Double_t> intersections = pmtFSignal->GetIntersections(fDiscriminatorThreshold);
      for (Int_t iInter = 0; iInter < intersections.size(); iInter+=2){
        AddDigi(intersections[iInter], intersections[iInter+1],0., 0.,0.,0.,iFiber);
      }
    }
    */
 }

  if (backPointsPerFibers[iBundle][iFiber].size() > 0){
    ERNeuRadPMTSignal* pmtBSignal =  AddPMTSignal(iBundle, iFiber);
    for(Int_t iFPoint = 0; iFPoint < backPointsPerFibers[iBundle][iFiber].size(); iFPoint++){
      ERNeuRadFiberPoint* FPoint = backPointsPerFibers[iBundle][iFiber][iFPoint];
      pmtBSignal->AddFiberPoint(FPoint);
      //pmtBSignal->AddLink(FairLink("NeuRadFiberPoint",FPoint->Index()));
    }
    pmtBSignal->Generate();
    /*
    if (pmtBSignal->Exist()){
      vector<Double_t> intersections = pmtBSignal->GetIntersections(fDiscriminatorThreshold);
      for (Int_t iInter = 0; iInter < intersections.size(); iInter+=2){
        AddDigi(intersections[iInter], intersections[iInter+1],0., 0.,0.,0.,iFiber);
      }
    }
    */
  }
}
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
ERNeuRadPMTSignal* ERNeuRadDigitizerTS::AddPMTSignal(Int_t iBundle, Int_t iFiber){
  ERNeuRadPMTSignal *pmtSignal = new((*fNeuRadPMTSignal)[PMTSignalCount()])
              ERNeuRadPMTSignalLP(iBundle, iFiber);            
  return pmtSignal;
}
// ----------------------------------------------------------------------------
ClassImp(ERNeuRadDigitizerTS)
