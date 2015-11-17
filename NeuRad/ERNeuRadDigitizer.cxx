
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

const Double_t ERNeuRadDigitizer::BC408_LIGHT_YIELD= 100.; // [1/MeV]
const Double_t ERNeuRadDigitizer::SPEED_OF_LIGHT_IN_MATERIAL = 14.;//[cm/ns]
const Int_t    ERNeuRadDigitizer::ERROR_POINTS_IN_MODULE_COUNT = 1000;
const Double_t ERNeuRadDigitizer::LIGHT_PART_TO_TOTAL_WLS_REFLECTED = 0.04;
//доля света захватываемая файбером в полное внутренне отражение в каждую сторону.
const Double_t ERNeuRadDigitizer::PMT_QUANTUM_EFFICIENCY = 0.3;
const Double_t ERNeuRadDigitizer::PMT_GAIN = 1.5e6;
const Double_t ERNeuRadDigitizer::EXCESS_NOISE_FACTOR = 0.8;
const Double_t ERNeuRadDigitizer::PMT_DELAY=0.1; //[ns]
const Double_t ERNeuRadDigitizer::PMT_JITTER = 0.001; //[ns]

// ----------------------------------------------------------------------------
ERNeuRadDigitizer::ERNeuRadDigitizer()
  : FairTask("ER NeuRad Digitization scheme"),
  fFiberThreshold(0.)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadDigitizer::ERNeuRadDigitizer(Int_t verbose)
  : FairTask("ER NeuRad Digitization scheme ", verbose),
  fFiberThreshold(0.)
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
  TRandom *rand = new TRandom(0);
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
    
    //ffp - front fiber point, bfp - back_fiber_point
    Double_t ffp_cathode_time  = point_time+(point_z_in_fiber)/SPEED_OF_LIGHT_IN_MATERIAL;
    Double_t bfp_cathode_time  = point_time+(fiber_length-point_z_in_fiber)/SPEED_OF_LIGHT_IN_MATERIAL;

	//scintillator light yield
	Int_t ffp_photon_count = point_lightYield/BC408_LIGHT_YIELD;
	Int_t bfp_photon_count = point_lightYield/BC408_LIGHT_YIELD;

    /*
    Two component attenuation for optical fibers. First term is rough 
    approximation for dirrect illumination of a cathode by the source, the 
    second one is for attrenuation of the totally reflected in the WLS. At 
    the ends - half of the light goes to the nearest photocathode 
    */
	Double_t k1 = 0.5-LIGHT_PART_TO_TOTAL_WLS_REFLECTED;
	Double_t k2 = LIGHT_PART_TO_TOTAL_WLS_REFLECTED;

    ffp_photon_count =  (Int_t)point_lightYield*(k1*exp(-point_z_in_fiber/0.5) + k2*exp(-point_z_in_fiber/200.));
    bfp_photon_count =  (Int_t)point_lightYield*(k1*exp(-(fiber_length-point_z_in_fiber)/0.5) 
                                                      + k2*exp(-(fiber_length-point_z_in_fiber)/200.));
	
	//Take into account quantum efficiency. Photoelectrons count
	//@todo Среднее значение квантовой эфективности должно быть разным для каждого файбера
    Int_t ffp_photoel_cathode_count = (Int_t)ffp_photon_count * (Double_t)rand->Poisson(PMT_QUANTUM_EFFICIENCY);
	Int_t bfp_photoel_cathode_count = (Int_t)bfp_photon_count * (Double_t)rand->Poisson(PMT_QUANTUM_EFFICIENCY);
	
	//Take into account pmt gain
    Int_t ffp_photoel_anode_count = (Int_t)rand->Gaus(ffp_photoel_cathode_count * PMT_GAIN,
														ffp_photoel_cathode_count * EXCESS_NOISE_FACTOR);
	Int_t bfp_photoel_anode_count = (Int_t)rand->Gaus(bfp_photoel_cathode_count * PMT_GAIN,
														bfp_photoel_cathode_count * EXCESS_NOISE_FACTOR);
	
	//Take into account PMT delay and jitter
	Double_t ffp_anode_time = ffp_cathode_time + (Double_t)rand->Gaus(PMT_DELAY, PMT_JITTER);
	Double_t bfp_anode_time = bfp_cathode_time + (Double_t)rand->Gaus(PMT_DELAY, PMT_JITTER);
	
	ERNeuRadFiberPoint* ffPoint = AddFiberPoint(0, ffp_cathode_time, ffp_anode_time, ffp_photon_count,
													ffp_photoel_cathode_count, ffp_photoel_anode_count);
	ERNeuRadFiberPoint* bfPoint = AddFiberPoint(1, bfp_cathode_time, bfp_anode_time, bfp_photon_count,
													bfp_photoel_cathode_count, bfp_photoel_anode_count);

	frontPointsPerFibers[point_fiber_nb].push_back(ffPoint);
    backPointsPerFibers[point_fiber_nb].push_back(bfPoint);
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
ERNeuRadFiberPoint* ERNeuRadDigitizer::AddFiberPoint(Int_t side, Double_t cathode_time, Double_t anode_time, 
									Int_t photon_count, Int_t photoel_cathode_count, 
									Int_t photoel_anode_count){
  ERNeuRadFiberPoint *fp = new ((*fNeuRadFiberPoint)[fNeuRadFiberPoint->GetEntriesFast()])
								ERNeuRadFiberPoint(side, cathode_time, anode_time, photon_count, 
													photoel_cathode_count, photoel_anode_count);
}
// ----------------------------------------------------------------------------
ClassImp(ERNeuRadDigitizer)
