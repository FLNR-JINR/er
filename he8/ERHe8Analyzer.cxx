#include "ERHe8Analyzer.h"

#include<iostream>
using namespace std;

#include "TGeoMaterial.h"
#include "TGeoMedium.h"
#include "TGeoCompositeShape.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

#include "ERSupport.h"

//----------------------------------------------------------------------------
ERHe8Analyzer::ERHe8Analyzer()
  : FairTask("ERHe8Analyzer"),
  fEvent(0),
  EjMat(NULL),
  Ntelescopes(0),
  NTelMax(5),
  NLayMax(10),
  NDetMax(20),
  NDivMax(0),
  NDivXYMax(0),
  Det(NULL),
  layer(NULL),
  NDet(NULL),
  target(NULL),
  projectile(NULL),
  CM0(NULL),
  fBeamDetRecoEvent(NULL),
  fDsrdEvent(NULL),
  fTelescopeEvent(NULL),
  mis(NULL),
  tel(new ERTelData()),
  inclu(new ERInclusiveData())
{
}
//----------------------------------------------------------------------------
ERHe8Analyzer::ERHe8Analyzer(Int_t verbose)
  : FairTask("ERHe8Analyzer", verbose),
  fEvent(0),
  EjMat(NULL),
  Ntelescopes(0),
  NTelMax(5),
  NLayMax(10),
  NDetMax(20),
  NDivMax(0),
  NDivXYMax(0),
  Det(NULL),
  layer(NULL),
  NDet(NULL),
  target(NULL),
  projectile(NULL),
  CM0(NULL),
  fBeamDetRecoEvent(NULL),
  fDsrdEvent(NULL),
  mis(NULL),
  fTelescopeEvent(NULL),
  tel(new ERTelData()),
  inclu(new ERInclusiveData())
{
}
//----------------------------------------------------------------------------
ERHe8Analyzer::~ERHe8Analyzer(){
}
//----------------------------------------------------------------------------
void ERHe8Analyzer::SetParContainers(){
}
//----------------------------------------------------------------------------
InitStatus ERHe8Analyzer::Init(){

  //Register input/output objects
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  
  //Get input objects
  fBeamDetRecoEvent = (ERBeamDetRecoEvent*)ioman->GetObject("BeamDetRecoEvent.");
  if (!fBeamDetRecoEvent)
      Fatal("Init", "Can`t find branch in input file!");

  fDsrdEvent = (ERDsrdCalEvent*)ioman->GetObject("DsrdCalEvent.");
  if (!fDsrdEvent)
      Fatal("Init", "Can`t find branch in input file!");

  fTelescopeEvent = (ERTelescopeCalEvent*)ioman->GetObject("TelescopeCalEvent.");
  if (!fTelescopeEvent)
      Fatal("Init", "Can`t find branch in input file!");

  ioman->Register("tel.", "Analys",tel, kTRUE);
  ioman->Register("inclu.", "Analys",inclu, kTRUE);

  FairRun* run = FairRun::Instance();
  header = (ERHe8EventHeader*)run->GetEventHeader();

  projectile = &(fBeamDetRecoEvent->projectile);
  target = &(fBeamDetRecoEvent->target);
  CM0 = &(fBeamDetRecoEvent->CM0);

  ReadTelescopeParameters();

  InitMemory();

  InitParticlesInOutputs();

  CheckInOutAZ();
  
  CreateTelescopeGeometry();

  ReadDeDx();

  TelescopeThresholds();

  PrintReaction();

  return kSUCCESS;
}
//----------------------------------------------------------------------------
void ERHe8Analyzer::CheckInOutAZ(){
  cout << "Check for Zin=Zout and Ain=Aout" << endl;
  int InAtNumber = projectile->AtNumber + target->AtNumber;
  int OutAtNumber = 0;
  for(int ip=0;ip<header->NofDetPart+header->NofUnObsPart;ip++) OutAtNumber += ejectile[0][ip][0].AtNumber;

  if(InAtNumber!=OutAtNumber) {printf("In ReactionInput.dat Zin is not equal to Zout!!!\n");}

  InAtNumber = projectile->AtMass + target->AtMass;
  OutAtNumber = 0;
  for(int ip=0;ip<header->NofDetPart+header->NofUnObsPart;ip++) OutAtNumber += ejectile[0][ip][0].AtMass;
  if(InAtNumber!=OutAtNumber) {printf("In ReactionInput.dat Ain is not equal to Aout!!!\n");}
}
//----------------------------------------------------------------------------
void ERHe8Analyzer::InitParticlesInOutputs(){
    ERParticle intermed[header->NofDetPart+header->NofUnObsPart]; // любая промежуточная частица
    TString filePath  = gSystem->Getenv("VMCWORKDIR") + TString("/input/StableNuclei.dat");
    char* WayMass= const_cast<char*>(filePath.Data());
    //projectile->ParticleID(header->projname,WayMass);
    //target->ParticleID(header->tarname,WayMass);
    //target->Part.SetPxPyPzE(0.,0.,0.,target->Mass);

    WhatParticlesInOut(&intermed[0],header->DetectedPart,header->NofDetPart);
    WhatParticlesInOut(&intermed[header->NofDetPart],header->UnObservedPart,header->NofUnObsPart);  

    cout << "Define particles in the output channel" << endl;
    
    for(int it=0;it<Ntelescopes;it++)
    {
      for(int ip=0;ip<header->NofDetPart+header->NofUnObsPart;ip++)
      {
        for(int imu=0;imu<NDivXYMax;imu++)
        {
          //cout << intermed[ip].NameOfNucleus << endl;
          ejectile[it][ip][imu].NameOfNucleus = new char [strlen(intermed[ip].NameOfNucleus)+1];
          strcpy(ejectile[it][ip][imu].NameOfNucleus,intermed[ip].NameOfNucleus);
          ejectile[it][ip][imu].ParticleID(ejectile[it][ip][imu].NameOfNucleus,WayMass);
          mis[it][ip][imu].NameOfNucleus = new char [2];
          strcpy(mis[it][ip][imu].NameOfNucleus,"mi");
        }
        //PlayEjectile[ip].NameOfNucleus = new char [5];
        //strcpy(PlayEjectile[ip].NameOfNucleus,ejectile[0][ip][0].NameOfNucleus);
        //PlayEjectile[ip].Mass = ejectile[0][ip][0].Mass;
      }
    }

    for(int it=0;it<Ntelescopes;it++)
    {
      for(int ip=0;ip<header->NofDetPart;ip++)
      {
        for(int imu=0;imu<NDivXYMax;imu++)
        {
          for(int ipx=0;ipx<header->NofDetPart+header->NofUnObsPart;ipx++)
          {
            if(ip!=ipx) mis[it][ip][imu].Mass += ejectile[0][ipx][0].Mass;
          }
        }
      }
    }
    for(int it=0;it<Ntelescopes;it++)
    {
      for(int itx=0;itx<Ntelescopes;itx++)
      {
        if(it!=itx)
        {
          for(int ip=0;ip<header->NofDetPart;ip++)
          {
            for(int ipx=0;ipx<header->NofDetPart;ipx++)
            {
              if(ip!=ipx)
              {
                participants[it][ip][itx][ipx].NameOfNucleus = new char [2];
                strcpy(participants[it][ip][itx][ipx].NameOfNucleus,"pa");
                spectator[it][ip][itx][ipx].NameOfNucleus = new char [2];
                strcpy(spectator[it][ip][itx][ipx].NameOfNucleus,"sp");
                participants[it][ip][itx][ipx].Mass = 
                  ejectile[0][ip][0].Mass + ejectile[0][ipx][0].Mass;
                for(int ipy=0;ipy<header->NofDetPart+header->NofUnObsPart;ipy++)
                {
                  if(ipy!=ipx&&ipy!=ip)
                  spectator[it][ip][itx][ipx].Mass += ejectile[0][ipy][0].Mass;
                }
              }
            }
          }
        }
      }
    }
}
//------------------------------------------------------------------------
void ERHe8Analyzer::InitMemory(){
  mp = new int[Ntelescopes];
  // Ntelescopes NLayMax NDetMax
  mpd = new int**[Ntelescopes];
  MuX = new int**[Ntelescopes];
  MuY = new int**[Ntelescopes];
  MuXT = new int**[Ntelescopes];
  MuYT = new int**[Ntelescopes];
  xbdet0 = new double**[Ntelescopes];
  ybdet0 = new double**[Ntelescopes];
  for (int i =0; i < Ntelescopes; i++){
    mpd[i] = new int*[NLayMax];
    MuX[i] = new int*[NLayMax];
    MuY[i] = new int*[NLayMax];
    MuXT[i] = new int*[NLayMax];
    MuYT[i] = new int*[NLayMax];
    xbdet0[i] = new double*[NLayMax];
    ybdet0[i] = new double*[NLayMax];
    for (int j = 0; j < NLayMax; j++){
      mpd[i][j] = new int[NDetMax];
      MuX[i][j] = new int[NDetMax];
      MuY[i][j] = new int[NDetMax];
      MuXT[i][j] = new int[NDetMax];
      MuYT[i][j] = new int[NDetMax];
      xbdet0[i][j] = new double[NDetMax];
      ybdet0[i][j] = new double[NDetMax];
    }
  }
  //Ntelescopes NLayMax
  TCheck = new int*[Ntelescopes];
  cx = new double*[Ntelescopes];
  cy = new double*[Ntelescopes];
  cz = new double*[Ntelescopes];
  AngleDet = new TVector3*[Ntelescopes];
  for (int i = 0; i < Ntelescopes;i++){
    TCheck[i] = new int[NLayMax];
    cx[i] = new double[NLayMax];
    cy[i] = new double[NLayMax];
    cz[i] = new double[NLayMax];
    AngleDet[i] = new TVector3[NLayMax];
  }
  //Ntelescopes NLayMax NDetMax NDivXYMax
  
  deposit = new double***[Ntelescopes];
  DepoX = new double***[Ntelescopes];
  DepoY = new double***[Ntelescopes];
  NhitX = new int***[Ntelescopes];
  NhitY = new int***[Ntelescopes];
  NhitXT = new int***[Ntelescopes];
  NhitYT = new int***[Ntelescopes];
  for (int i =0; i <Ntelescopes; i++){
    deposit[i] = new double**[NLayMax];
    DepoX[i] = new double**[NLayMax];
    DepoY[i] = new double**[NLayMax];
    NhitX[i] = new int**[NLayMax];
    NhitY[i] = new int**[NLayMax];
    NhitXT[i] = new int**[NLayMax];
    NhitYT[i] = new int**[NLayMax];
    for (int j = 0; j < NLayMax; j++){
      deposit[i][j] = new double*[NDetMax];
      DepoX[i][j] = new double*[NDetMax];
      DepoY[i][j] = new double*[NDetMax];
      NhitX[i][j] = new int*[NDetMax];
      NhitY[i][j] = new int*[NDetMax];
      NhitXT[i][j] = new int*[NDetMax];
      NhitYT[i][j] = new int*[NDetMax];
      for (int k = 0; k < NDetMax; k++){
        deposit[i][j][k] = new double[NDivXYMax];
        DepoX[i][j][k] = new double[NDivXYMax];
        DepoY[i][j][k] = new double[NDivXYMax];
        NhitX[i][j][k] = new int[NDivXYMax];
        NhitY[i][j][k] = new int[NDivXYMax];
        NhitXT[i][j][k] = new int[NDivXYMax];
        NhitYT[i][j][k] = new int[NDivXYMax];
      }
    }
  }

  //Ntelescopes NLayMax header->NofDetPart NDivXYMax
  al = new double***[Ntelescopes];
  for (int i =0; i <Ntelescopes; i++){
    al[i] = new double**[NLayMax];
    for (int j = 0; j < NLayMax; j++){
      al[i][j] = new double*[header->NofDetPart];
      for (int k = 0; k < header->NofDetPart; k++){
        al[i][j][k] = new double[NDivXYMax];
      }
    }
  }

  // Ntelescopes header->NofDetPart Ntelescopes header->NofDetPart
  spectator = new ERParticle***[Ntelescopes];
  participants = new ERParticle***[Ntelescopes];
  for (int i = 0; i<Ntelescopes; i++){
    spectator[i] = new ERParticle**[header->NofDetPart];
    participants[i] = new ERParticle**[header->NofDetPart];
    for (int j = 0; j < header->NofDetPart; j++){
      spectator[i][j] = new ERParticle*[Ntelescopes];
      participants[i][j] = new ERParticle*[Ntelescopes];
      for (int k = 0; k < Ntelescopes; k++){
        spectator[i][j][k] = new ERParticle[header->NofDetPart];
        participants[i][j][k] = new ERParticle[header->NofDetPart];
      }
    }
  }

  //Ntelescopes NLayMax NDetMax header->NofDetPart
  HitX = new int***[Ntelescopes];
  HitY = new int***[Ntelescopes];
  HitXT = new int***[Ntelescopes];
  HitYT = new int***[Ntelescopes];
  for (int i = 0; i<Ntelescopes; i++){
    HitX[i] = new int**[NLayMax];
    HitY[i] = new int**[NLayMax];
    HitXT[i] = new int**[NLayMax];
    HitYT[i] = new int**[NLayMax];
    for (int j = 0; j < NLayMax; j++){
      HitX[i][j] = new int*[NDetMax];
      HitY[i][j] = new int*[NDetMax];
      HitXT[i][j] = new int*[NDetMax];
      HitYT[i][j] = new int*[NDetMax];
      for (int k = 0; k < NDetMax; k++){
        HitX[i][j][k] = new int[header->NofDetPart];
        HitY[i][j][k] = new int[header->NofDetPart];
        HitXT[i][j][k] = new int[header->NofDetPart];
        HitYT[i][j][k] = new int[header->NofDetPart];
      }
    }
  }
  //Ntelescopes header->NofDetPart+header->NofUnObsPart
  ejectile = new ERParticle**[Ntelescopes];
  for (int i=0; i<Ntelescopes; i++){
    ejectile[i] = new ERParticle*[header->NofDetPart+header->NofUnObsPart];
    for (int j=0; j<header->NofDetPart+header->NofUnObsPart; j++)
      ejectile[i][j] = new ERParticle[NDivXYMax];
  }
  //Ntelescopes header->NofDetPart+header->NofUnObsPart NDivXYMax
  mis = new ERParticle**[Ntelescopes];
  for (int i = 0; i<Ntelescopes; i++){
    mis[i] = new ERParticle*[header->NofDetPart+header->NofUnObsPart];
    for (int j = 0; j<header->NofDetPart+header->NofUnObsPart; j++){
      mis[i][j] = new ERParticle[NDivXYMax];
    }
  }

  cout << "Memory inited" << endl;
}
//----------------------------------------------------------------------------
TVector3 ERHe8Analyzer::VertexPosition(TVector3 V1,TVector3 V2,TVector3 V3,TVector3 V4){
  double x1 = V3.X();
  double y1 = V3.Y();
  double z1 = V3.Z();

  double x2 = V4.X();
  double y2 = V4.Y();
  double z2 = V4.Z();

  double axn = (V1.Cross(V2)).Unit().X();
  double ayn = (V1.Cross(V2)).Unit().Y();
  double azn = (V1.Cross(V2)).Unit().Z();

  double ax1 = V1.Unit().X();
  double ay1 = V1.Unit().Y();
  double az1 = V1.Unit().Z();

  double ax2 = V2.Unit().X();
  double ay2 = V2.Unit().Y();
  double az2 = V2.Unit().Z();

  double A = ((az2*axn-ax2*azn)*(axn*ay1-ax1*ayn)/(axn*ay2-ax2*ayn) + (ax1*azn-axn*az1))/axn;
  double B = (ayn*(az2*axn-ax2*azn)/(axn*ay2-ax2*ayn)-azn)*(x2-x1)/axn;
  B = B+z2-z1-(az2*axn-ax2*azn)*(y2-y1)/(axn*ay2-ax2*ayn);
  double t0 = -B/A;
  double s0 = ((axn*ay1-ax1*ayn)*t0+ayn*(x2-x1)-axn*(y2-y1))/(axn*ay2-ax2*ayn);

  V1.SetXYZ((ax1*t0+x1+ax2*s0+x2)/2.,(ay1*t0+y1+ay2*s0+y2)/2.,(az1*t0+z1+az2*s0+z2)/2.);
  return V1;
}
// -------------------------------------------------------------------------
TVector3 ERHe8Analyzer::Traject(Telescope* Dx,Telescope* Dy,int Nx,int Ny,TVector3 Vint){
  TVector3 Px,Py,VecDX,VecDY;
  double x,y,x0,x1,x2,x3,y0,y1,y2,y3,z0,z1,z2,z3,arba;
  double R,Phi,x_1,y_1,z_1,x_2,y_2,z_2,x_3,y_3,z_3;
  double t11,t12,t13,t21,t22,t23,t31,t32,t33,A,B,D;

  TRandom Xrnd;
  if(Nx>0&&Ny>0&&Nx<=abs(Dx->NstripX)&&Ny<=abs(Dy->NstripY))
  {
    x3 = Vint.X();y3 = Vint.Y();z3 = Vint.Z();

    if(Dx->DetLabVect.Mag()==Dy->DetLabVect.Mag())
    {
      if(!strcmp(Dx->StripFB,"XY")||!strcmp(Dx->StripFB,"RS"))
      {
        VecDX = Dx->DetLabVect - Vint;
        VecDY = Dx->DetLabVect + Dx->Thick*Dx->DetOwnAxisZ.Unit() - Vint;
      }
      if(!strcmp(Dy->StripFB,"YX")||!strcmp(Dy->StripFB,"SR"))
      {
        VecDY = Dx->DetLabVect - Vint;
        VecDX = Dx->DetLabVect + Dx->Thick*Dx->DetOwnAxisZ.Unit() - Vint;
      }
    }
    if(Dx->DetLabVect.Mag()!=Dy->DetLabVect.Mag())
    {
      if(!strcmp(Dx->StripFB,"XY")||!strcmp(Dx->StripFB,"RS")) VecDX = Dx->DetLabVect - Vint;
      if(!strcmp(Dx->StripFB,"YX")||!strcmp(Dx->StripFB,"SR")) VecDX=Dx->DetLabVect+Dx->Thick*Dx->DetOwnAxisZ.Unit() - Vint;
      if(!strcmp(Dy->StripFB,"YX")||!strcmp(Dy->StripFB,"SR")) VecDY = Dy->DetLabVect - Vint;
      if(!strcmp(Dy->StripFB,"XY")||!strcmp(Dy->StripFB,"RS")) VecDY=Dy->DetLabVect+Dy->Thick*Dy->DetOwnAxisZ.Unit() - Vint;
    }
    if(!strcmp(Dy->Shape,"square"))
    {
      arba = (rand() %10000)/10000.;
      y = (double(Ny)-abs(Dy->NstripY)/2.-1.+arba)*Dy->SizeY/Dy->NstripY;
      Py = y*Dy->DetOwnAxisY.Unit() + VecDY + Vint;
      x1 = Py.X();y1 = Py.Y();z1 = Py.Z();
      Py += Dy->DetOwnAxisX.Unit();
      x2 = Py.X();y2 = Py.Y();z2 = Py.Z();
    }
    if(!strcmp(Dy->Shape,"sector")||!strcmp(Dy->Shape,"annular"))
    {

      arba = (rand() %10000)/10000.;
      if(Dy->Nsector>0) Phi = (double(Ny)-1.+arba)*360./Dy->Nsector;
      else if(Dy->Nsector<0) Phi = 360.+(double(Ny)-1.+arba)*360./Dy->Nsector;

      Py = cos(Phi*rad)*Dy->DetOwnAxisX + sin(Phi*rad)*Dy->DetOwnAxisY;
      Px = Py + VecDY + Vint;
      x1 = Px.X();y1 = Px.Y();z1 = Px.Z();
      Px -= Py;
      x2 = Px.X();y2 = Px.Y();z2 = Px.Z();
    }
    Px = VecDX + Vint;
    x0 = Px.X();y0 = Px.Y();z0 = Px.Z();
    t11 = Dx->DetOwnAxisX.Unit().X();
    t21 = Dx->DetOwnAxisX.Unit().Y();
    t31 = Dx->DetOwnAxisX.Unit().Z();
    t12 = Dx->DetOwnAxisY.Unit().X();
    t22 = Dx->DetOwnAxisY.Unit().Y();
    t32 = Dx->DetOwnAxisY.Unit().Z();
    t13 = Dx->DetOwnAxisZ.Unit().X();
    t23 = Dx->DetOwnAxisZ.Unit().Y();
    t33 = Dx->DetOwnAxisZ.Unit().Z();
    x_1 = t11*(x1-x0) + t21*(y1-y0) + t31*(z1-z0);
    y_1 = t12*(x1-x0) + t22*(y1-y0) + t32*(z1-z0);
    z_1 = t13*(x1-x0) + t23*(y1-y0) + t33*(z1-z0);
    x_2 = t11*(x2-x0) + t21*(y2-y0) + t31*(z2-z0);
    y_2 = t12*(x2-x0) + t22*(y2-y0) + t32*(z2-z0);
    z_2 = t13*(x2-x0) + t23*(y2-y0) + t33*(z2-z0);
    x_3 = t11*(x3-x0) + t21*(y3-y0) + t31*(z3-z0);
    y_3 = t12*(x3-x0) + t22*(y3-y0) + t32*(z3-z0);
    z_3 = t13*(x3-x0) + t23*(y3-y0) + t33*(z3-z0);
    A = y_1*(z_2-z_3) - y_2*(z_1-z_3) + y_3*(z_1-z_2);
    B = z_1*(x_2-x_3) - z_2*(x_1-x_3) + z_3*(x_1-x_2);
    D = x_1*(y_2*z_3-y_3*z_2)-x_2*(y_1*z_3-y_3*z_1)+x_3*(y_1*z_2-y_2*z_1);
    A = -A/B; B = D/B;
    if(!strcmp(Dx->Shape,"annular")||!strcmp(Dx->Shape,"sector"))
    {
      arba = (rand() %10000)/10000.;
      if(Dx->Nring>0) R=(double(Nx)-1.+arba)*(Dx->Rout-Dx->Rin)/Dx->Nring+Dx->Rin;
      else if(Dx->Nring<0) R=(double(abs(Dx->Nring)-Nx)+arba)*(Dx->Rout-Dx->Rin)/abs(Dx->Nring)+Dx->Rin;
      x1 = (-A*B+sqrt(R*R*(1+A*A)-B*B))/(1+A*A);
      y1 = A*x1 + B;
      x2 = (-A*B-sqrt(R*R*(1+A*A)-B*B))/(1+A*A);
      y2 = A*x2 + B;
      Px = x1*Dx->DetOwnAxisX.Unit() + y1*Dx->DetOwnAxisY.Unit();
      if(Px*Py<0) Px = x2*Dx->DetOwnAxisX.Unit() + y2*Dx->DetOwnAxisY.Unit();
    }
    if(!strcmp(Dx->Shape,"square"))
    {
      arba = (rand() %10000)/10000.;
      x = (double(Nx)-abs(Dx->NstripX)/2.-1.+arba)*Dx->SizeX/Dx->NstripX;
      y = A*x + B;
      Px = x*Dx->DetOwnAxisX.Unit() + y*Dx->DetOwnAxisY.Unit();
    }
    Px += VecDX;
    //**** Px is a vector coinciding with a particle trajectory!!!
  } 
  return Px;
}
//----------------------------------------------------------------------------
void ERHe8Analyzer::Exec(Option_t* opt){
  cout << "Event "  << fEvent++ << endl;

  Reset();

  projectile = &(fBeamDetRecoEvent->projectile);
  target = &(fBeamDetRecoEvent->target);
  CM0 = &(fBeamDetRecoEvent->CM0);

  for(int it=0;it<Ntelescopes;it++)
  {
    for(int il=0;il<layer[it];il++)
    {
      for(int id=0;id<NDet[it][il];id++)
      {
        for(int ip=0;ip<NDivXYMax;ip++) deposit[it][il][id][ip] = 0.;
      }
    }
  }

  MuY[0][0][0]=fDsrdEvent->mC11;MuX[0][1][0]=fDsrdEvent->mC12;
  MuY[1][0][0]=fTelescopeEvent->mC21;MuX[1][1][0]=fTelescopeEvent->mC22;MuX[1][2][0]=fTelescopeEvent->mC23;

  for(int imu=0;imu<=MuY[0][0][0];imu++)
  {
    DepoX[0][0][0][imu] = fDsrdEvent->eC11[imu];
    NhitY[0][0][0][imu] = fDsrdEvent->nC11[imu];
    NhitX[0][0][0][imu] = 1;
  }
  for(int imu=0;imu<=MuX[0][1][0];imu++)
  {
    DepoX[0][1][0][imu] = fDsrdEvent->eC12[imu];
    NhitX[0][1][0][imu] = fDsrdEvent->nC12[imu];
    NhitY[0][1][0][imu] = 1;
  }
  for(int imu=0;imu<=MuY[1][0][0];imu++)
  {
    DepoX[1][0][0][imu] = fTelescopeEvent->eC21[imu];
    NhitY[1][0][0][imu] = fTelescopeEvent->nC21[imu];
    NhitX[1][0][0][imu] = 1;
  }
  for(int imu=0;imu<=MuX[1][1][0];imu++)
  {
    DepoX[1][1][0][imu] = fTelescopeEvent->eC22[imu];
    NhitX[1][1][0][imu] = fTelescopeEvent->nC22[imu];
    NhitY[1][1][0][imu] = 1;
  }
  for(int imu=0;imu<=MuX[1][2][0];imu++)
  {
    DepoX[1][2][0][imu] = fTelescopeEvent->eC23[imu];
    NhitX[1][2][0][imu] = fTelescopeEvent->nC23[imu];
  }
  //cout << "Condition" << endl;
  if(MuY[0][0][0]==0&&NhitY[0][0][0][0]>0&&NhitY[0][0][0][0]<=abs(Det[0][0][0].NstripY)&&DepoX[0][0][0][0]>0.) mpd[0][0][0]=MuY[0][0][0]+1;

  if(MuX[0][1][0]==0&&NhitX[0][1][0][0]>0&&NhitX[0][1][0][0]<=abs(Det[0][1][0].NstripX)&&DepoX[0][1][0][0]>0.) mpd[0][1][0]=MuX[0][1][0]+1;

  if(MuY[1][0][0]==0&&NhitY[1][0][0][0]>0&&NhitY[1][0][0][0]<=abs(Det[1][0][0].NstripY)&&DepoX[1][0][0][0]>0.) mpd[1][0][0]=MuY[1][0][0]+1;
  
  if(MuX[1][1][0]==0&&NhitX[1][1][0][0]>0&&NhitX[1][1][0][0]<=abs(Det[1][1][0].NstripX)&&DepoX[1][1][0][0]>0.) mpd[1][1][0]=MuX[1][1][0]+1;

  if(mpd[0][0][0]==1&&mpd[0][1][0]==1) mp[0] = mpd[0][0][0];
  if(mpd[1][0][0]==1&&mpd[1][1][0]==1) mp[1] = mpd[1][0][0];

  header->mp1 = mp[0]; header->mp2 = mp[1]; header->mp3 = mp[2];
  if(mp[0]&&header->mtrack&&header->mbeam) good_mp0++;
  if(mp[1]&&header->mtrack&&header->mbeam) good_mp1++;

  int NxX1,NyX1,NxY1,NyY1,NxX2,NyX2,NxY2,NyY2;
  double dst,minX1,minY1,minX2,minY2;
  double dstmin = 1000.;
  int Nstep = 20;
  double Delta = 0.05;
  double tarcoord[3];
  //this one for He8 experiment
  if(!header->ReIN.Vertex&&!header->ReIN.DetectorTune)
  {
    // Typical case: no vertices:
    for(int it=0;it<Ntelescopes;it++)
    {
      if(mp[it]==1)
      {
          Vert1.SetXYZ(fBeamDetRecoEvent->xbt,fBeamDetRecoEvent->ybt,fBeamDetRecoEvent->zbt);
          if(it==2) AngleDet[it][0] = Traject(&Det[it][0][0],&Det[it][0][0],NhitX[it][0][0][0],NhitY[it][1][0][0],Vert1);
          else AngleDet[it][0] = Traject(&Det[it][1][0],&Det[it][0][0],NhitX[it][1][0][0],NhitY[it][0][0][0],Vert1);
          cx[it][0] = (Vert1+AngleDet[it][0]).X();
          cy[it][0] = (Vert1+AngleDet[it][0]).Y();
          cz[it][0] = (Vert1+AngleDet[it][0]).Z();
        }
      }
  } /* if(!ReIN.Vertex&&!ReIN.DetectorTune) */

  //cout << "Now we know trajectories" << endl;
  char* plett;
  int itx,ilx,idx,ipx,is,count;
  char OutputChannelTemp[32];
  for(int it=0;it<Ntelescopes;it++)
      {
        if(mp[it]==1&&header->mbeam&&header->mtrack)
        {
          trajectory = geom->InitTrack(fBeamDetRecoEvent->xbt,fBeamDetRecoEvent->ybt,fBeamDetRecoEvent->zbt,
            AngleDet[it][0].Unit().X(),AngleDet[it][0].Unit().Y(),AngleDet[it][0].Unit().Z());
          while(!geom->IsOutside())
          {
            itx=-1;ilx=-1;idx=-1;is=-1;
            const char *path=geom->GetPath();
            trajectory = geom->FindNextBoundary();
            if(strcmp(path,"/Top_1")) 
            {
              range = geom->GetStep();
              plett=const_cast<char*>(strchr(path,'t')); plett++;
              plett=strtok(plett,"_");
              strcpy(OutputChannelTemp,plett);
              if(strcmp(OutputChannelTemp,"gas")&&strcmp(OutputChannelTemp,"foilex")&&
                strcmp(OutputChannelTemp,"foilen")&&strcmp(OutputChannelTemp,"TargetCell")&&
                strcmp(OutputChannelTemp,"HscrFoil")&&strcmp(OutputChannelTemp,"HeatScreen"))
              {
                itx = int(atoi(OutputChannelTemp)/pow(10,strlen(OutputChannelTemp)-2));
                ilx = int((atoi(OutputChannelTemp)-itx*pow(10,strlen(OutputChannelTemp)-2))/
                  pow(10,strlen(OutputChannelTemp)-4));
                idx = int((atoi(OutputChannelTemp)-itx*pow(10,strlen(OutputChannelTemp)-2)-ilx*
                  pow(10,strlen(OutputChannelTemp)-4))/pow(10,strlen(OutputChannelTemp)-6));
                is = int(atoi(OutputChannelTemp)-itx*pow(10,strlen(OutputChannelTemp)-2)-ilx*
                  pow(10,strlen(OutputChannelTemp)-4)-idx*pow(10,strlen(OutputChannelTemp)-6));
                if(is==0&&DepoX[itx][ilx][0][0]>0.1) deposit[itx][ilx][idx][0] = range;
              }
              if(!strcmp(OutputChannelTemp,"gas")) deposit[0][0][0][1] = range*header->UpMat.TarPress*TempNorm/header->UpMat.TarTemp;
              if(!strcmp(OutputChannelTemp,"foilex")) deposit[0][0][0][2] = range;
              if(!strcmp(OutputChannelTemp,"foilen")) deposit[0][0][0][3] = range;
              if(!strcmp(OutputChannelTemp,"TargetCell")) deposit[0][0][0][4] = range;
              if(!strcmp(OutputChannelTemp,"HscrFoil")) deposit[0][0][0][5] = range;
            }
            trajectory = geom->Step();
          } /* while(!geom->IsOutside) */
          for(int ip=0;ip<header->NofDetPart;ip++)
          {
            ilx = -1;count=-1;Tb = 0.;Ta=0.;
            for(int il=layer[it]-1;il>=0;il--)
            {
              for(int id=0;id<NDet[it][il];id++)
              {
                if(!strcmp(Det[it][il][id].Matt,"si")&&deposit[it][il][id][0]!=0.) idx = id;
              }
              if(count==-1) Tb = DepoX[it][il][0][0];

              if(count==-1&&!strcmp(Det[it][il][idx].Matt,"si"))
                Tb = EiEo(EjMat[ip]->ej_si,Tb,-deposit[it][il][idx][0]*Det[it][il][idx].DeadZoneF/Det[it][il][idx].Thick);

              if(count>-1&&Tb>0.1)
              {
                Tb = EiEo(EjMat[ip]->ej_si,Tb,-deposit[it][il][idx][0]*Det[it][il][idx].DeadZoneB/Det[it][il][idx].Thick);
                Tb += DepoX[it][il][0][0];
                Tb = EiEo(EjMat[ip]->ej_si,Tb,-deposit[it][il][idx][0]*Det[it][il][idx].DeadZoneF/Det[it][il][idx].Thick);
              }
              if(Tb>0.1) count++;
            } /* for(il=layer[it]-1;il=0;il--) */
            if(deposit[0][0][0][5]>0.&&Tb>0.1) Tb = EiEo(EjMat[ip]->ej_TARwin,Tb,-deposit[0][0][0][5]);
            if(deposit[0][0][0][3]>0.&&Tb>0.1) Tb = EiEo(EjMat[ip]->ej_TARwin,Tb,-deposit[0][0][0][3]);
            if(deposit[0][0][0][2]>0.&&Tb>0.1) Tb = EiEo(EjMat[ip]->ej_TARwin,Tb,-deposit[0][0][0][2]);
            if(deposit[0][0][0][4]>0.&&Tb>0.1) Tb = EiEo(EjMat[ip]->ej_TARwin,Tb,-deposit[0][0][0][4]);
            if(deposit[0][0][0][1]>0.&&Tb>0.1) Tb = EiEo(EjMat[ip]->ej_target,Tb,-deposit[0][0][0][1]);
            if(Tb>0.1)
            {
              Ta = sqrt(pow(Tb+ejectile[it][ip][0].Mass,2)-pow(ejectile[it][ip][0].Mass,2));
              ejectile[it][ip][0].Part.SetPxPyPzE(Ta*sin(AngleDet[it][0].Theta())*cos(AngleDet[it][0].Phi()),
                Ta*sin(AngleDet[it][0].Theta())*sin(AngleDet[it][0].Phi()),Ta*cos(AngleDet[it][0].Theta()),Tb+ejectile[it][ip][0].Mass);
            }
            // Calibration
            Tb = ejectile[it][ip][0].Part.E()-ejectile[it][ip][0].Mass;
            trajectory = geom->InitTrack(fBeamDetRecoEvent->xbt,fBeamDetRecoEvent->ybt,fBeamDetRecoEvent->zbt,
              AngleDet[it][0].Unit().X(),AngleDet[it][0].Unit().Y(),AngleDet[it][0].Unit().Z());
            while(!geom->IsOutside())
            {
              itx=-1;ilx=-1;idx=-1;Ta=0.;
              const char *path=geom->GetPath();
              trajectory = geom->FindNextBoundary();
              if(strcmp(path,"/Top_1")) 
              {
                range = geom->GetStep();
                plett=const_cast<char*>(strchr(path,'t')); plett++;
                plett=strtok(plett,"_");
                strcpy(OutputChannelTemp,plett);
                if(strcmp(OutputChannelTemp,"gas")&&strcmp(OutputChannelTemp,"foilex")&&
                  strcmp(OutputChannelTemp,"foilen")&&strcmp(OutputChannelTemp,"TargetCell")&&
                  strcmp(OutputChannelTemp,"HscrFoil")&&strcmp(OutputChannelTemp,"HeatScreen"))
                {
                  itx = int(atoi(OutputChannelTemp)/pow(10,strlen(OutputChannelTemp)-2));
                  ilx = int((atoi(OutputChannelTemp)-itx*pow(10,strlen(OutputChannelTemp)-2))/
                    pow(10,strlen(OutputChannelTemp)-4));
                  idx = int((atoi(OutputChannelTemp)-itx*pow(10,strlen(OutputChannelTemp)-2)-ilx*
                    pow(10,strlen(OutputChannelTemp)-4))/pow(10,strlen(OutputChannelTemp)-6));
                  is = int(atoi(OutputChannelTemp)-itx*pow(10,strlen(OutputChannelTemp)-2)-ilx*
                    pow(10,strlen(OutputChannelTemp)-4)-idx*pow(10,strlen(OutputChannelTemp)-6));
                  if(!strcmp(Det[itx][ilx][idx].Matt,"si")&&is==0) 
                  {
                    if(Tb>0.1) Tb = EiEo(EjMat[ip]->ej_si,Tb,range*Det[itx][ilx][idx].DeadZoneF/Det[itx][ilx][idx].Thick);
                    if(Tb>0.1) Ta = EiEo(EjMat[ip]->ej_si,Tb,range*(Det[itx][ilx][idx].Thick-Det[itx][ilx][idx].DeadZoneF-
                      Det[itx][ilx][idx].DeadZoneB)/Det[itx][ilx][idx].Thick);
                    if(al[itx][ilx][ip][0]==-1000.) al[itx][ilx][ip][0] = Tb-Ta;
                    else al[itx][ilx][ip][0] += Tb-Ta;
                    Tb = Ta;
                    if(Tb>0.1) Tb = EiEo(EjMat[ip]->ej_si,Tb,range*Det[itx][ilx][idx].DeadZoneB/Det[itx][ilx][idx].Thick);
                  }
                  if(!strcmp(Det[itx][ilx][idx].Matt,"csi")&&is==0) 
                  {
                    if(Tb>0.1) Ta = EiEo(EjMat[ip]->ej_csi,Tb,range);
                    if(al[itx][ilx][ip][0]==-1000.) al[itx][ilx][ip][0] = Tb-Ta;
                    else al[itx][ilx][ip][0] += Tb-Ta;
                  }
                }
                if(!strcmp(OutputChannelTemp,"gas")&&Tb>0.1) Tb = EiEo(EjMat[ip]->ej_target,Tb,range*header->UpMat.TarPress*TempNorm/header->UpMat.TarTemp);
                if(!strcmp(OutputChannelTemp,"foilex")&&Tb>0.1) Tb = EiEo(EjMat[ip]->ej_TARwin,Tb,range); 
                if(!strcmp(OutputChannelTemp,"foilen")&&Tb>0.1) Tb = EiEo(EjMat[ip]->ej_TARwin,Tb,range); 
                if(!strcmp(OutputChannelTemp,"TargetCell")&&Tb>0.1) Tb = EiEo(EjMat[ip]->ej_TARwin,Tb,range); 
                if(!strcmp(OutputChannelTemp,"HscrFoil")&&Tb>0.1) Tb = EiEo(EjMat[ip]->ej_TARwin,Tb,range); 
              }
              trajectory = geom->Step();
            } /* while(!geom->IsOutside) */
          } /* for(ip=0;ip<NofObsPart;ip++) */
        } /* if(mp[it]==1&&RawD.mbeam&&RawD.mtrack) */
      } /* for(it=0;it<Ntelescopes;it++) */
  
  //cout << "Inclusive events calculations" << endl;
  for(int it=0;it<Ntelescopes;it++)
  {
    if(mp[it]==1&&header->mbeam&&header->mtrack)
    {
      for(int ip=0;ip<header->NofDetPart;ip++)
      {
        for(int imu=0;imu<mp[it];imu++)
        {
          Tb = 0.;Tout=0.;
          Tb = sqrt(pow(projectile->Part.Px()-ejectile[it][ip][imu].Part.Px(),2)+
            pow(projectile->Part.Py()-ejectile[it][ip][imu].Part.Py(),2)+
            pow(projectile->Part.Pz()-ejectile[it][ip][imu].Part.Pz(),2)+
            pow(mis[it][ip][imu].Mass,2))-mis[it][ip][imu].Mass;
          Tout = projectile->Part.E()+target->Mass-ejectile[it][ip][imu].Part.E()-Tb-mis[it][ip][imu].Mass;
          Tb = sqrt(pow(projectile->Part.Px()-ejectile[it][ip][imu].Part.Px(),2)+
            pow(projectile->Part.Py()-ejectile[it][ip][imu].Part.Py(),2)+
            pow(projectile->Part.Pz()-ejectile[it][ip][imu].Part.Pz(),2)+
            pow(mis[it][ip][imu].Mass+Tout,2))-mis[it][ip][imu].Mass-Tout;
          Tout = projectile->Part.E()+target->Mass-ejectile[it][ip][imu].Part.E()-Tb-mis[it][ip][imu].Mass;
          mis[it][ip][imu].Excitation = Tout;
          mis[it][ip][imu].Part.SetPxPyPzE(projectile->Part.Px()-ejectile[it][ip][imu].Part.Px(),
          projectile->Part.Py()-ejectile[it][ip][imu].Part.Py(),projectile->Part.Pz()-ejectile[it][ip][imu].Part.Pz(),
          Tb+Tout+mis[it][ip][imu].Mass);
          
        }/* for(imu=0;imu<Mu[it][0];imu++) */
      } /* for(ip==0;ip<header->NofDetPart;ip++) */
    } /* if(mp[it]==1&&RawD.mbeam&&RawD.mtrack) */
  } /* for(it==0;it<Ntelescopes) */
  InLabFrame();
  InReactionCM();
  InProjectileFrame();
}
//----------------------------------------------------------------------------
void ERHe8Analyzer::InLabFrame(){
  //cout << "In Laboratory frame" << endl;

  tel->dep11 = DepoX[0][0][0][0];tel->dep12 = DepoX[0][1][0][0];     
  tel->dep21 = DepoX[1][0][0][0];tel->dep22 = DepoX[1][1][0][0];tel->dep23 = DepoX[1][2][0][0];
  tel->dep24 = DepoX[1][3][0][0];tel->dep25 = DepoX[1][4][0][0];tel->dep26 = DepoX[1][5][0][0];
  
  tel->t11 = ejectile[0][0][0].Part.E()-ejectile[0][0][0].Mass;
  tel->t22 = ejectile[1][1][0].Part.E()-ejectile[1][1][0].Mass;
  
  tel->th11 = ejectile[0][0][0].Part.Theta()/rad;tel->phi11 = ejectile[0][0][0].Part.Phi()/rad;
  tel->th22 = ejectile[1][1][0].Part.Theta()/rad;tel->phi22 = ejectile[1][1][0].Part.Phi()/rad;

  tel->x11 = cx[0][0];
  tel->y11 = cy[0][0];
  tel->x21 = cx[1][0];
  tel->y21 = cy[1][0];
  tel->x22 = cx[1][1];
  tel->y22 = cy[1][1];
  tel->x23 = cx[1][2];
  tel->y23 = cy[1][2];
  tel->x24 = cx[1][3];
  tel->y24 = cy[1][3];
  tel->x25 = cx[1][4];
  tel->y25 = cy[1][4];
  tel->x26 = cx[1][5];
  tel->y26 = cy[1][5];
  
  tel->al212 = al[1][0][0][0];
  tel->al222 = al[1][1][0][0];
  tel->al232 = al[1][2][0][0];
  tel->al242 = al[1][3][0][0];
  tel->al252 = al[1][4][0][0];
  tel->al262 = al[1][5][0][0];

  inclu->tmis11 = mis[0][0][0].Part.E()-mis[0][0][0].Mass-mis[0][0][0].Excitation;
  inclu->tmis22 = mis[1][1][0].Part.E()-mis[1][1][0].Mass-mis[1][1][0].Excitation;

  inclu->thmis11 = mis[0][0][0].Part.Theta()/rad;
  inclu->thmis22 = mis[1][1][0].Part.Theta()/rad;
  inclu->exmis11 = mis[0][0][0].Excitation;
  inclu->exmis22 = mis[1][1][0].Excitation;
}
//----------------------------------------------------------------------------
void ERHe8Analyzer::InReactionCM(){
  //cout << "In Reaction CM" << endl;
  for(int it=0;it<Ntelescopes;it++)
    {
      if(mp[it]==1&&header->mbeam&&header->mtrack)
      {
        for(int ip=0;ip<header->NofDetPart;ip++)
        {
          for(int imu=0;imu<=mp[it];imu++)
          {
            mis[it][ip][imu].Part.Boost(-1*CM0->Part.BoostVector());
            ejectile[it][ip][imu].Part.Boost(-1*CM0->Part.BoostVector());
          }
        }
      }
    }
  
  tel->t11cm0 = ejectile[0][0][0].Part.E()-ejectile[0][0][0].Mass;
  tel->t22cm0 = ejectile[1][1][0].Part.E()-ejectile[1][1][0].Mass;

  tel->th11cm0 = ejectile[0][0][0].Part.Theta()/rad;
  tel->th22cm0 = ejectile[1][1][0].Part.Theta()/rad;
 
  inclu->tmis11cm0 = mis[0][0][0].Part.E()-mis[0][0][0].Mass-mis[0][0][0].Excitation;
  inclu->tmis22cm0 = mis[1][1][0].Part.E()-mis[1][1][0].Mass-mis[1][1][0].Excitation;
  
  inclu->thmis11cm0 = mis[0][0][0].Part.Theta()/rad;
  inclu->thmis22cm0 = mis[1][1][0].Part.Theta()/rad;
}
//----------------------------------------------------------------------------
void ERHe8Analyzer::InProjectileFrame(){
  //cout << "In Projectile CM" << endl;

  for(int it=0;it<Ntelescopes;it++)
  {
    if(mp[it]==1&&header->mbeam&&header->mtrack)
    {
      for(int ip=0;ip<header->NofDetPart;ip++)
      {
        for(int imu=0;imu<=mp[it];imu++)
        {
          mis[it][ip][imu].Part.Boost(CM0->Part.BoostVector());
          ejectile[it][ip][imu].Part.Boost(CM0->Part.BoostVector());
          mis[it][ip][imu].Part.Boost(-projectile->Part.BoostVector());
          ejectile[it][ip][imu].Part.Boost(-projectile->Part.BoostVector());
        }
      }
    }
  }
  tel->t11cmp = ejectile[0][0][0].Part.E()-ejectile[0][0][0].Mass;
  tel->t22cmp = ejectile[1][1][0].Part.E()-ejectile[1][1][0].Mass;

  tel->pz11cmp = ejectile[0][0][0].Part.Pz();
  tel->pz22cmp = ejectile[1][1][0].Part.Pz();

  tel->th11cmp = ejectile[0][0][0].Part.Theta()/rad;
  tel->th22cmp = ejectile[1][1][0].Part.Theta()/rad;

  inclu->tmis11cmp = mis[0][0][0].Part.E()-mis[0][0][0].Mass-mis[0][0][0].Excitation;
  inclu->tmis22cmp = mis[1][1][0].Part.E()-mis[1][1][0].Mass-mis[1][1][0].Excitation;
  
  inclu->thmis11cmp = mis[0][0][0].Part.Theta()/rad;
  inclu->thmis22cmp = mis[1][1][0].Part.Theta()/rad;

  for(int it=0;it<Ntelescopes;it++)
  {
    if(mp[it]==1&&header->mbeam&&header->mtrack)
    {
      for(int ip=0;ip<header->NofDetPart;ip++)
      {
        for(int imu=0;imu<=mp[it];imu++)
        {
          ejectile[it][ip][imu].Part.Boost(projectile->Part.BoostVector());
        }
      }
    }
  }
}
//----------------------------------------------------------------------------
void ERHe8Analyzer::Reset(){
  tel->Reset();
  inclu->Reset();
  for(int it=0;it<Ntelescopes;it++)
      {
        mp[it] = 0;
        for(int itx=0;itx<Ntelescopes;itx++)
        {
          if(it!=itx)
          {
            for(int ip=0;ip<header->NofDetPart;ip++)
            {
              for(int ipx=0;ipx<header->NofDetPart;ipx++)
              {
                if(ip!=ipx)
                {
                  participants[it][ip][itx][ipx].Excitation = -1000.;
                  spectator[it][ip][itx][ipx].Excitation = -1000.;
                }
              }
            }
          }
        }
        
        for(int il=0;il<layer[it];il++)
        {
          TCheck[it][il] = 0;
          for(int ip=0;ip<header->NofDetPart;ip++)
          {
            for(int imu=0;imu<NDivXYMax;imu++)
            {
              al[it][il][ip][imu] = -1000.;
            }
          }
          for(int id=0;id<NDetMax;id++)
          {
            mpd[it][il][id] = 0;
            MuX[it][il][id] = -1;
            MuY[it][il][id] = -1;
            MuXT[it][il][id] = -1;
            MuYT[it][il][id] = -1;
            xbdet0[it][il][id] = -100.;
            ybdet0[it][il][id] = -100.;
            for(int imu=0;imu<NDivXYMax;imu++)
            {
              NhitX[it][il][id][imu] = 0;
              NhitY[it][il][id][imu] = 0;
              NhitXT[it][il][id][imu] = 0;
              NhitYT[it][il][id][imu] = 0;
              DepoX[it][il][id][imu] = 0.;
              DepoY[it][il][id][imu] = 0.;
              cx[it][il] = 0.;
              cy[it][il] = 0.;
              cz[it][il] = 0.;
              deposit[it][il][id][imu] = 0.;
            }
            for(int ip=0;ip<header->NofDetPart;ip++)
            {
              HitX[it][il][id][ip] = -1;
              HitXT[it][il][id][ip] = -1;
              HitY[it][il][id][ip] = -1;
              HitYT[it][il][id][ip] = -1;
            }
          }
        }
      }
}
// ----------------------------------------------------------------------------
void ERHe8Analyzer::Finish()
{   
}
// ----------------------------------------------------------------------------
void ERHe8Analyzer::ReadTelescopeParameters(){

  layer = new int[NTelMax];
  NDet = new int*[NTelMax];
  for (int i = 0; i<NTelMax;i++)
    NDet[i] = new int[NLayMax];

  int it,il,id,is,imu,ip,count;
  int itx,ilx,idx,ipx;

  char DUpMatmyC[256];
  char Zeros[32];

  double read_angle1,read_angle2;
  double dist,dzf,dzb,thi,szx,szy,ofx,ofy,a1,a2,a3;
  double ResolutionSi;
  double ResolutionCsI;
  int nx,ny;
  char st[12];
  char mat[12];
  char sha[12];

  TString filePath = gSystem->Getenv("VMCWORKDIR") + TString("/input/detsys.prm");
  FILE *F1 = fopen(filePath.Data(),"r");
  if(F1==NULL) printf("Main: File detsys.prm was not found\n");
  else
  {
    it=0;il=0;
    while (fgets(DUpMatmyC,256,F1)) 
    {
      if(strstr(DUpMatmyC,"Telescope")) {layer[it]=-1;it++;}
      if(strstr(DUpMatmyC,"Layer")) {layer[it-1]++;NDet[it-1][layer[it-1]]=-1;}
      if(strstr(DUpMatmyC,"Detector")) NDet[it-1][layer[it-1]]--;
      if(strstr(DUpMatmyC,"Resolution")) break;
      if(layer[it-1]>-1) NDet[it-1][layer[it-1]]++;
    }
    Ntelescopes = it;
  }
  NLayMax=0;
  for(it=0;it<Ntelescopes;it++)
  {
    layer[it]++;
    if(layer[it]>=NLayMax) NLayMax = layer[it];
    printf("Main: Telescope %i - %i layers:\n",it,layer[it]);
    for(il=0;il<layer[it];il++)
    {
      if(NDet[it][il]>=NDetMax) NDetMax=NDet[it][il];
    }
  }
  printf("Main: Ntelescopes=%i, NLayMax=%i, NDetMax=%i\n",Ntelescopes,NLayMax,NDetMax);
  fclose(F1);

  Det = new Telescope**[Ntelescopes];
  for (int i = 0; i < Ntelescopes; i++){
    Det[i] = new Telescope*[NLayMax];
    for(int j = 0; j<NLayMax; j++)
      Det[i][j] = new Telescope[NDetMax];
  }
  F1 = fopen(filePath.Data(),"r");
  for(it=0;it<Ntelescopes;it++)
  {
    fscanf(F1,"%s %lf %lf\n",Zeros,&read_angle1,&read_angle2);
    for(il=0;il<layer[it];il++)
    {
      fscanf(F1,"%s\n",Zeros);
      for(id=0;id<NDet[it][il];id++)
      {
        fscanf(F1,"%s %lf %lf %lf %lf %s %s\n",Zeros,&dist,&a1,&a2,&a3,mat,sha);
        fscanf(F1,"%s %lf %lf %lf %lf %lf %lf %lf %i %i\n",st,&dzf,&dzb,&thi,&szx,&szy,&ofx,&ofy,&nx,&ny);
        Det[it][il][id].Theta = read_angle1*rad;
        Det[it][il][id].Psi = read_angle2*rad;
        strcpy(Det[it][il][id].StripFB,st);
        Det[it][il][id].Dist = dist;
        Det[it][il][id].DeadZoneF = dzf;
        Det[it][il][id].DeadZoneB = dzb;
        Det[it][il][id].Thick = thi;
        Det[it][il][id].SizeX = szx;
        Det[it][il][id].SizeY = szy;
        Det[it][il][id].Rin = szx;
        Det[it][il][id].Rout = szy;
        Det[it][il][id].OffsetX = ofx;
        Det[it][il][id].OffsetY = ofy;
        Det[it][il][id].DeltaAlpha = a1*rad;
        Det[it][il][id].DeltaBeta = a2*rad;
        Det[it][il][id].DeltaGamma = a3*rad;
        Det[it][il][id].NstripX = nx;
        Det[it][il][id].NstripY = ny;
        Det[it][il][id].Nring = nx;
        Det[it][il][id].Nsector = ny;
        strcpy(Det[it][il][id].Matt,mat);
        strcpy(Det[it][il][id].Shape,sha);
        Det[it][il][id].DetOwnAxisX.SetXYZ(0.,-1.,0.);
        Det[it][il][id].DetOwnAxisY.SetXYZ(1.,0.,0.);
        Det[it][il][id].DetOwnAxisZ.SetXYZ(0.,0.,1.);
        Det[it][il][id].DetLabVect.SetXYZ(Det[it][il][id].OffsetY,-Det[it][il][id].OffsetX,Det[it][il][id].Dist);

        if(abs(Det[it][il][id].NstripX)+abs(Det[it][il][id].NstripY)>NDivMax)
          NDivMax=abs(Det[it][il][id].NstripX)+abs(Det[it][il][id].NstripY);
        if(abs(Det[it][il][id].NstripX)>NDivXYMax)
          NDivXYMax=abs(Det[it][il][id].NstripX);
        if(abs(Det[it][il][id].NstripY)>NDivXYMax)
          NDivXYMax=abs(Det[it][il][id].NstripY);
      }
    }
  }
  fscanf(F1,"%s %lf %lf\n",Zeros,&ResolutionSi,&ResolutionCsI);
  fclose(F1);



  printf("******************* CHECK SETUP *************************\n");
  for(it=0;it<Ntelescopes;it++)
  {
    printf("TELESCOPE%i\n",it);
    printf("|----------------------------------------------------------|\n");
    for(il=0;il<layer[it];il++)
    {
      printf("LAYER%i\n",il);
      for(id=0;id<NDet[it][il];id++)
      {
        printf("| D%i: D=%lf, Theta=%lf, Psi=%lf\n",id,Det[it][il][id].Dist,Det[it][il][id].Theta/rad,Det[it][il][id].Psi/rad);
        printf("| D%i: A=%lf, B=%lf, G=%lf\n",id,Det[it][il][id].DeltaAlpha/rad,Det[it][il][id].DeltaBeta/rad,Det[it][il][id].DeltaGamma/rad);
        printf("| D%i: Material-%s, Shape-%s, Strip-%s\n",id,Det[it][il][id].Matt,Det[it][il][id].Shape,Det[it][il][id].StripFB);
        printf("| D%i: Thick=%lf, DeadZoneF=%lf, DeadZoneB=%lf\n",id,Det[it][il][id].Thick,Det[it][il][id].DeadZoneF,Det[it][il][id].DeadZoneB);
        printf("| D%i: SizeX(Rin)=%lf, SizeY(Rout)=%lf\n",id,Det[it][il][id].SizeX,Det[it][il][id].SizeY);
        printf("| D%i: OffsetX=%lf, OffsetY=%lf\n",id,Det[it][il][id].OffsetX,Det[it][il][id].OffsetY);
        printf("| D%i: NstripX(Nring)=%i, NstripY(Nsector)=%i\n",id,Det[it][il][id].NstripX,Det[it][il][id].NstripY);
        printf("|\n");
      }
    }
    printf("|----------------------------------------------------------|\n");
  }
  printf("Resolution: Si=%lf MeV, CsI=%lf percent\n",ResolutionSi,ResolutionCsI);
  printf("************************************************************\n");
}
// ----------------------------------------------------------------------------
void ERHe8Analyzer::CreateTelescopeGeometry(){
  cout << "CreateTelescopeGeometry started" << endl;
  double ThickStrip=0.0001;
  geom=new TGeoManager("Reaction Chamber","geom"); 
  TGeoMaterial *matVacuum=new TGeoMaterial("Vacuum",0,0,0);
  TGeoMaterial *matSi=new TGeoMaterial("Si",24.,12,2.4);
  TGeoMedium *Vacuum=new TGeoMedium("Vacuum",1,matVacuum);
  TGeoMedium *Si=new TGeoMedium("Si",2,matSi);
  TGeoVolume *top=geom->MakeSphere("Top",Vacuum,0.,100.);
  geom->SetTopVolume(top);

  TGeoRotation *rot0 = new TGeoRotation("rot0",0.,90.,0.);
  rot0->RegisterYourself();

  TGeoVolume *pDet[Ntelescopes][NLayMax][NDetMax][NDivMax+1];
  
  char DetName[128];
  char N1[128];
  char N0[128];

  double StripSizeX,StripSizeY,RingInit,RingFin,PhiInit,PhiFin,move;
  
  for(int it=0;it<Ntelescopes;it++)
  {
    for(int il=0;il<layer[it];il++)
    {
      for(int id=0;id<NDet[it][il];id++)
      {
        TGeoRotation *labrot=new TGeoRotation();
        TGeoTranslation *labshift=new TGeoTranslation();

        TGeoRotation *detrot_a1=new TGeoRotation();
        TGeoRotation *detrot_a2=new TGeoRotation();
        TGeoRotation *detrot_a3=new TGeoRotation();

        TGeoCombiTrans *pos0=new TGeoCombiTrans();
        TGeoCombiTrans *pos1=new TGeoCombiTrans();
        TGeoCombiTrans *pos2=new TGeoCombiTrans();
        TGeoCombiTrans *pos3=new TGeoCombiTrans();
        TGeoCombiTrans *pos4=new TGeoCombiTrans();

        labrot->SetAngles(90.,Det[it][il][id].Theta/rad-90.,Det[it][il][id].Psi/rad);
        labshift->SetTranslation(Det[it][il][id].OffsetY,-Det[it][il][id].Dist-Det[it][il][id].Thick/2.,-Det[it][il][id].OffsetX);
        detrot_a1->SetAngles(Det[it][il][id].DeltaAlpha/rad,0.,0.);
        detrot_a2->SetAngles(0.,Det[it][il][id].DeltaBeta/rad,0.);
        detrot_a3->SetAngles(Det[it][il][id].DeltaGamma/rad,0.,0.);

        *pos0 = *rot0*(*detrot_a3);
        *pos1 = *detrot_a1*(*pos0);
        *pos2 = *detrot_a2*(*pos1);
        *pos3 = *labrot*(*labshift);
        *pos4 = *pos3*(*pos2);
        pos4->RegisterYourself();

        Det[it][il][id].DetOwnAxisX.RotateZ(Det[it][il][id].DeltaGamma);
        Det[it][il][id].DetOwnAxisX.RotateX(90.*rad);
        Det[it][il][id].DetOwnAxisX.RotateZ(Det[it][il][id].DeltaAlpha);
        Det[it][il][id].DetOwnAxisX.RotateX(Det[it][il][id].DeltaBeta);
        Det[it][il][id].DetOwnAxisX.RotateZ(90.*rad);
        Det[it][il][id].DetOwnAxisX.RotateZ(Det[it][il][id].Psi);
        Det[it][il][id].DetOwnAxisX.RotateY(Det[it][il][id].Theta-90.*rad);

        Det[it][il][id].DetOwnAxisY.RotateZ(Det[it][il][id].DeltaGamma);
        Det[it][il][id].DetOwnAxisY.RotateX(90.*rad);
        Det[it][il][id].DetOwnAxisY.RotateZ(Det[it][il][id].DeltaAlpha);
        Det[it][il][id].DetOwnAxisY.RotateX(Det[it][il][id].DeltaBeta);
        Det[it][il][id].DetOwnAxisY.RotateZ(90.*rad);
        Det[it][il][id].DetOwnAxisY.RotateZ(Det[it][il][id].Psi);
        Det[it][il][id].DetOwnAxisY.RotateY(Det[it][il][id].Theta-90.*rad);

        Det[it][il][id].DetOwnAxisZ.RotateZ(Det[it][il][id].DeltaGamma);
        Det[it][il][id].DetOwnAxisZ.RotateX(90.*rad);
        Det[it][il][id].DetOwnAxisZ.RotateZ(Det[it][il][id].DeltaAlpha);
        Det[it][il][id].DetOwnAxisZ.RotateX(Det[it][il][id].DeltaBeta);
        Det[it][il][id].DetOwnAxisZ.RotateZ(90.*rad);
        Det[it][il][id].DetOwnAxisZ.RotateZ(Det[it][il][id].Psi);
        Det[it][il][id].DetOwnAxisZ.RotateY(Det[it][il][id].Theta-90.*rad);

        Det[it][il][id].DetLabVect.RotateX(90.*rad);
        Det[it][il][id].DetLabVect.RotateZ(90.*rad);
        Det[it][il][id].DetLabVect.RotateZ(Det[it][il][id].Psi);
        Det[it][il][id].DetLabVect.RotateY(Det[it][il][id].Theta-90.*rad);

        strcpy(DetName,"pDet");
        if(it<10) strcat(DetName,"0");sprintf(N1,"%d",it);strcat(DetName,N1);
        if(il<10) strcat(DetName,"0");sprintf(N1,"%d",il);strcat(DetName,N1);
        if(id<10) strcat(DetName,"0");sprintf(N1,"%d",id);strcat(DetName,N1);
        strcpy(N0,DetName);
        strcat(DetName,"00");
        
        if(!strcmp(Det[it][il][id].Shape,"square"))
        {
          pDet[it][il][id][0]=geom->MakeBox(DetName,Si,Det[it][il][id].SizeY/2.,Det[it][il][id].SizeX/2.,Det[it][il][id].Thick/2.);
          top->AddNode(pDet[it][il][id][0],1,pos4);

          for(int is=1;is<=fabs(Det[it][il][id].NstripX);is++)
          {
            TGeoCombiTrans *pos5=new TGeoCombiTrans();
            TGeoTranslation *strmove=new TGeoTranslation();
            strcpy(DetName,N0);if(is<10) strcat(DetName,"0");
            sprintf(N1,"%d",is);strcat(DetName,N1);

            StripSizeX=fabs(Det[it][il][id].SizeX/2./Det[it][il][id].NstripX);
            StripSizeY=Det[it][il][id].SizeY/2.;
            move=(-1.)*Det[it][il][id].SizeX*(double(is)-(fabs(Det[it][il][id].NstripX)+1)/2.)/Det[it][il][id].NstripX;

            if(!strcmp(Det[it][il][id].StripFB,"XY")) strmove->SetTranslation(0.,move,-1.*(Det[it][il][id].Thick+ThickStrip)/2.);
            if(!strcmp(Det[it][il][id].StripFB,"YX")) strmove->SetTranslation(0.,move,(Det[it][il][id].Thick+ThickStrip)/2.);

            *pos5 = *pos4*(*strmove);
            pDet[it][il][id][is]=geom->MakeBox(DetName,Si,StripSizeY,StripSizeX,ThickStrip/2.);
            top->AddNode(pDet[it][il][id][is],1,pos5);
          }
          for(int is=abs(Det[it][il][id].NstripX)+1;is<=abs(Det[it][il][id].NstripX)+abs(Det[it][il][id].NstripY);is++)
          {
            TGeoCombiTrans *pos5=new TGeoCombiTrans();
            TGeoTranslation *strmove1=new TGeoTranslation();
            strcpy(DetName,N0);if(is<10) strcat(DetName,"0");
            sprintf(N1,"%d",is);strcat(DetName,N1);

            StripSizeX=Det[it][il][id].SizeX/2.;
            StripSizeY=fabs(Det[it][il][id].SizeY/2./Det[it][il][id].NstripY);
            move=-1*Det[it][il][id].SizeY*(double(is-fabs(Det[it][il][id].NstripX))-(fabs(Det[it][il][id].NstripY)+1)/2.)/Det[it][il][id].NstripY;

            if(!strcmp(Det[it][il][id].StripFB,"XY")) strmove1->SetTranslation((-1.)*move,0.,(Det[it][il][id].Thick+ThickStrip)/2.);
            if(!strcmp(Det[it][il][id].StripFB,"YX")) strmove1->SetTranslation((-1.)*move,0.,-(Det[it][il][id].Thick+ThickStrip)/2.);

            *pos5 = *pos4*(*strmove1);
            pDet[it][il][id][is]=geom->MakeBox(DetName,Si,StripSizeY,StripSizeX,ThickStrip/2.);
            top->AddNode(pDet[it][il][id][is],1,pos5);
          }
        }
        if(!strcmp(Det[it][il][id].Shape,"annular"))
        {
          pDet[it][il][id][0]=geom->MakeTube(DetName,Si,Det[it][il][id].Rin,Det[it][il][id].Rout,Det[it][il][id].Thick/2.);
          top->AddNode(pDet[it][il][id][0],1,pos4);

          for(int is=1;is<=abs(Det[it][il][id].Nring);is++)
          {
            TGeoCombiTrans *pos5=new TGeoCombiTrans();
            TGeoTranslation *strmove=new TGeoTranslation();
            strcpy(DetName,N0);if(is<10) strcat(DetName,"0");
            sprintf(N1,"%d",is);strcat(DetName,N1);

            if(Det[it][il][id].Nring>0) 
            RingInit=Det[it][il][id].Rin+(Det[it][il][id].Rout-Det[it][il][id].Rin)*double(is-1)/Det[it][il][id].Nring;
            else
            RingInit=Det[it][il][id].Rin+(Det[it][il][id].Rout-Det[it][il][id].Rin)*double(is-abs(Det[it][il][id].Nring))/Det[it][il][id].Nring;

            RingFin=RingInit+(Det[it][il][id].Rout-Det[it][il][id].Rin)/abs(Det[it][il][id].Nring);

            if(!strcmp(Det[it][il][id].StripFB,"RS")) 
            strmove->SetTranslation(0.,0.,-(Det[it][il][id].Thick+ThickStrip)/2.);
            if(!strcmp(Det[it][il][id].StripFB,"SR")) strmove->SetTranslation(0.,0.,(Det[it][il][id].Thick+ThickStrip)/2.);

            *pos5 = *pos4*(*strmove);
            pDet[it][il][id][is]=geom->MakeTube(DetName,Si,RingInit,RingFin,ThickStrip/2.);
            top->AddNode(pDet[it][il][id][is],1,pos5);
          }
          for(int is=abs(Det[it][il][id].Nring)+1;is<=abs(Det[it][il][id].Nring)+abs(Det[it][il][id].Nsector);is++)
          {
            TGeoCombiTrans *pos5=new TGeoCombiTrans();
            TGeoTranslation *strmove1=new TGeoTranslation();
            strcpy(DetName,N0);if(is<10) strcat(DetName,"0");
            sprintf(N1,"%d",is);strcat(DetName,N1);

            if(Det[it][il][id].Nsector>0) 
            PhiInit=360.*double(is-abs(Det[it][il][id].Nring)-1)/Det[it][il][id].Nsector-90.;
            else if(Det[it][il][id].Nsector<0)
            PhiInit=360.*double(is-abs(Det[it][il][id].Nring)-abs(Det[it][il][id].Nsector))/Det[it][il][id].Nsector-90.;

            PhiFin=PhiInit+360./abs(Det[it][il][id].Nsector);
            if(!strcmp(Det[it][il][id].StripFB,"RS")) strmove1->SetTranslation(0.,0.,(Det[it][il][id].Thick+ThickStrip)/2.);
            if(!strcmp(Det[it][il][id].StripFB,"SR")) strmove1->SetTranslation(0.,0.,-(Det[it][il][id].Thick+ThickStrip)/2.);

            *pos5 = *pos4*(*strmove1);
            pDet[it][il][id][is]=geom->MakeTubs(DetName,Si,Det[it][il][id].Rin,Det[it][il][id].Rout,ThickStrip/2.,PhiInit,PhiFin);
            top->AddNode(pDet[it][il][id][is],1,pos5);
          }
        }
        if(!strcmp(Det[it][il][id].Shape,"sector"))
        {
          PhiInit=-90.;
          PhiFin=PhiInit+360./abs(Det[it][il][id].Nsector);
          pDet[it][il][id][0]=geom->MakeTubs(DetName,Si,Det[it][il][id].Rin,Det[it][il][id].Rout,Det[it][il][id].Thick/2.,PhiInit,PhiFin);
          top->AddNode(pDet[it][il][id][0],1,pos4);

          strcpy(DetName,N0);
          strcat(DetName,"01");
          TGeoTranslation *strmove=new TGeoTranslation();
          strmove->SetTranslation(0.,0.,(Det[it][il][id].Thick+ThickStrip)/2.);
            
          TGeoCombiTrans *pos5=new TGeoCombiTrans();
          *pos5 = *pos4*(*strmove);
          pDet[it][il][id][1]=geom->MakeTubs(DetName,Si,Det[it][il][id].Rin,Det[it][il][id].Rout,ThickStrip/2.,PhiInit,PhiFin);
          top->AddNode(pDet[it][il][id][1],1,pos5);

          strcpy(DetName,N0);
          strcat(DetName,"02");
          TGeoTranslation *strmove1=new TGeoTranslation();
          strmove1->SetTranslation(0.,0.,-(Det[it][il][id].Thick+ThickStrip)/2.);

          TGeoCombiTrans *pos6=new TGeoCombiTrans();
          *pos6 = *pos4*(*strmove1);
          pDet[it][il][id][2]=geom->MakeTubs(DetName,Si,Det[it][il][id].Rin,Det[it][il][id].Rout,ThickStrip/2.,PhiInit,PhiFin);
          top->AddNode(pDet[it][il][id][2],1,pos6);
        }
        delete detrot_a1;
        delete detrot_a2;
        delete detrot_a3;
        delete labshift;
        delete labrot;
      }
    }
  } /* (it=0;it<Ntelescopes;it++) */
  cout << "CreateTelescopeGeometry Finished" << endl;
  cout << "Target geometry creating started" << endl;
  if(!strcmp(header->UpMat.TarShape,"SemiCylindrical"))
  {
    header->UpMat.TarThick = header->UpMat.TarRadius;
    TGeoVolume *ttargetgas=geom->MakeTubs("ttargetgas",Si,0,header->UpMat.TarRadius,header->UpMat.TarHeight/2.,0.,180.);

    TGeoVolume *TargetUp=geom->MakeTubs("TargetUp",Si,0,header->UpMat.TarRadius,header->UpMat.TarWallThick/2.,0.,180.);
    TGeoTranslation *TargetUpShift=new TGeoTranslation("TargetUpShift",0.,-header->UpMat.TarRadius/2.,-(header->UpMat.TarHeight+header->UpMat.TarWallThick)/2.);
    TargetUpShift->RegisterYourself();

    TGeoVolume *TargetDown=geom->MakeTubs("TargetDown",Si,0,header->UpMat.TarRadius,header->UpMat.TarWallThick/2,0.,180.);
    TGeoTranslation *TargetDownShift=new TGeoTranslation("TargetDownShift",0.,-header->UpMat.TarRadius/2.,(header->UpMat.TarHeight+header->UpMat.TarWallThick)/2.);
    TargetDownShift->RegisterYourself();

    TGeoVolume *TargetBack=geom->MakeBox("TargetBack",Si,header->UpMat.TarRadius,header->UpMat.TarWallThick+(header->UpMat.TarHeight/2.),header->UpMat.TarWallThick/2.);
    TGeoTranslation *TargetBackShift=new TGeoTranslation("TargetBackShift",0.,0.,-header->UpMat.TarWallThick/2.-header->UpMat.TarRadius/2.);
    TargetBackShift->RegisterYourself();

    TGeoVolume *TargetEntrHole=geom->MakeTube("TargetEntrHole",Si,0.,header->UpMat.TarEntrHoleRad,(header->UpMat.TarWallThick+header->UpMat.FoilThick)/2.);
    TGeoCompositeShape *target=new TGeoCompositeShape("target","(TargetUp:TargetUpShift+TargetDown:TargetDownShift):rot0+(TargetBack:TargetBackShift-TargetEntrHole:TargetBackShift)");
    TGeoVolume *ttarcell=new TGeoVolume("ttarcell",target,Si);

    TGeoVolume *tExitFoil=geom->MakeTubs("tExitFoil",Si,header->UpMat.TarRadius,header->UpMat.TarRadius+header->UpMat.FoilThick,header->UpMat.TarHeight/2.,0.,180.);
    TGeoTranslation *ExitFoilShift=new TGeoTranslation("ExitFoilShift",0.,0.,-header->UpMat.TarRadius/2.);
    ExitFoilShift->RegisterYourself();

    TGeoVolume *tEntrFoil=geom->MakeTube("tEntrFoil",Si,0.,header->UpMat.TarEntrHoleRad,header->UpMat.FoilThick/2.);
    TGeoTranslation *EntrFoilShift=new TGeoTranslation(0,0.,-header->UpMat.FoilThick/2.-header->UpMat.TarRadius/2.);

    TGeoCombiTrans *posexf=new TGeoCombiTrans();
    posexf->RegisterYourself();
    *posexf = *ExitFoilShift*(*rot0);

    top->AddNode(tEntrFoil,1,EntrFoilShift);
    top->AddNode(tExitFoil,1,posexf);
    top->AddNode(ttarcell,1);
    top->AddNode(ttargetgas,1,posexf);
  }
  else if(!strcmp(header->UpMat.TarShape,"Cylindrical"))
  {
    TGeoRotation *rottar=new TGeoRotation();
    rottar->SetAngles(90.,header->UpMat.TarAngle,0.);

    header->UpMat.TarThick = header->UpMat.TarHeight;

  // making the cylindrical part of the gaseous body "ttargetgas", that afterward must be shifted backward by TargetShift

    TGeoVolume *ttargetgas=geom->MakeTube("ttargetgas",Si,0,header->UpMat.TarEntrHoleRad,header->UpMat.TarThick/2.);
    TGeoTranslation *TargetShift=new TGeoTranslation("TargetShift",0.,0.,-(header->UpMat.MeniskSize+header->UpMat.TarThick)/2.);
    TargetShift->RegisterYourself();

  // making the menisk entrance part of the gaseous body "tGasMenEntr", that afterward must be shifted backward by MeniskShift

    TGeoVolume *tGasMenEntr=geom->MakeParaboloid("tGasMenEntr",Si,0.,header->UpMat.TarEntrHoleRad,header->UpMat.MeniskSize/2.);
    TGeoTranslation *MeniskShift=new TGeoTranslation("MeniskShift",0.,0.,-(header->UpMat.MeniskSize+header->UpMat.TarThick));
    MeniskShift->RegisterYourself();

  // making the menisk exit part of the gaseous body "tGasMenExit", that afterward must be revolved by 180 deg by rotExitMen

    TGeoVolume *tGasMenExit=geom->MakeParaboloid("tGasMenExit",Si,0.,header->UpMat.TarEntrHoleRad,header->UpMat.MeniskSize/2.);
    TGeoRotation *rotExitMen=new TGeoRotation("rotExitMen",0.,180.,0.);
    rotExitMen->RegisterYourself();

  // Unifying three previous forms into one whole gaseous body

    TGeoCompositeShape *tgg=new TGeoCompositeShape("tgg","ttargetgas:TargetShift+tGasMenEntr:MeniskShift+tGasMenExit:rotExitMen");
    TGeoVolume *tgas=new TGeoVolume("tgas",tgg,Si);

  // The whole gaseous body must be shifted forward to the origin of the frame by GasShift and then rotated around the Y-axis if needed

    TGeoTranslation *GasShift=new TGeoTranslation("GasShift",header->UpMat.TarYshift,-header->UpMat.TarXshift,(header->UpMat.MeniskSize+header->UpMat.TarHeight)/2.+header->UpMat.TarZshift);
    GasShift->RegisterYourself();
    TGeoTranslation *Zshift=new TGeoTranslation("Zshift",header->UpMat.TarYshift,-header->UpMat.TarXshift,header->UpMat.TarZshift);
    Zshift->RegisterYourself();
    TGeoCombiTrans *postfex=new TGeoCombiTrans();
    postfex->RegisterYourself();
    *postfex = *rottar*(*GasShift);
    TGeoCombiTrans *posttar=new TGeoCombiTrans();
    posttar->RegisterYourself();
    *posttar = *rottar*(*Zshift);

  // To construct the foil shell surrounding the gaseous body, we repeat the previous procedure twice, for the inner and outer shapes of the foil, and subtruct one from another

    TGeoVolume *tTargetCell=geom->MakeTube("tTargetCell",Si,header->UpMat.TarEntrHoleRad,header->UpMat.TarRadius,header->UpMat.TarThick/2.);

    TGeoVolume *tEntrFoilInt=geom->MakeParaboloid("tEntrFoilInt",Si,0.,header->UpMat.TarEntrHoleRad,header->UpMat.MeniskSize/2.);
    TGeoVolume *tEntrFoilExt=geom->MakeParaboloid("tEntrFoilExt",Si,0.,header->UpMat.TarEntrHoleRad+3.*header->UpMat.FoilThick,header->UpMat.MeniskSize/2.+header->UpMat.FoilThick);
    TGeoVolume *tEntrFoilHole=geom->MakeTube("tEntrFoilHole",Si,0.,header->UpMat.TarEntrHoleRad+3.*header->UpMat.FoilThick,1.01*header->UpMat.FoilThick/2.);
    TGeoTranslation *HoleShift=new TGeoTranslation("HoleShift",0.,0.,header->UpMat.MeniskSize/2.+header->UpMat.FoilThick/2.);
    HoleShift->RegisterYourself();

    TGeoVolume *tExitFoilInt=geom->MakeParaboloid("tExitFoilInt",Si,0.,header->UpMat.TarEntrHoleRad,header->UpMat.MeniskSize/2.);
    TGeoVolume *tExitFoilExt=geom->MakeParaboloid("tExitFoilExt",Si,0.,header->UpMat.TarEntrHoleRad+3.*header->UpMat.FoilThick,header->UpMat.MeniskSize/2.+header->UpMat.FoilThick);
    TGeoVolume *tExitFoilHole=geom->MakeTube("tExitFoilHole",Si,0.,header->UpMat.TarEntrHoleRad+3.*header->UpMat.FoilThick,1.01*header->UpMat.FoilThick/2.);

    TGeoCompositeShape *tfg1=new TGeoCompositeShape("tfg1","(tEntrFoilExt-tEntrFoilHole:HoleShift):MeniskShift-tEntrFoilInt:MeniskShift");
    TGeoCompositeShape *tfg2=new TGeoCompositeShape("tfg2","(tExitFoilExt-tExitFoilHole:HoleShift):rotExitMen-tExitFoilInt:rotExitMen");
    TGeoVolume *tfoilen=new TGeoVolume("tfoilen",tfg1,Si);
    TGeoVolume *tfoilex=new TGeoVolume("tfoilex",tfg2,Si);

    top->AddNode(tgas,1,postfex);
    top->AddNode(tfoilen,1,postfex);
    top->AddNode(tfoilex,1,postfex);
    top->AddNode(tTargetCell,1,rottar);
  }
  /**********************************************************************************************************************************************/
  /**************************************************** Heat screen: ****************************************************************************/
  /**********************************************************************************************************************************************/
  if(!strcmp(header->UpMat.HeatScreenAns,"yes"))
  {
    TGeoVolume *Hscr=geom->MakeTube("Hscr",Si,header->UpMat.InHscrRad,header->UpMat.InHscrRad+header->UpMat.HscrWallWidth,header->UpMat.HscrHeight/2.);

    TGeoVolume *HscrEntHole=geom->MakeTube("HscrEntHole",Si,0.,header->UpMat.EntrHRad,header->UpMat.InHscrRad);
    TGeoTranslation *HEntHolShift=new TGeoTranslation("HEntHolShift",0.,0.,-header->UpMat.InHscrRad);
    HEntHolShift->RegisterYourself();

    TGeoVolume *HscrExHole=geom->MakeBox("HscrExHole",Si,header->UpMat.ExHX/2.,header->UpMat.ExHY/2.,header->UpMat.InHscrRad);
    TGeoTranslation *HExHolShift=new TGeoTranslation("HExHolShift",0.,0.,header->UpMat.InHscrRad);
    HExHolShift->RegisterYourself();

    TGeoCompositeShape *HeatScr=new TGeoCompositeShape("HeatScr","Hscr:rot0-(HscrEntHole:HEntHolShift+HscrExHole:HExHolShift)");
    TGeoVolume *tHeatScreen=new TGeoVolume("tHeatScreen",HeatScr,Si);

    TGeoVolume *tHscrFoil=geom->MakeTube("tHscrFoil",Si,header->UpMat.InHscrRad-header->UpMat.HeatScreenThick,header->UpMat.InHscrRad,header->UpMat.HscrHeight/2.);

    tHeatScreen->SetLineColor(1);
    tHscrFoil->SetLineColor(2);
    top->AddNode(tHeatScreen,1);
    top->AddNode(tHscrFoil,1,rot0);
  }
  geom->CloseGeometry();
  cout << "Target geometry creating finished" << endl;
}
//-----------------------------------------------------------------------------
void ERHe8Analyzer::WhatParticlesInOut(ERParticle* ptr,char* str,int N){
  char* poi;
  ptr->NameOfNucleus = new char [strlen(str)+1];
  poi = strtok(str,"+");
  strcpy(ptr->NameOfNucleus,str);
  for(int i=1;i<N;i++)
  {
    poi = strtok(NULL,"+");
    (ptr+i)->NameOfNucleus = new char [strlen(poi)+1];
    strcpy((ptr+i)->NameOfNucleus,poi);
  }
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void ERHe8Analyzer::ReadDeDx(){
  EjMat = new DownstreamMatter*[header->NofDetPart];
  for(int i =0; i<header->NofDetPart;i++)
    EjMat[i] = new DownstreamMatter();
  char Matter[128];
  TString filePath = gSystem->Getenv("VMCWORKDIR") + TString("/input/eloss/");
  for(int ip=0;ip<header->NofDetPart;ip++)
  {   
    strcpy(Matter,filePath.Data());
    strcat(Matter,ejectile[0][ip][0].NameOfNucleus);
    strcat(Matter,"_");
    strcat(Matter,"si");
    ReadRint(Matter,EjMat[ip]->ej_si);
  }
  for(int ip=0;ip<header->NofDetPart;ip++)
  {   
    strcpy(Matter,filePath.Data());
    strcat(Matter,ejectile[0][ip][0].NameOfNucleus);
    strcat(Matter,"_");
    strcat(Matter,"csi");
    ReadRint(Matter,EjMat[ip]->ej_csi);
  } 
  for(int ip=0;ip<header->NofDetPart;ip++)
  {   
    strcpy(Matter,filePath.Data());
    strcat(Matter,ejectile[0][ip][0].NameOfNucleus);
    strcat(Matter,"_");
    strcat(Matter,header->UpMat.TarFoilMatter);
    ReadRint(Matter,EjMat[ip]->ej_TARwin);
  }
  for(int ip=0;ip<header->NofDetPart;ip++)
  {   
    strcpy(Matter,filePath.Data());
    strcat(Matter,ejectile[0][ip][0].NameOfNucleus);
    strcat(Matter,"_");
    strcat(Matter,target->NameOfNucleus);
    ReadRint(Matter,EjMat[ip]->ej_target);
  }
}
//-----------------------------------------------------------------------------
void ERHe8Analyzer::PrintReaction(){
  printf("************ ReactionInput is decoded **********************\n");
  printf("So we're studying the following reaction:\n");
  printf("Projectile %s (M=%lf MeV)\n",projectile->NameOfNucleus,projectile->Mass);
  printf("interacts with the target nucleus %s (M=%lf MeV)\n",
  target->NameOfNucleus,target->Mass);
  printf(" \n");
  printf("As a result of the %s reaction we have:\n",header->ReIN.Mechanism);
  printf("%i detected particles:\n",header->NofDetPart);
  for(int ip=0;ip<header->NofDetPart;ip++) printf("%s (M=%lf MeV)\n",ejectile[0][ip][0].NameOfNucleus,ejectile[0][ip][0].Mass);
  printf("And %i unobserved particles:\n",header->NofUnObsPart);
  for(int ip=header->NofDetPart;ip<header->NofDetPart+header->NofUnObsPart;ip++) printf("%s (M=%lf MeV)\n",ejectile[0][ip][0].NameOfNucleus,ejectile[0][ip][0].Mass);
  printf(" \n");
  /*
  if(NofPartRes>1)
  {
    printf("There is a %i-body nucleon unstable resonance in %s:\n",NofPartRes,ResonanceDecay[0].NameOfNucleus);
    for(int ip=1;ip<=NofPartRes;ip++) printf("%s (M=%lf MeV)\n",ResonanceDecay[ip].NameOfNucleus,ResonanceDecay[ip].Mass);
  }
  else printf("%i particles in resonance\n",NofPartRes);*/
  printf(" \n");
  printf("Qreaction=%lf MeV\n",Qreaction);
  printf("************************************************************\n");
  printf("Energy of projectile:\n");
  printf("DIPOLE2: E=%lf +/- %lf MeV (%lfA MeV)\n",projectile->Part.E()-projectile->Mass,
    BeamSpread,(projectile->Part.E()-projectile->Mass)/projectile->AtMass);
}
//-----------------------------------------------------------------------------
void ERHe8Analyzer::TelescopeThresholds(){
  double Threshold[Ntelescopes][header->NofDetPart];
  printf("Telescope thresholds:\n");
  for(int it=0;it<Ntelescopes;it++)
  {
    for(int ip=0;ip<header->NofDetPart;ip++)
    {
      dT = (projectile->Part.E()-projectile->Mass)/5000.;
      Tb = 0.;
      Ta = 10.*dT;
      for(int id=0;id<NDet[it][0];id++)
      {
        do
        {
          Tb = EiEo(EjMat[ip]->ej_si,Ta,Det[it][0][id].Thick);
          Ta += dT;
        } while(Tb<=0.&&Ta<999.);
      }
      Threshold[it][ip] = Ta;
      printf("Tel%i. %s: threshold %lf MeV\n",it,ejectile[it][ip][0].NameOfNucleus,Threshold[it][ip]);
    } 
  }
  printf("************************************************************\n");
}
//-----------------------------------------------------------------------------
ClassImp(ERHe8Analyzer)
