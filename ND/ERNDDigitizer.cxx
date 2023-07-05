#include "ERNDDigitizer.h"

#include "TVector3.h"
#include "TGeoMatrix.h"
#include "TMath.h"
#include "TRandom3.h"

#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"

#include <limits>
#include <vector>
#include <map>

#include "ERDetectorList.h"
#include "ERNDPoint.h"

Int_t ERNDDigitizer::fEvent = 0;
// ----------------------------------------------------------------------------
ERNDDigitizer::ERNDDigitizer()
  : FairTask("ER ND Digi producing scheme")
,fNDPoints(NULL)
,fNDDigis(NULL)
,fEdepErrorA(0)
,fEdepErrorB(0)
,fEdepErrorC(0)
,fLYErrorA(0)
,fLYErrorB(0)
,fLYErrorC(0)
,fQuenchThreshold(0)
,fLYThreshold(0)
,fProbabilityB(0)
,fProbabilityC(0)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNDDigitizer::ERNDDigitizer(Int_t verbose)
  : FairTask("ER ND Digi producing scheme ", verbose)
,fNDPoints(NULL)
,fNDDigis(NULL)
,fEdepErrorA(0)
,fEdepErrorB(0)
,fEdepErrorC(0)
,fLYErrorA(0)
,fLYErrorB(0)
,fLYErrorC(0)
,fQuenchThreshold(0)
,fLYThreshold(0)
,fProbabilityB(0)
,fProbabilityC(0)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNDDigitizer::~ERNDDigitizer()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERNDDigitizer::SetParContainers()
{
  // Get run and runtime database
  FairRun* run = FairRun::Instance();
  if ( ! run ) Fatal("SetParContainers", "No analysis run");

  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  if ( ! rtdb ) Fatal("SetParContainers", "No runtime database");
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus ERNDDigitizer::Init()
{
  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  
  fNDPoints = (TClonesArray*) ioman->GetObject("NDPoint");
  if (!fNDPoints)
    Fatal("Init", "Can`t find collection NDPoint!"); 

  // Register output array fNDDigis
  fNDDigis = new TClonesArray("ERNDDigi",10);

  ioman->Register("NDDigi", "ND digi", fNDDigis, kTRUE);

  fSetup = ERNDSetup::Instance();
   
  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERNDDigitizer::Exec(Option_t* opt)
{
  
  LOG(DEBUG2) << "ERNDDigitizer event: " << fEvent++ << FairLogger::endl;
  Reset();

  //Sort points by crystalls
  std::map<Int_t, std::vector<Int_t>> pointsByCrystall;
  for (Int_t iPoint = 0; iPoint < fNDPoints->GetEntriesFast(); iPoint++){
    ERNDPoint* point = (ERNDPoint*)fNDPoints->At(iPoint);
    pointsByCrystall[point->StilbenNr()].push_back(iPoint);
  }

  //loop over crystall with points
  for (const auto &itCrystall : pointsByCrystall){
    Float_t edep = 0;   // sum edep in crystall
    Float_t edep_n1= 0; 
    Float_t edep_n2= 0; 
    Float_t edep_mis= 0;
    Float_t time = std::numeric_limits<float>::max();   // first time in crystall
    Float_t ly = 0.;    // sum ligth yield in crystall
    Float_t x_in = 0.;
    Float_t y_in = 0.;
    Float_t z_in = 0.;
    Int_t parentTrackID = -2;

    //loop over points in crysrall itCrystall.first()
    for (const auto iPoint : itCrystall.second){
      ERNDPoint* point = (ERNDPoint*)fNDPoints->At(iPoint);
      edep += point->GetEnergyLoss();
      ly += point->LightYield();
      if (point->GetTime() < time){	// time 1,2 - заполнять по parenttrackID case switch
        time = point->GetTime();
        //sonya:new variables x_in,y_in,z_in - enter point of neutron (proton in fact) in stilben
        x_in = point->GetXIn();
        y_in = point->GetYIn();
        z_in = point->GetZIn();
      }
      //if(point->GetMot0TrackID()==2)	printf("*********================\nPoint from 1st neutron\n");
      
      parentTrackID = point->GetParentTrackID();
     // printf("******** parent track id %i\n",parentTrackID);
      switch (parentTrackID)
      {
         case 3:         
            edep_n1+=point->GetEnergyLoss();
            break;
         case 4:
            edep_n2+=point->GetEnergyLoss();
            break;
         default:
            edep_mis+=point->GetEnergyLoss();
      }
    }
	
    Float_t edepSigma = sqrt(pow(fEdepErrorA,2) + pow(fEdepErrorB*TMath::Sqrt(edep),2) + pow(fEdepErrorC*edep,2));	
//    Float_t edepSigma = sqrt(pow(fEdepErrorA,2) + pow(fEdepErrorB*TMath::Sqrt(edep/1000.),2) + pow(fEdepErrorC*edep,2));
    edep = gRandom->Gaus(edep, edepSigma);

	/*fEdepErrorA [MeV], fEdepErrorB  [Sqrt(MeV)],  fEdepErrorC  [dimensionless], e.g. if fEdepErrorA = 0.01 MeV, fEdepErrorB = 0.02 MeV^0.5, fEdepErrorC= 0.03 for Edep=2.0 [MeV] sigma is equal Sqrt(0.01^2+(0.02*(2.0^0.5))^2+(0.03*2.0)^2) = 0.067 MeV
    The errors for LY are set in a similar way*/
    /*
    fEdepErrorB and fLYErrorB are expressed in per cent divided by square root of edep or LY,
    expressed in MeVlike it is usually done in detector description. e.g. for Edep=0.001 (i.e. 1 Mev)
    sigma is equal0.04*0.001Gev = 0.04 MeV
    */

    Float_t lySigma = sqrt(pow(fLYErrorA,2) + pow(fLYErrorB*TMath::Sqrt(ly),2) + pow(fLYErrorC*ly,2));
//    Float_t lySigma = sqrt(pow(fLYErrorA,2) + pow(fLYErrorB*TMath::Sqrt(ly/1000.),2) + pow(fLYErrorC*ly,2));
    ly = gRandom->Gaus(ly, lySigma);

    Float_t timeSigma = TMath::Sqrt(fTimeErrorA/ly);
    time = gRandom->Gaus(time, timeSigma);

    Float_t neutronProb = NeutronProbability(edep,ly);
  
    AddDigi(itCrystall.first,edep, edep_n1, edep_n2, edep_mis, parentTrackID, ly, time, neutronProb,x_in,y_in,z_in);	//sonya:new variables x_in,y_in,z_in - enter point of neutron (proton in fact) in stilben
  }
  LOG(DEBUG) << "Digis count: " << fNDDigis->GetEntriesFast() << FairLogger::endl;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERNDDigitizer::Reset()
{
  if (fNDDigis) {
    fNDDigis->Delete();
  }
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERNDDigitizer::Finish()
{   
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNDDigi* ERNDDigitizer::AddDigi(Int_t stilbenNb, Float_t edep, Float_t edep_n1, Float_t edep_n2, Float_t edep_mis, Int_t parentTrackID, Float_t ly,
                                 Float_t time,Float_t neutronProb,Float_t x_in,Float_t y_in,Float_t z_in)
{
  ERNDDigi *Digi = new((*fNDDigis)[fNDDigis->GetEntriesFast()])
              ERNDDigi(stilbenNb, edep, edep_n1, edep_n2, edep_mis, parentTrackID, ly, time, neutronProb,x_in,y_in,z_in);
  return Digi;
}
// ----------------------------------------------------------------------------
Float_t ERNDDigitizer::NeutronProbability(Float_t edep, Float_t ly){
  Float_t neutronProb;
  Float_t quench = ly/edep;
  if ((ly > fLYThreshold) && (quench < fQuenchThreshold)){
    neutronProb = 1.;
  }
  if ((ly < fLYThreshold) && (quench < fQuenchThreshold)){
    neutronProb = fProbabilityB+(1-fProbabilityB)*(ly/fLYThreshold);
  }
  if ((ly > fLYThreshold) && (quench > fQuenchThreshold)){
    neutronProb = 0.;
  }
  if ((ly < fLYThreshold) && (quench > fQuenchThreshold)){
    neutronProb = fProbabilityC*(1-ly/fLYThreshold);
  }
  return neutronProb;
}
//-----------------------------------------------------------------------------
ClassImp(ERNDDigitizer)
