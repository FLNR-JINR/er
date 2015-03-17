
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

#include "ExpertNeuRadPoint.h"
#include "ExpertNeuRadDigitizer.h"

const Double_t ExpertNeuRadDigitizer::DEFAULT_SATURATION_COEFFICIENT = 0.012;
const Double_t ExpertNeuRadDigitizer::LIGHT_ATTENUATION  = 0.008; // [1/cm]
const Double_t ExpertNeuRadDigitizer::BC408_DECAY_CONSTANT= 1./2.1; // [1/ns]
const Double_t ExpertNeuRadDigitizer::SPEED_OF_FLIGHT_IN_MATERIAL = 14.;//[cm/ns]
const Double_t ExpertNeuRadDigitizer::MEASURING_ERROR_WIDTH_COEF = 0.05;
const Int_t    ExpertNeuRadDigitizer::ERROR_POINTS_IN_MODULE_COUNT = 1000;


// ----------------------------------------------------------------------------
inline Double_t ExpertNeuRadDigitizer::BuildTOFRangeFromBeamEnergy(const Double_t &e) // [ns]
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
ExpertNeuRadDigitizer::ExpertNeuRadDigitizer()
  : FairTask("Expert NeuRad Digitization scheme"),
  fModuleThreshold(0.),fTOFRange(0.),fBeamEnergy(0.),fSaturationCoefficient(DEFAULT_SATURATION_COEFFICIENT)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ExpertNeuRadDigitizer::ExpertNeuRadDigitizer(Int_t verbose)
  : FairTask("Expert NeuRad Digitization scheme ", verbose),
  fModuleThreshold(0.),fTOFRange(0.),fBeamEnergy(0.),fSaturationCoefficient(DEFAULT_SATURATION_COEFFICIENT)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ExpertNeuRadDigitizer::~ExpertNeuRadDigitizer()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ExpertNeuRadDigitizer::SetParContainers()
{
  // Get run and runtime database
  FairRunAna* run = FairRunAna::Instance();
  if ( ! run ) Fatal("SetParContainers", "No analysis run");

  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  if ( ! rtdb ) Fatal("SetParContainers", "No runtime database");

  //fLandDigiPar = (R3BLandDigiPar*)(rtdb->getContainer("R3BLandDigiPar"));

  if ( fVerbose /*&& fLandDigiPar*/ ) {
    LOG(INFO) << "ExpertNeuRadDigitizer::SetParContainers() "<< FairLogger::endl;
    //LOG(INFO) << "Container R3BLandDigiPar loaded " << FairLogger::endl;
  }
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus ExpertNeuRadDigitizer::Init()
{
  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  fNeuRadPoints = (TClonesArray*) ioman->GetObject("NeuRadPoint");
  
  // Register output array NeuRadDigi
  fNeuRadDigi = new TClonesArray("ExpertNeuRadDigi",1000);
  ioman->Register("NeuRadDigi", "Digital response in NeuRad", fNeuRadDigi, kTRUE);
  
  fNModules = 64;  //@todo get from parameter class 
  fFrontPointsPerModules = new vector<ExpertNeuRadModulePoint> [fNModules];
  fBackPointsPerModules = new vector<ExpertNeuRadModulePoint>  [fNModules];
  
  // If integration time has not been set otherwise, set it from beam energy - mimicks previous behavior
  if (fTOFRange < 0.01) {
    fTOFRange = BuildTOFRangeFromBeamEnergy(fBeamEnergy);
  }
  
  return kSUCCESS;
}
// -------------------------------------------------------------------------

// ----------------------------------------------------------------------------
bool pointsPerModuleTimeCompare (const ExpertNeuRadModulePoint& first, const ExpertNeuRadModulePoint& second){
  return first.time > second.time;
}
// ----------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ExpertNeuRadDigitizer::Exec(Option_t* opt)
{
  // Reset entries in output arrays
  Reset();
  TRandom3 *Rnd = new TRandom3();
  
  Double_t module_length = 100.;//[cm] //@todo get from parameter class 
  Double_t module_energyThreshold = 0.001; //[MeV/ns] threshold on instantaneous current on PMT
  Double_t timeRes = 0.15; //[ns]
  
  Int_t points_count = fNeuRadPoints->GetEntries();
  
  for (Int_t i_point=0; i_point < points_count; i_point++) {
    ExpertNeuRadPoint *point = (ExpertNeuRadPoint*) fNeuRadPoints->At(i_point);
    
    Int_t    point_module_nb  = point->GetModuleInBundleNb();
    Double_t point_lightYield = point->GetLightYield(); // [MeV]
    Double_t point_x          = point->GetXIn();
    Double_t point_y          = point->GetYIn();
    Double_t point_z          = point->GetZIn();
    Double_t point_time       = point->GetTime();
    Double_t point_z_in_module = point_z + module_length/2.;
    
    if (fFrontPointsPerModules[point_module_nb].size() > ERROR_POINTS_IN_MODULE_COUNT)
       LOG(ERROR) << "ExpertNeuRadDigitizer: Too many points in one module: "
                  << fFrontPointsPerModules[point_module_nb].size()<< " points" << FairLogger::endl;
    
    ExpertNeuRadModulePoint newFrontModulePoint;
    ExpertNeuRadModulePoint newBackModulePoint;
    
    newFrontModulePoint.time  = point_time+(point_z_in_module)/SPEED_OF_FLIGHT_IN_MATERIAL;
    newBackModulePoint.time   = point_time+(module_length-point_z_in_module)/SPEED_OF_FLIGHT_IN_MATERIAL;
    
    //newFrontModulePoint.lightQDC = point_lightYield*exp(-LIGHT_ATTENUATION*point_z_in_module);
    //newBackModulePoint.lightQDC  = point_lightYield*exp(-LIGHT_ATTENUATION*(module_length-point_z_in_module));
    newFrontModulePoint.lightQDC = point_lightYield*(0.46*exp(-point_z_in_module/0.5) + 0.04*exp(-point_z_in_module/200.));
    newBackModulePoint.lightQDC  = point_lightYield*(0.46*exp(-(module_length-point_z_in_module)/0.5) 
                                                      + 0.04*exp(-(module_length-point_z_in_module)/200.));
    
    fFrontPointsPerModules[point_module_nb].push_back(newFrontModulePoint);
    fBackPointsPerModules[point_module_nb].push_back(newBackModulePoint);
  }
  
  // sort points according to time; 
  for (Int_t i_module = 0; i_module <fNModules; i_module++){
    sort (fFrontPointsPerModules[i_module].begin(), fFrontPointsPerModules[i_module].end(), pointsPerModuleTimeCompare);
    sort (fBackPointsPerModules[i_module].begin(),  fBackPointsPerModules[i_module].end(),  pointsPerModuleTimeCompare); 
  }
  
  // Check for leading edge
  Double_t triggerTime=1e100;
  
  for (Int_t i_module = 0; i_module <fNModules; i_module++){
    for (Int_t i_point = 0; i_point < fFrontPointsPerModules[i_module].size(); i_point++){
      fFrontPointsPerModules[i_module][i_point].energy = BC408_DECAY_CONSTANT*fFrontPointsPerModules[i_module][i_point].lightQDC; //instantaneous current on PMT
      fBackPointsPerModules[i_module][i_point].energy = BC408_DECAY_CONSTANT*fBackPointsPerModules[i_module][i_point].lightQDC;
      if (i_point > 0){
        Double_t curPoint_frontTime = fFrontPointsPerModules[i_module][i_point].time;
        Double_t prevPoint_frontTime = fFrontPointsPerModules[i_module][i_point-1].time;
        Double_t curPoint_backTime = fBackPointsPerModules[i_module][i_point].time;
        Double_t prevPoint_backTime = fBackPointsPerModules[i_module][i_point-1].time;
        
        /* 
        Warning!!!!!
        This piece of code is just inherited from R3B Land
        Idea has not adapted for fiber detector
        */
        fFrontPointsPerModules[i_module][i_point].energy += fFrontPointsPerModules[i_module][i_point].energy*
                                        exp(-BC408_DECAY_CONSTANT*(curPoint_frontTime - prevPoint_frontTime));
        fBackPointsPerModules[i_module][i_point].energy += fBackPointsPerModules[i_module][i_point].energy*
                                        exp(-BC408_DECAY_CONSTANT*(curPoint_backTime - prevPoint_backTime)); 
      }
      
      if (fFrontPointsPerModules[i_module][i_point].energy > module_energyThreshold)
        if (fFrontPointsPerModules[i_module][i_point].time < triggerTime)
          triggerTime = fFrontPointsPerModules[i_module][i_point].time;

      if (fBackPointsPerModules[i_module][i_point].energy > module_energyThreshold)
        if (fBackPointsPerModules[i_module][i_point].time < triggerTime)
          triggerTime = fBackPointsPerModules[i_module][i_point].time;
    } //i_point
  }// i_module
    
  Int_t digi_nr = 0;
  for (Int_t i_module = 0; i_module < fNModules; i_module++){
  
    Double_t frontSumLightQDC = 0.;
    Double_t backSumLightQDC = 0.;
    Double_t frontTDC = 0.;
    Double_t backTDC = 0.;
    Bool_t   frontTimeTrigerFlag = kFALSE;
    Bool_t   backTimeTrigerFlag = kFALSE;
    
    for(Int_t i_point = 0; i_point < fFrontPointsPerModules[i_module].size(); i_point++) {
      // This is supposed to mimic a QDC and a TDC
      // check if light energy is larger than threshold and register time 
      // Take also time resolution of sigma=timeRes ps into account
      if(fFrontPointsPerModules[i_module][i_point].energy > module_energyThreshold && !frontTimeTrigerFlag) {
        frontTDC=fFrontPointsPerModules[i_module][i_point].time+Rnd->Gaus(0.,timeRes);
        frontTimeTrigerFlag = kTRUE;
      }

      if(fBackPointsPerModules[i_module][i_point].energy > module_energyThreshold && !backTimeTrigerFlag) {
        backTDC=fBackPointsPerModules[i_module][i_point].time+Rnd->Gaus(0.,timeRes);
        backTimeTrigerFlag = kTRUE;
      }
      
      // if point satisfies time gate for QDC ("Integration time") at runtime
      if(TMath::Abs(fFrontPointsPerModules[i_module][i_point].time-triggerTime+fTOFRange/2.) < fTOFRange) { 
        frontSumLightQDC += fFrontPointsPerModules[i_module][i_point].lightQDC;
      }
      
      if(TMath::Abs(fBackPointsPerModules[i_module][i_point].time-triggerTime+fTOFRange/2.) < fTOFRange) {
        backSumLightQDC += fBackPointsPerModules[i_module][i_point].lightQDC;
      }
    } // i_point

    if(!(frontTimeTrigerFlag || backTimeTrigerFlag) && fBackPointsPerModules[i_module].size() > 0) {
      LOG(INFO) << "In module "<< i_module << " one side of module has not fired !" << FairLogger::endl;
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

      if(frontSumLightQDC < fModuleThreshold || backSumLightQDC < fModuleThreshold) {
        continue;
      }
      //explicitly light yield and quantum что-то not introduced 
      
      Double_t frontQDC = frontSumLightQDC;
      Double_t backQDC  = backSumLightQDC;
      Double_t QDC      = TMath::Sqrt(frontQDC*backQDC);
      Double_t TDC      = (frontTDC + backTDC) / 2. - module_length/SPEED_OF_FLIGHT_IN_MATERIAL;

      AddDigi(digi_nr, frontTDC, backTDC, TDC, frontQDC, backQDC, QDC, i_module);
      
      digi_nr ++;
    }
  } //i_paddle

  LOG(INFO) << "ExpertNeuRadDigitizer: produced " << fNeuRadDigi->GetEntries() << " digis" << FairLogger::endl;
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ExpertNeuRadDigitizer::Reset()
{
  if (fNeuRadDigi) {
    fNeuRadDigi->Clear();
  }
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ExpertNeuRadDigitizer::Finish()
{
  if (fVerbose > 1)
    for (Int_t i_digi = 0; i_digi < fNeuRadDigi->GetEntriesFast(); i_digi++){
      ExpertNeuRadDigi* digi = (ExpertNeuRadDigi*) fNeuRadDigi->At(i_digi);
      digi->Print();
    }
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ExpertNeuRadDigi* ExpertNeuRadDigitizer::AddDigi(Int_t digi_nr, Double_t frontTDC, Double_t backTDC,
                                      Double_t TDC, Double_t frontQDC, Double_t backQDC, Double_t QDC,
                                      Int_t module_nr)
{
  ExpertNeuRadDigi *digi = new((*fNeuRadDigi)[fNeuRadDigi->GetEntriesFast()]) ExpertNeuRadDigi(digi_nr,
                                                                    frontTDC, backTDC, TDC, 
                                                                    frontQDC, backQDC, QDC,
                                                                    module_nr);
  return digi;
}
// ----------------------------------------------------------------------------

ClassImp(ExpertNeuRadDigitizer)
