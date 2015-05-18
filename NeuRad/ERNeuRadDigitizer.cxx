
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

using std::sort;

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

const Double_t ERNeuRadDigitizer::DEFAULT_SATURATION_COEFFICIENT = 0.012;
const Double_t ERNeuRadDigitizer::LIGHT_ATTENUATION  = 0.008; // [1/cm]
const Double_t ERNeuRadDigitizer::BC408_DECAY_CONSTANT= 1./2.1; // [1/ns]
const Double_t ERNeuRadDigitizer::SPEED_OF_FLIGHT_IN_MATERIAL = 14.;//[cm/ns]
const Double_t ERNeuRadDigitizer::MEASURING_ERROR_WIDTH_COEF = 0.05;
const Int_t    ERNeuRadDigitizer::ERROR_POINTS_IN_MODULE_COUNT = 1000;


// ----------------------------------------------------------------------------
inline Double_t ERNeuRadDigitizer::BuildTOFRangeFromBeamEnergy(const Double_t &e) // [ns]
{
  if(e > 1100) {
    return 15.;
  }
  if(e < 350) {
    return 13.;
  }
  return 11. + (e / 498. - 1.) * 0.9;
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadDigitizer::ERNeuRadDigitizer()
  : FairTask("ER NeuRad Digitization scheme"),
  fFiberThreshold(0.),fTOFRange(0.),fBeamEnergy(0.),fSaturationCoefficient(DEFAULT_SATURATION_COEFFICIENT)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadDigitizer::ERNeuRadDigitizer(Int_t verbose)
  : FairTask("ER NeuRad Digitization scheme ", verbose),
  fFiberThreshold(0.),fTOFRange(0.),fBeamEnergy(0.),fSaturationCoefficient(DEFAULT_SATURATION_COEFFICIENT)
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

  //fLandDigiPar = (R3BLandDigiPar*)(rtdb->getContainer("R3BLandDigiPar"));

  if ( fVerbose /*&& fLandDigiPar*/ ) {
    LOG(INFO) << "ERNeuRadDigitizer::SetParContainers() "<< FairLogger::endl;
    //LOG(INFO) << "Container R3BLandDigiPar loaded " << FairLogger::endl;
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
  
  // Register output array NeuRadDigi
  fNeuRadDigi = new TClonesArray("ERNeuRadDigi",1000);
  ioman->Register("NeuRadDigi", "Digital response in NeuRad", fNeuRadDigi, kTRUE);
  
  fNFibers = 64;  //@todo get from parameter class 
  
  // If integration time has not been set otherwise, set it from beam energy - mimicks previous behavior
  if (fTOFRange < 0.01) {
    fTOFRange = BuildTOFRangeFromBeamEnergy(fBeamEnergy);
  }
  
  return kSUCCESS;
}
// -------------------------------------------------------------------------

// ----------------------------------------------------------------------------
bool pointsPerFiberTimeCompare (const ERNeuRadFiberPoint& first, const ERNeuRadFiberPoint& second){
  return first.time > second.time;
}
// ----------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERNeuRadDigitizer::Exec(Option_t* opt)
{
  // Reset entries in output arrays
  Reset();
  TRandom3 *Rnd = new TRandom3();
  vector<ERNeuRadFiberPoint>* frontPointsPerFibers = new vector<ERNeuRadFiberPoint> [fNFibers];
  vector<ERNeuRadFiberPoint>* backPointsPerFibers = new vector<ERNeuRadFiberPoint>  [fNFibers];
  
  Double_t fiber_length = 100.;//[cm] //@todo get from parameter class 
  Double_t fiber_energyThreshold = 0.000001; //[MeV/ns] threshold on instantaneous current on PMT
  Double_t timeRes = 0.15; //[ns]
  
  Int_t points_count = fNeuRadPoints->GetEntries();
  
  for (Int_t i_point=0; i_point < points_count; i_point++) {
    ERNeuRadPoint *point = (ERNeuRadPoint*) fNeuRadPoints->At(i_point);
    
    Int_t    point_fiber_nb  = point->GetFiberInBundleNb();
    Double_t point_eLoss	    =  point->GetEnergyLoss();
    Double_t point_lightYield = point->GetLightYield(); // [MeV]
    Double_t point_x          = point->GetXIn();
    Double_t point_y          = point->GetYIn();
    Double_t point_z          = point->GetZIn();
    Double_t point_time       = point->GetTime();
    Double_t point_z_in_fiber = point_z + fiber_length/2.;
    
    
    if (frontPointsPerFibers[point_fiber_nb].size() > ERROR_POINTS_IN_MODULE_COUNT)
       LOG(ERROR) << "ERNeuRadDigitizer: Too many points in one fiber: "
                  << frontPointsPerFibers[point_fiber_nb].size()<< " points" << FairLogger::endl;
    
    ERNeuRadFiberPoint newFrontFiberPoint;
    ERNeuRadFiberPoint newBackFiberPoint;
    
    newFrontFiberPoint.time  = point_time+(point_z_in_fiber)/SPEED_OF_FLIGHT_IN_MATERIAL;
    newBackFiberPoint.time   = point_time+(fiber_length-point_z_in_fiber)/SPEED_OF_FLIGHT_IN_MATERIAL;
    
    //newFrontFiberPoint.lightQDC = point_lightYield*exp(-LIGHT_ATTENUATION*point_z_in_fiber);
    //newBackFiberPoint.lightQDC  = point_lightYield*exp(-LIGHT_ATTENUATION*(fiber_length-point_z_in_fiber));
    /*
    Two component attenuation for optical fibers. First term is rough 
    approximation for dirrect illumination of a cathode by the source, the 
    second one is for attrenuation of the totally reflected in the WLS. At 
    the ends - half of the light goes to the nearest photocathode 
    */
    newFrontFiberPoint.lightQDC = point_lightYield*(0.46*exp(-point_z_in_fiber/0.5) + 0.04*exp(-point_z_in_fiber/200.));
    newBackFiberPoint.lightQDC  = point_lightYield*(0.46*exp(-(fiber_length-point_z_in_fiber)/0.5) 
                                                      + 0.04*exp(-(fiber_length-point_z_in_fiber)/200.));
    frontPointsPerFibers[point_fiber_nb].push_back(newFrontFiberPoint);
    backPointsPerFibers[point_fiber_nb].push_back(newBackFiberPoint);
  }
  
  // sort points according to time; 
  for (Int_t i_fiber = 0; i_fiber <fNFibers; i_fiber++){
    sort (frontPointsPerFibers[i_fiber].begin(), frontPointsPerFibers[i_fiber].end(), pointsPerFiberTimeCompare);
    sort (backPointsPerFibers[i_fiber].begin(),  backPointsPerFibers[i_fiber].end(),  pointsPerFiberTimeCompare); 
  }
  
  // Check for leading edge
  Double_t triggerTime=1e100;
  
  for (Int_t i_fiber = 0; i_fiber <fNFibers; i_fiber++){
    for (Int_t i_point = 0; i_point < frontPointsPerFibers[i_fiber].size(); i_point++){
      frontPointsPerFibers[i_fiber][i_point].energy = BC408_DECAY_CONSTANT*frontPointsPerFibers[i_fiber][i_point].lightQDC; //instantaneous current on PMT
      backPointsPerFibers[i_fiber][i_point].energy = BC408_DECAY_CONSTANT*backPointsPerFibers[i_fiber][i_point].lightQDC;
      if (i_point > 0){
        Double_t curPoint_frontTime = frontPointsPerFibers[i_fiber][i_point].time;
        Double_t prevPoint_frontTime = frontPointsPerFibers[i_fiber][i_point-1].time;
        Double_t curPoint_backTime = backPointsPerFibers[i_fiber][i_point].time;
        Double_t prevPoint_backTime = backPointsPerFibers[i_fiber][i_point-1].time;
        
        /* 
        Warning!!!!!
        This piece of code is just inherited from R3B Land
        Idea has not adapted for fiber detector
        */
        frontPointsPerFibers[i_fiber][i_point].energy += frontPointsPerFibers[i_fiber][i_point].energy*
                                        exp(-BC408_DECAY_CONSTANT*(curPoint_frontTime - prevPoint_frontTime));
        backPointsPerFibers[i_fiber][i_point].energy += backPointsPerFibers[i_fiber][i_point].energy*
                                        exp(-BC408_DECAY_CONSTANT*(curPoint_backTime - prevPoint_backTime)); 
      }
      
      if (frontPointsPerFibers[i_fiber][i_point].energy > fiber_energyThreshold)
        if (frontPointsPerFibers[i_fiber][i_point].time < triggerTime)
          triggerTime = frontPointsPerFibers[i_fiber][i_point].time;

      if (backPointsPerFibers[i_fiber][i_point].energy > fiber_energyThreshold)
        if (backPointsPerFibers[i_fiber][i_point].time < triggerTime)
          triggerTime = backPointsPerFibers[i_fiber][i_point].time;
    } //i_point
  }// i_fiber
    
  Int_t digi_nr = 0;
  for (Int_t i_fiber = 0; i_fiber < fNFibers; i_fiber++){
  
    Double_t frontSumLightQDC = 0.;
    Double_t backSumLightQDC = 0.;
    Double_t frontTDC = 0.;
    Double_t backTDC = 0.;
    Bool_t   frontTimeTrigerFlag = kFALSE;
    Bool_t   backTimeTrigerFlag = kFALSE;
    
    for(Int_t i_point = 0; i_point < frontPointsPerFibers[i_fiber].size(); i_point++) {
      // This is supposed to mimic a QDC and a TDC
      // check if light energy is larger than threshold and register time 
      // Take also time resolution of sigma=timeRes ps into account
      if(frontPointsPerFibers[i_fiber][i_point].energy > fiber_energyThreshold && !frontTimeTrigerFlag) {
        frontTDC=frontPointsPerFibers[i_fiber][i_point].time+Rnd->Gaus(0.,timeRes);
        frontTimeTrigerFlag = kTRUE;
      }

      if(backPointsPerFibers[i_fiber][i_point].energy > fiber_energyThreshold && !backTimeTrigerFlag) {
        backTDC=backPointsPerFibers[i_fiber][i_point].time+Rnd->Gaus(0.,timeRes);
        backTimeTrigerFlag = kTRUE;
      }
      
      // if point satisfies time gate for QDC ("Integration time") at runtime
      if(TMath::Abs(frontPointsPerFibers[i_fiber][i_point].time-triggerTime+fTOFRange/2.) < fTOFRange) { 
        frontSumLightQDC += frontPointsPerFibers[i_fiber][i_point].lightQDC;
      }
      
      if(TMath::Abs(backPointsPerFibers[i_fiber][i_point].time-triggerTime+fTOFRange/2.) < fTOFRange) {
        backSumLightQDC += backPointsPerFibers[i_fiber][i_point].lightQDC;
      }
    } // i_point

    if(!(frontTimeTrigerFlag || backTimeTrigerFlag) && backPointsPerFibers[i_fiber].size() > 0) {
      LOG(INFO) << "In fiber "<< i_fiber << " one side of fiber has not fired !" << FairLogger::endl;
    }
    
    if(frontTimeTrigerFlag && backTimeTrigerFlag) {

      //lightl *= TMath::Exp((2.*(plength))*att/2.);
      //lightr *= TMath::Exp((2.*(plength))*att/2.);
      
      //MA PMT Saturation
      frontSumLightQDC = frontSumLightQDC / (1. + fSaturationCoefficient*frontSumLightQDC);
      backSumLightQDC = backSumLightQDC / (1. + fSaturationCoefficient*backSumLightQDC);

      //measuring error
      frontSumLightQDC = Rnd->Gaus(frontSumLightQDC, MEASURING_ERROR_WIDTH_COEF*frontSumLightQDC);
      backSumLightQDC = Rnd->Gaus(backSumLightQDC, MEASURING_ERROR_WIDTH_COEF*backSumLightQDC);
      LOG(INFO) << "frontSumLightQDC =" << frontSumLightQDC << " backSumLightQDC = " << backSumLightQDC << FairLogger::endl;
      if(frontSumLightQDC < fFiberThreshold || backSumLightQDC < fFiberThreshold) {
        continue;
      }
      //explicitly light yield and quantum что-то not introduced 
      
      Double_t frontQDC = frontSumLightQDC;
      Double_t backQDC  = backSumLightQDC;
      Double_t QDC      = TMath::Sqrt(frontQDC*backQDC);
      Double_t TDC      = (frontTDC + backTDC) / 2. - fiber_length/SPEED_OF_FLIGHT_IN_MATERIAL;

      AddDigi(digi_nr, frontTDC, backTDC, TDC, frontQDC, backQDC, QDC, i_fiber);
      
      digi_nr ++;
    }
  } //i_paddle

  LOG(INFO) << "ERNeuRadDigitizer: produced " << fNeuRadDigi->GetEntries() << " digis" << FairLogger::endl;
  if (fVerbose > 1)
    for (Int_t i_digi = 0; i_digi < fNeuRadDigi->GetEntriesFast(); i_digi++){
      ERNeuRadDigi* digi = (ERNeuRadDigi*) fNeuRadDigi->At(i_digi);
      digi->Print();
    }
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERNeuRadDigitizer::Reset()
{
  if (fNeuRadDigi) {
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
  ERNeuRadDigi *digi = new((*fNeuRadDigi)[fNeuRadDigi->GetEntriesFast()]) ERNeuRadDigi(digi_nr,
                                                                    frontTDC, backTDC, TDC, 
                                                                    frontQDC, backQDC, QDC,
                                                                    fiber_nr);
  return digi;
}
// ----------------------------------------------------------------------------

ClassImp(ERNeuRadDigitizer)
