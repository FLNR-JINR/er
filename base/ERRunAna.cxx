#include "ERRunAna.h"


#include <iostream>
using namespace std;

#include "G4RunManager.hh"
#include "G4EmCalculator.hh"
#include "QGSP_BERT.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4ProductionCutsTable.hh"
#include "G4UImanager.hh"


B1DetectorConstruction::B1DetectorConstruction()
: G4VUserDetectorConstruction(){ }

B1DetectorConstruction::~B1DetectorConstruction()
{ }

G4VPhysicalVolume* B1DetectorConstruction::Construct()
{
  G4NistManager* nist = G4NistManager::Instance();
  G4Box* solidWorld = new G4Box("World",1,1,1);
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, world_mat, "World");
  G4VPhysicalVolume* physWorld =  new G4PVPlacement(0, G4ThreeVector(),logicWorld,"World",0,false,0,true);
  return physWorld;
}
B1ActionInitialization::B1ActionInitialization()
 : G4VUserActionInitialization()
{}

B1ActionInitialization::~B1ActionInitialization()
{}

void B1ActionInitialization::BuildForMaster() const
{
}

void B1ActionInitialization::Build() const
{
} 

B1PrimaryGeneratorAction::B1PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(){

}

 B1PrimaryGeneratorAction::~B1PrimaryGeneratorAction()
 {
}

void B1PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
 {
  }

ERRunAna::ERRunAna()
:FairRunAna()
{
	//-----------------------------------------------------------------------
    G4RunManager* runManager = new G4RunManager;
    QGSP_BERT* physlist = new QGSP_BERT(0);
    runManager->SetUserInitialization(physlist);
    B1DetectorConstruction* geom = new B1DetectorConstruction();
    runManager->SetUserInitialization(geom);
    runManager->SetUserAction(new B1PrimaryGeneratorAction());

    G4UImanager* UI = G4UImanager::GetUIpointer();
    UI->ApplyCommand("/run/verbose 0");
    UI->ApplyCommand("/run/particle/verbose 0");
    UI->ApplyCommand("/event/verbose 0");
    UI->ApplyCommand("/tracking/verbose 0"); 
    runManager->Initialize();
    runManager->BeamOn(0);

    G4EmCalculator* calc = new G4EmCalculator();
	G4Material* mat = new G4Material ("MWPC",0.916,1);
	G4Element* h = new G4Element ("H", "H", 1., 1.00794);
	//G4Element* C = new G4Element ("C", "C", 6., 12.0107);
	mat->AddElement (h, 1);
	//mat->AddElement (C, 6);
	cout << calc->GetDEDX(0.5,"proton","G4_AIR") << endl;
}


ClassImp(ERRunAna)

