
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
#include "ExpertNeuRadStep.h"
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
  fNeuRadFirstStep = (TClonesArray*) ioman->GetObject("NeuRadFirstStep");
  //todo check
  
  
  // Register output array NeuRadDigi
  fNeuRadDigi = new TClonesArray("ExpertNeuRadDigi",1000);
  ioman->Register("NeuRadDigi", "Digital response in NeuRad", fNeuRadDigi, kTRUE);
  
  fNModules = 64;  //@todo get from parameter class 
  
  // If integration time has not been set otherwise, set it from beam energy - mimicks previous behavior
  if (fTOFRange < 0.01) {
    fTOFRange = BuildTOFRangeFromBeamEnergy(fBeamEnergy);
  }
  
  fhFirstPointELoss       = new TH1F("FirstPointELoss",     "FirstPointELoss, [MeV]", 30, 0., 3.);
  fhSecondPointELoss      = new TH1F("SecondPointELoss",    "SecondPointELoss, [MeV]", 30, 0., 3.);
  fhFristPointLightYeild  = new TH1F("FristPointLightYeild","FristPointLightYeild, [MeV]", 30, 0., 3.);
  fhSecondPointLightYeild = new TH1F("SecondPointLightYeild","SecondPointLightYeild, [MeV]", 30, 0., 3.);
  fhFrontQDCcmXMinusFirst = new TH1F("FrontQDCcmXMinusFirst","FrontQDCcmXMinusFirst, [digi]", 40., 0., 8.);
  fhFrontQDCcmYMinusFirst = new TH1F("FrontQDCcmYMinusFirst","FrontQDCcmYMinusFirst, [digi]", 40., 0., 8.);
  fhBackQDCcmXMinusFirst  = new TH1F("BackQDCcmXMinusFirst", "BackQDCcmXMinusFirst, [digi]", 40., 0., 8.);
  fhBackQDCcmYMinusFirst  = new TH1F("BackQDCcmYMinusFirst", "BackQDCcmYMinusFirst, [digi]", 40., 0., 8.);
  
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
  vector<ExpertNeuRadModulePoint>* frontPointsPerModules = new vector<ExpertNeuRadModulePoint> [fNModules];
  vector<ExpertNeuRadModulePoint>* backPointsPerModules = new vector<ExpertNeuRadModulePoint>  [fNModules];
  
  Double_t module_length = 100.;//[cm] //@todo get from parameter class 
  Double_t module_energyThreshold = 0.000001; //[MeV/ns] threshold on instantaneous current on PMT
  Double_t timeRes = 0.15; //[ns]
  
  Int_t points_count = fNeuRadPoints->GetEntries();
  
  for (Int_t i_point=0; i_point < points_count; i_point++) {
    ExpertNeuRadPoint *point = (ExpertNeuRadPoint*) fNeuRadPoints->At(i_point);
    
    Int_t    point_module_nb  = point->GetModuleInBundleNb();
    Double_t point_eLoss	    =  point->GetEnergyLoss();
    Double_t point_lightYield = point->GetLightYield(); // [MeV]
    Double_t point_x          = point->GetXIn();
    Double_t point_y          = point->GetYIn();
    Double_t point_z          = point->GetZIn();
    Double_t point_time       = point->GetTime();
    Double_t point_z_in_module = point_z + module_length/2.;
    
    if (points_count >1)
      if (i_point == 0){
        fhFirstPointELoss->Fill(point_eLoss/1000.);
        fhFristPointLightYeild->Fill(point_lightYield);
      }
      if (i_point == 1){
        fhSecondPointELoss->Fill(point_eLoss/1000.);
        fhSecondPointLightYeild->Fill(point_lightYield);
      }
    
    LOG(INFO) << "point lightYield = " << point_lightYield << FairLogger::endl;
    
    if (frontPointsPerModules[point_module_nb].size() > ERROR_POINTS_IN_MODULE_COUNT)
       LOG(ERROR) << "ExpertNeuRadDigitizer: Too many points in one module: "
                  << frontPointsPerModules[point_module_nb].size()<< " points" << FairLogger::endl;
    
    ExpertNeuRadModulePoint newFrontModulePoint;
    ExpertNeuRadModulePoint newBackModulePoint;
    
    newFrontModulePoint.time  = point_time+(point_z_in_module)/SPEED_OF_FLIGHT_IN_MATERIAL;
    newBackModulePoint.time   = point_time+(module_length-point_z_in_module)/SPEED_OF_FLIGHT_IN_MATERIAL;
    
    //newFrontModulePoint.lightQDC = point_lightYield*exp(-LIGHT_ATTENUATION*point_z_in_module);
    //newBackModulePoint.lightQDC  = point_lightYield*exp(-LIGHT_ATTENUATION*(module_length-point_z_in_module));
    newFrontModulePoint.lightQDC = point_lightYield*(0.46*exp(-point_z_in_module/0.5) + 0.04*exp(-point_z_in_module/200.));
    newBackModulePoint.lightQDC  = point_lightYield*(0.46*exp(-(module_length-point_z_in_module)/0.5) 
                                                      + 0.04*exp(-(module_length-point_z_in_module)/200.));
    LOG(INFO) << "frontQDC = " <<  newFrontModulePoint.lightQDC << "backQDC = " << newBackModulePoint.lightQDC << FairLogger::endl;
    LOG(INFO) << "point_module_nb" << point_module_nb << FairLogger::endl;
    frontPointsPerModules[point_module_nb].push_back(newFrontModulePoint);
    backPointsPerModules[point_module_nb].push_back(newBackModulePoint);
  }
  
  // sort points according to time; 
  for (Int_t i_module = 0; i_module <fNModules; i_module++){
    sort (frontPointsPerModules[i_module].begin(), frontPointsPerModules[i_module].end(), pointsPerModuleTimeCompare);
    sort (backPointsPerModules[i_module].begin(),  backPointsPerModules[i_module].end(),  pointsPerModuleTimeCompare); 
  }
  
  // Check for leading edge
  Double_t triggerTime=1e100;
  
  for (Int_t i_module = 0; i_module <fNModules; i_module++){
    for (Int_t i_point = 0; i_point < frontPointsPerModules[i_module].size(); i_point++){
      frontPointsPerModules[i_module][i_point].energy = BC408_DECAY_CONSTANT*frontPointsPerModules[i_module][i_point].lightQDC; //instantaneous current on PMT
      backPointsPerModules[i_module][i_point].energy = BC408_DECAY_CONSTANT*backPointsPerModules[i_module][i_point].lightQDC;
      if (i_point > 0){
        Double_t curPoint_frontTime = frontPointsPerModules[i_module][i_point].time;
        Double_t prevPoint_frontTime = frontPointsPerModules[i_module][i_point-1].time;
        Double_t curPoint_backTime = backPointsPerModules[i_module][i_point].time;
        Double_t prevPoint_backTime = backPointsPerModules[i_module][i_point-1].time;
        
        /* 
        Warning!!!!!
        This piece of code is just inherited from R3B Land
        Idea has not adapted for fiber detector
        */
        frontPointsPerModules[i_module][i_point].energy += frontPointsPerModules[i_module][i_point].energy*
                                        exp(-BC408_DECAY_CONSTANT*(curPoint_frontTime - prevPoint_frontTime));
        backPointsPerModules[i_module][i_point].energy += backPointsPerModules[i_module][i_point].energy*
                                        exp(-BC408_DECAY_CONSTANT*(curPoint_backTime - prevPoint_backTime)); 
      }
      
      if (frontPointsPerModules[i_module][i_point].energy > module_energyThreshold)
        if (frontPointsPerModules[i_module][i_point].time < triggerTime)
          triggerTime = frontPointsPerModules[i_module][i_point].time;

      if (backPointsPerModules[i_module][i_point].energy > module_energyThreshold)
        if (backPointsPerModules[i_module][i_point].time < triggerTime)
          triggerTime = backPointsPerModules[i_module][i_point].time;
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
    
    for(Int_t i_point = 0; i_point < frontPointsPerModules[i_module].size(); i_point++) {
      // This is supposed to mimic a QDC and a TDC
      // check if light energy is larger than threshold and register time 
      // Take also time resolution of sigma=timeRes ps into account
      if(frontPointsPerModules[i_module][i_point].energy > module_energyThreshold && !frontTimeTrigerFlag) {
        frontTDC=frontPointsPerModules[i_module][i_point].time+Rnd->Gaus(0.,timeRes);
        frontTimeTrigerFlag = kTRUE;
      }

      if(backPointsPerModules[i_module][i_point].energy > module_energyThreshold && !backTimeTrigerFlag) {
        backTDC=backPointsPerModules[i_module][i_point].time+Rnd->Gaus(0.,timeRes);
        backTimeTrigerFlag = kTRUE;
      }
      
      // if point satisfies time gate for QDC ("Integration time") at runtime
      if(TMath::Abs(frontPointsPerModules[i_module][i_point].time-triggerTime+fTOFRange/2.) < fTOFRange) { 
        frontSumLightQDC += frontPointsPerModules[i_module][i_point].lightQDC;
      }
      
      if(TMath::Abs(backPointsPerModules[i_module][i_point].time-triggerTime+fTOFRange/2.) < fTOFRange) {
        backSumLightQDC += backPointsPerModules[i_module][i_point].lightQDC;
      }
    } // i_point

    if(!(frontTimeTrigerFlag || backTimeTrigerFlag) && backPointsPerModules[i_module].size() > 0) {
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
      LOG(INFO) << "frontSumLightQDC =" << frontSumLightQDC << " backSumLightQDC = " << backSumLightQDC << FairLogger::endl;
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
  if (fVerbose > 1)
    for (Int_t i_digi = 0; i_digi < fNeuRadDigi->GetEntriesFast(); i_digi++){
      ExpertNeuRadDigi* digi = (ExpertNeuRadDigi*) fNeuRadDigi->At(i_digi);
      digi->Print();
    }
  if (fNeuRadDigi->GetEntriesFast() > 0){
    Double_t frontQDC_cm_x  = 0.;
    Double_t backQDC_cm_x   = 0.;
    Double_t frontQDC_cm_y  = 0.;
    Double_t backQDC_cm_y   = 0.;
    Double_t sumFrontQDC    = 0.;
    Double_t sumBackQDC     = 0.;
    
    for (Int_t i_digi =0; i_digi < fNeuRadDigi->GetEntriesFast(); i_digi++){
      ExpertNeuRadDigi* digi = (ExpertNeuRadDigi*)fNeuRadDigi->At(i_digi);
      Int_t digi_module_nr = digi->GetModuleNr();
      digi_module_nr+=1;
      Double_t digi_frontQDC = digi->GetFrontQDC();
      Double_t digi_backQDC = digi->GetBackQDC();
      Int_t x_module_nr = digi_module_nr%8; //@todo Get from Parameter class
      Int_t y_module_nr = digi_module_nr/8 + 1;
      LOG(INFO) << "digi_module_nr = " << digi_module_nr << " x_module_nr = "<< x_module_nr << " y_module_nr = " << y_module_nr << FairLogger::endl;
      frontQDC_cm_x += x_module_nr*digi_frontQDC;
      frontQDC_cm_y += y_module_nr*digi_frontQDC;
      backQDC_cm_x  += x_module_nr*digi_backQDC;
      backQDC_cm_y  += y_module_nr*digi_backQDC;
      sumFrontQDC   += digi_frontQDC;
      sumBackQDC    += digi_backQDC;
    }
    frontQDC_cm_x /= sumFrontQDC;
    frontQDC_cm_y /= sumFrontQDC;
    backQDC_cm_x  /= sumBackQDC;
    backQDC_cm_y  /= sumBackQDC;  
    
    //firstStep in Neurad
    ExpertNeuRadStep* firstStep = (ExpertNeuRadStep*) fNeuRadFirstStep->At(0);
    Int_t module_nr = firstStep->GetModuleInBundleNb();
    module_nr+=1;
    Int_t x_module_nr = module_nr%8; //@todo Get from Parameter class
    Int_t y_module_nr = module_nr/8 + 1;
    LOG(INFO) << "module_nr = " << module_nr << " x_module_nr = "<< x_module_nr << " y_module_nr = " << y_module_nr << FairLogger::endl;
    
    fhFrontQDCcmXMinusFirst->Fill(TMath::Abs(frontQDC_cm_x - x_module_nr));
    fhFrontQDCcmYMinusFirst->Fill(TMath::Abs(frontQDC_cm_y - y_module_nr));
    fhBackQDCcmXMinusFirst->Fill(TMath::Abs(backQDC_cm_x - x_module_nr));
    fhBackQDCcmYMinusFirst->Fill(TMath::Abs(backQDC_cm_y - y_module_nr));
  }
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
  fhFirstPointELoss->Write();
  fhSecondPointELoss->Write();
  fhFristPointLightYeild->Write();
  fhSecondPointLightYeild->Write();
  fhFrontQDCcmXMinusFirst->Write();
  fhFrontQDCcmYMinusFirst->Write();
  fhBackQDCcmXMinusFirst->Write();
  fhBackQDCcmYMinusFirst->Write();
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
