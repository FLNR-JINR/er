#ifndef ERRunAna_H
#define ERRunAna_H

#include "FairRunAna.h"


#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
class G4VPhysicalVolume;
class G4LogicalVolume;


class B1DetectorConstruction : public G4VUserDetectorConstruction
{
   public:
        B1DetectorConstruction();
        virtual ~B1DetectorConstruction(); 
        virtual G4VPhysicalVolume* Construct();
};


#include "G4VUserActionInitialization.hh"
class B1ActionInitialization : public G4VUserActionInitialization
{
   public:
 B1ActionInitialization();
virtual ~B1ActionInitialization();

virtual void BuildForMaster() const;
virtual void Build() const;
};

#include "G4VUserPrimaryGeneratorAction.hh"

 class B1PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
B1PrimaryGeneratorAction();    
 virtual ~B1PrimaryGeneratorAction();
 virtual void GeneratePrimaries(G4Event*);         
 };

class ERRunAna : public FairRunAna
{

  public:
    virtual ~ERRunAna();
    ERRunAna();
    
    ClassDef(ERRunAna ,1)

};

#endif
