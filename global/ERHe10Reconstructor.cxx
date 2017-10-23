#include "ERHe10Reconstructor.h"

#include<iostream>

#include "TVector3.h"

#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRuntimeDb.h"

#include "ERParticle.h"

using namespace std;

// ----------------------------------------------------------------------------
ERHe10Reconstructor::ERHe10Reconstructor()
  : FairTask("ER He10 reconstruction scheme")
,fBeamDetEvent(NULL)
,fRTelescopeEvent(NULL)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERHe10Reconstructor::ERHe10Reconstructor(Int_t verbose)
  : FairTask("ER He10 reconstruction scheme", verbose)
,fBeamDetEvent(NULL)
,fRTelescopeEvent(NULL)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERHe10Reconstructor::~ERHe10Reconstructor()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERHe10Reconstructor::SetParContainers()
{
  // Get run and runtime database
  FairRun* run = FairRun::Instance();
  if ( ! run ) Fatal("SetParContainers", "No analysis run");

  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  if ( ! rtdb ) Fatal("SetParContainers", "No runtime database");
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus ERHe10Reconstructor::Init()
{
  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  
  fRTelescopeEvent = (ERTelescopeRecoEvent*)ioman->GetObject("TelescopeRecoEvent.");
  if (!fRTelescopeEvent)
      Fatal("Init", "Can`t find branch in input file!");
  fBeamDetEvent = (ERBeamDetRecoEvent*)ioman->GetObject("BeamDetRecoEvent.");
  if (!fBeamDetEvent)
      Fatal("Init", "Can`t find branch in input file!");

  fOutEvent = new ERHe10RecoEvent();
  ioman->Register("He10RecoEvent.","Analyze",fOutEvent, kTRUE);
   
  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERHe10Reconstructor::Exec(Option_t* opt)
{
  ERParticle* projectile = &(fBeamDetEvent->projectile);
  ERParticle* target = &(fBeamDetEvent->target);
  ERParticle* CM0 = &(fBeamDetEvent->CM0);

  ERParticle* ej11 = &(fRTelescopeEvent->ej11);
  //Собираем четырех вектор mis He10
  Float_t Mmis = fMisMass; //Масса He10
  //Импульс миса
  TVector3 Pmis(projectile->Part.Px()-ej11->Part.Px(),
                projectile->Part.Py()-ej11->Part.Py(),
                projectile->Part.Pz()-ej11->Part.Pz());
  //***************** Подход СИ *****************************/
  //Полная энергия миса
  Float_t Emis =  TMath::Sqrt(Pmis.Mag2()+pow(Mmis,2));
  //Кинетическая энергия миса
  Float_t Tmis = Emis-Mmis;
  //Энергия возбуждения миса
  Float_t Exmis = projectile->Part.E()+target->Mass-ej11->Part.E()-Tmis-Mmis;

  //Пересчет с текущим приближением Exmis
  Emis = TMath::Sqrt(Pmis.Mag2()+pow(Mmis+Exmis,2));
  Tmis = Emis-Mmis-Exmis;
  Exmis = projectile->Part.E()+target->Mass-ej11->Part.E()-Tmis-Mmis;
  //Формируем выходной объект
  ERParticle mis;
  mis.Mass = Mmis;
  mis.Excitation = Exmis;
  mis.Part.SetVect(Pmis);
  mis.Part.SetE(Tmis+Exmis+Mmis);
  fOutEvent->mis11 = mis;
  //************************************************************/
  Emis = projectile->Part.E()+target->Mass-ej11->Part.E();
  Exmis= sqrt(pow(Emis,2)- Pmis.Mag2()) - Mmis;
  ERParticle misNew;
  misNew.Mass = Mmis;
  misNew.Excitation = Exmis;
  misNew.Part.SetVect(Pmis);
  misNew.Part.SetE(Emis);
  fOutEvent->mis11n = misNew;
  //************************************************************/
  //Бустируем все в центр масс первой реакции и заносим в дерево
  projectile->Part.Boost(-CM0->Part.BoostVector());
  target->Part.Boost(-CM0->Part.BoostVector());
  ej11->Part.Boost(-CM0->Part.BoostVector());
  mis.Part.Boost(-CM0->Part.BoostVector());
  misNew.Part.Boost(-CM0->Part.BoostVector());
  fOutEvent->mis11cm0 = mis;
  fOutEvent->mis11ncm0 = misNew;
  fOutEvent->ej11cm0 = (*ej11);
  fOutEvent->projcm0 = (*projectile);
  fOutEvent->targetcm0 = (*target);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERHe10Reconstructor::Reset()
{    
  fOutEvent->Reset();
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERHe10Reconstructor::Finish()
{   

}
// ----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
ClassImp(ERHe10Reconstructor)
