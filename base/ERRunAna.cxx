#include "ERRunAna.h"


#include <iostream>

#include "TObjArray.h"

#include "G4EmCalculator.hh"
#include "G4NistManager.hh"
#include "G4Proton.hh"

#include "TG4RunConfiguration.h"
#include "TGeant4.h"

#include "FairGeoLoader.h"
#include "FairGeoInterface.h"

#include "ERRecoMCApplication.h"

using namespace std;

ERRunAna::ERRunAna()
:FairRunAna()
{

}

void ERRunAna::Init(){
  
  FairGeoLoader* loader=new FairGeoLoader("TGeo", "Geo Loader");
  FairGeoInterface* GeoInterFace=loader->getGeoInterface();
  GeoInterFace->SetNoOfSets(0);
  TString erPath = gSystem->Getenv("VMCWORKDIR");
  GeoInterFace->setMediaFile(erPath+"/geometry/media.geo");
  GeoInterFace->readMedia();

  ERRecoMCApplication* fApp= new ERRecoMCApplication("Fair","The Fair VMC App",new TObjArray(), erPath+"/geometry/media.geo");

  FairRunAna::Init();

  TG4RunConfiguration* runConfiguration
             = new TG4RunConfiguration("geomRoot", "QGSP_BERT_HP", "specialCuts+stackPopper");

  TGeant4* geant4 = new TGeant4("TGeant4", "The Geant4 Monte Carlo", runConfiguration);

  geant4->ProcessGeantMacro(erPath+"/gconfig/g4config.in");
  geant4->Init();
  geant4->ProcessRun(0);

}
/*Example of energy deposit calculation
  
  G4EmCalculator* calc = new G4EmCalculator();
  G4NistManager* nist = G4NistManager::Instance();
  G4Material* mat = nist->FindOrBuildMaterial("BC408");
  cout << calc->GetDEDX(0.5, G4Proton::Definition(),mat) << endl;
*/
ClassImp(ERRunAna)

