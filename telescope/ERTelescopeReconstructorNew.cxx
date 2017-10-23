#include "ERTelescopeReconstructorNew.h"

#include<iostream>
using namespace std;

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"
#include "TMath.h"

#include "ERSupport.h"
#include "ERHe8EventHeader.h"

// ----------------------------------------------------------------------------
ERTelescopeReconstructorNew::ERTelescopeReconstructorNew()
  : FairTask("Convert cal event to reco event"),
  fRTelescopeEvent(NULL),
  fQTelescopeEvent(NULL),
  fEvent(0)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERTelescopeReconstructorNew::ERTelescopeReconstructorNew(Int_t verbose)
  : FairTask("Convert cal event to reco event", verbose),
  fRTelescopeEvent(NULL),
  fQTelescopeEvent(NULL),
  fEvent(0)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERTelescopeReconstructorNew::~ERTelescopeReconstructorNew()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERTelescopeReconstructorNew::SetParContainers()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus ERTelescopeReconstructorNew::Init()
{

  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  
  //Get input objects
  fQTelescopeEvent = (ERQTelescopeCalEvent*)ioman->GetObject("QTelescopeCalEvent.");
  if (!fQTelescopeEvent)
      Fatal("Init", "Can`t find branch in input file!");

  fRTelescopeEvent = (ERRTelescopeCalEvent*)ioman->GetObject("RTelescopeCalEvent.");
  if (!fRTelescopeEvent)
      Fatal("Init", "Can`t find branch in input file!");
  fBeamDetEvent = (ERBeamDetRecoEvent*)ioman->GetObject("BeamDetRecoEvent.");
  if (!fBeamDetEvent)
      Fatal("Init", "Can`t find branch in input file!");

  fOutEvent = new ERTelescopeRecoEvent();
  ioman->Register("TelescopeRecoEvent.","Analyze",fOutEvent, kTRUE);

  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERTelescopeReconstructorNew::Exec(Option_t* opt)
{
  LOG(DEBUG) << "ERTelescopeReconstructorNew "<< fEvent++ << FairLogger::endl;

  Reset();

  ERParticle* projectile = &(fBeamDetEvent->projectile);
  ERParticle* target = &(fBeamDetEvent->target);
  ERParticle* CM0 = &(fBeamDetEvent->CM0);

  FairRun* run = FairRun::Instance();
  run->MarkFill(kFALSE);
  //Условие отбора
  if(fRTelescopeEvent->mC11==0 && /*fRTelescopeEvent->mC12==0 && */ //Множественность в RTelescope
    fRTelescopeEvent->eC11[0]>0 && fRTelescopeEvent->eC11[0]>0 && // Депозиты в RTelescope
    
    fQTelescopeEvent->mC21==0 && fQTelescopeEvent->mC22==0 &&     //Множественности в QTelescope
    fQTelescopeEvent->mC23<=0 && fQTelescopeEvent->mC24 <=0 &&
    fQTelescopeEvent->mC25<=0 && fQTelescopeEvent->mC26 <=0 &&
    fQTelescopeEvent->eC21[0] >0 && fQTelescopeEvent->eC22[0] >0 //Депозиты в QTelescope
    /* && (fBeamDetEvent->tofb < 115. || fBeamDetEvent->tofb > 135.)*/) // Условия на Tof
  {
    run->MarkFill(kTRUE);
  }
  else
    return;

  //Проекции projectile на первые две плоскости QTelescope
  ERHe8EventHeader* header = (ERHe8EventHeader*)run->GetEventHeader();

  double Zdist;
  if(abs(header->UpMat.MWclosXNum)==1) Zdist = header->UpMat.MWclosDist + header->UpMat.MWXYdist/2.;
  else if(abs(header->UpMat.MWclosXNum)==2) Zdist = header->UpMat.MWclosDist - header->UpMat.MWXYdist/2.;

  fOutEvent->xbd1 = fBeamDetEvent->xmw2 - (Zdist-10.0)*tan(projectile->Part.Theta())*cos(projectile->Part.Phi());
  fOutEvent->ybd1 = fBeamDetEvent->ymw2 - (Zdist-10.0)*tan(projectile->Part.Theta())*sin(projectile->Part.Phi());

  fOutEvent->xbd2 = fBeamDetEvent->xmw2 - (Zdist-25.0)*tan(projectile->Part.Theta())*cos(projectile->Part.Phi());
  fOutEvent->ybd2 = fBeamDetEvent->ymw2 - (Zdist-25.0)*tan(projectile->Part.Theta())*sin(projectile->Part.Phi());

  //Читаем депозиты
  fOutEvent->dep11 = fRTelescopeEvent->eC11[0];
  fOutEvent->dep12 = fRTelescopeEvent->eC12[0];
  fOutEvent->dep21 = fQTelescopeEvent->eC21[0];
  fOutEvent->dep22 = fQTelescopeEvent->eC22[0];
  fOutEvent->dep23 = fQTelescopeEvent->eC23[0];
  fOutEvent->dep24 = fQTelescopeEvent->eC24[0];
  fOutEvent->dep25 = fQTelescopeEvent->eC25[0];
  fOutEvent->dep26 = fQTelescopeEvent->eC26[0];
  fOutEvent->dep2sum = fOutEvent->dep21 + fOutEvent->dep22 + fOutEvent->dep23 + fOutEvent->dep24 + fOutEvent->dep25 + fOutEvent->dep26;
  
  //Вычисляем координаты на RTelescope
  Float_t sector = fRTelescopeEvent->nC11[0];
  Float_t ring = fRTelescopeEvent->nC12[0];

  Float_t nRings = 16.,nSectors = 16.;
  Float_t rmin = 1.6, rmax = 4.1;
  Float_t phi = (sector-0.5)*2.*TMath::Pi()/nSectors; //+ TMath::Pi()/2.;
  Float_t R = rmax-(ring-0.5)*(rmax-rmin)/nRings;
  //cout << phi*TMath::RadToDeg() << " " << R << " " << ring << endl;
  fOutEvent->x11 = R*TMath::Cos(phi);
  fOutEvent->y11 = R*TMath::Sin(phi);

  //From local to global
  fOutEvent->y11 = (-1.)*fOutEvent->y11;

  Float_t z = -10.;
  //Точка на RTelescope
  TVector3 rTelescopeHit(fOutEvent->x11,fOutEvent->y11,z);
  //Точка на мишени
  TVector3 targetHit(fBeamDetEvent->xbt,fBeamDetEvent->ybt,fBeamDetEvent->zbt);
  //Направление протона
  TVector3 pVec = rTelescopeHit - targetHit;
  fOutEvent->th1 = pVec.Theta()*TMath::RadToDeg();
  fOutEvent->phi1 = pVec.Phi()*TMath::RadToDeg();
  //Собираем четырех вектор протона в глобальной СК
  ERParticle ej11;
  Float_t T = fOutEvent->dep11; // Кинетическая энергия протона
  Float_t mass = 938.8; //Масса протона
  Float_t E = T + mass; //Полная энергия протона
  Float_t P = TMath::Sqrt(E*E-mass*mass); //Модуль импульса протона
  ej11.Part.SetPxPyPzE(P*pVec.Unit().X(),P*pVec.Unit().Y(),P*pVec.Unit().Z(),E);
  ej11.Mass = mass;
  ej11.AtMass = 1;
  ej11.AtNumber = 1;
  fOutEvent->t11 = T;
  fOutEvent->ej11 = ej11;
}
//----------------------------------------------------------------------------
void ERTelescopeReconstructorNew::Reset()
{
}
// ---------------------------------------------------------------------------
void ERTelescopeReconstructorNew::Finish()
{   
}
// ----------------------------------------------------------------------------
ClassImp(ERTelescopeReconstructorNew)
