/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERParticle.h"

#include <iostream>

using namespace std;

#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"

#include "FairLogger.h"

//-------------------------------------------------------------------------------
ERParticle::ERParticle(TString name, Int_t Z, Int_t A, Int_t Q, Double_t mass/* = 0.*/):
  TNamed(name, name),
  fMass(mass),
  fExcitation(0.)
{
  fPDG = G4IonTable::GetIonTable()->GetNucleusEncoding(Z,A);
}
//-------------------------------------------------------------------------------
ERParticle::ERParticle(TString name, Int_t PDG, Double_t mass/* = 0.*/):
  TNamed(name, name),
  fMass(mass),
  fPDG(PDG),
  fExcitation(0.)
{
}
//-------------------------------------------------------------------------------
ERParticle::~ERParticle()
{
}
//-------------------------------------------------------------------------------
void ERParticle::Print() const
{
  LOG(INFO) << "[ERParticle] "              << fName << " info:" << FairLogger::endl;
  LOG(INFO) << "[ERParticle] PDG: "         << fPDG << FairLogger::endl;
  LOG(INFO) << "[ERParticle] Mass: "        << fMass << FairLogger::endl;
  LOG(INFO) << "[ERParticle] Excitation: "  << fExcitation << FairLogger::endl;
}
//-------------------------------------------------------------------------------
void ERParticle::DefineMass()
{
  if (fMass == 0.)
    fMass = ((G4ParticleDefinition*)G4ParticleTable::GetParticleTable()->FindParticle(fPDG))->GetPDGMass();
}
//-------------------------------------------------------------------------------
