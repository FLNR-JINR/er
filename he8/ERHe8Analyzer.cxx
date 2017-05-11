#include "ERHe8Analyzer.h"

#include<iostream>
using namespace std;

#include "TGeoMaterial.h"
#include "TGeoMedium.h"
#include "TGeoCompositeShape.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

#include "ERHe8EventHeader.h"

//----------------------------------------------------------------------------
ERHe8Analyzer::ERHe8Analyzer()
  : FairTask("ERHe8Analyzer"),
  fEvent(0),
  ReIN(new ReactionDataInput()),
  SimDat(new SimulationData()),
  UpMat (new UpstreamMatter()),
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
  target(new Particle()),
  projectile(new Particle()),
  CM0(new Particle()),
  fMwpcEvent(NULL),
  fTofEvent(NULL),
  fDsrdEvent(NULL),
  fTelescopeEvent(NULL),
  mis(NULL),
  RawT(new RawTrack()),
  trackD(new TrackData()),
  tel(new ERTelData()),
  inclu(new ERInclusiveData())
{
}
//----------------------------------------------------------------------------
ERHe8Analyzer::ERHe8Analyzer(Int_t verbose)
  : FairTask("ERHe8Analyzer", verbose),
  fEvent(0),
  ReIN(new ReactionDataInput()),
  SimDat(new SimulationData()),
  UpMat (new UpstreamMatter()),
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
  target(new Particle()),
  projectile(new Particle()),
  CM0(new Particle()),
  fMwpcEvent(NULL),
  fTofEvent(NULL),
  fDsrdEvent(NULL),
  mis(NULL),
  fTelescopeEvent(NULL),
  RawT(new RawTrack()),
  trackD(new TrackData()),
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
  fMwpcEvent = (ERMwpcEvent*)ioman->GetObject("MwpcEvent.");
  if (!fMwpcEvent)
      Fatal("Init", "Can`t find branch in input file!");

  fTofEvent = (ERTofCalEvent*)ioman->GetObject("TofCalEvent.");
  if (!fTofEvent)
      Fatal("Init", "Can`t find branch in input file!");

  fDsrdEvent = (ERDsrdCalEvent*)ioman->GetObject("DsrdCalEvent.");
  if (!fDsrdEvent)
      Fatal("Init", "Can`t find branch in input file!");

  fTelescopeEvent = (ERTelescopeCalEvent*)ioman->GetObject("TelescopeCalEvent.");
  if (!fTelescopeEvent)
      Fatal("Init", "Can`t find branch in input file!");

  ioman->Register("RawT.", "Analys",RawT, kTRUE);
  ioman->Register("trackD.", "Analys",trackD, kTRUE);
  ioman->Register("tel.", "Analys",tel, kTRUE);
  ioman->Register("inclu.", "Analys",inclu, kTRUE);

  ReadInputData();
  
  ReadTelescopeParameters();

  ReactionPreparation();

  InitMemory();

  InitParticlesInOutputs();

  CheckInOutAZ();
  
  CreateTelescopeGeometry();

  ReadDeDx();

  DefineBeamEnergy();

  TelescopeThresholds();

  PrintReaction();

  ElossTOFaMWPCaTarget();

  return kSUCCESS;
}
//----------------------------------------------------------------------------
void ERHe8Analyzer::CheckInOutAZ(){
  cout << "Check for Zin=Zout and Ain=Aout" << endl;
  int InAtNumber = projectile->AtNumber + target->AtNumber;
  int OutAtNumber = 0;
  for(int ip=0;ip<NofDetPart+NofUnObsPart;ip++) OutAtNumber += ejectile[0][ip][0].AtNumber;

  if(InAtNumber!=OutAtNumber) {printf("In ReactionInput.dat Zin is not equal to Zout!!!\n");}

  InAtNumber = projectile->AtMass + target->AtMass;
  OutAtNumber = 0;
  for(int ip=0;ip<NofDetPart+NofUnObsPart;ip++) OutAtNumber += ejectile[0][ip][0].AtMass;
  if(InAtNumber!=OutAtNumber) {printf("In ReactionInput.dat Ain is not equal to Aout!!!\n");}
}
//----------------------------------------------------------------------------
void ERHe8Analyzer::InitParticlesInOutputs(){
    Particle intermed[NofDetPart+NofUnObsPart]; // любая промежуточная частица
    TString filePath  = gSystem->Getenv("VMCWORKDIR") + TString("/input/StableNuclei.dat");
    char* WayMass= const_cast<char*>(filePath.Data());
    projectile->ParticleID(projname,WayMass);
    target->ParticleID(tarname,WayMass);
    target->Part.SetPxPyPzE(0.,0.,0.,target->Mass);

    WhatParticlesInOut(&intermed[0],DetectedPart,NofDetPart);
    WhatParticlesInOut(&intermed[NofDetPart],UnObservedPart,NofUnObsPart);  

    cout << "Define particles in the output channel" << endl;
    
    for(int it=0;it<Ntelescopes;it++)
    {
      for(int ip=0;ip<NofDetPart+NofUnObsPart;ip++)
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
      for(int ip=0;ip<NofDetPart;ip++)
      {
        for(int imu=0;imu<NDivXYMax;imu++)
        {
          for(int ipx=0;ipx<NofDetPart+NofUnObsPart;ipx++)
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
          for(int ip=0;ip<NofDetPart;ip++)
          {
            for(int ipx=0;ipx<NofDetPart;ipx++)
            {
              if(ip!=ipx)
              {
                participants[it][ip][itx][ipx].NameOfNucleus = new char [2];
                strcpy(participants[it][ip][itx][ipx].NameOfNucleus,"pa");
                spectator[it][ip][itx][ipx].NameOfNucleus = new char [2];
                strcpy(spectator[it][ip][itx][ipx].NameOfNucleus,"sp");
                participants[it][ip][itx][ipx].Mass = 
                  ejectile[0][ip][0].Mass + ejectile[0][ipx][0].Mass;
                for(int ipy=0;ipy<NofDetPart+NofUnObsPart;ipy++)
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
// -------------------------------------------------------------------------
void ERHe8Analyzer::ReactionPreparation(){
  cout << " Separate Input and Output channels " << endl;
  char ReaNa[32];
  char InputChannel[32];
  char OutputChannel[32];
  char OutputChannelTemp[32];
  char Resonance[32];
  char ResonanceTemp[32];
  strcpy(ReaNa,ReIN->ReactionName);
  char* plett;
  plett = strchr(ReIN->ReactionName,'-');
  plett+=2;
  strcpy(OutputChannelTemp,plett);
  strcpy(ResonanceTemp,plett);
  plett = strtok(ReIN->ReactionName,"-");
  strcpy(InputChannel,plett);

  cout << "Define if there is any resonance in the Output channel" << endl;
  int NofPartRes = 0;
  plett = strchr(OutputChannelTemp,'[');
  if(plett!=NULL)
  {
    plett = strtok(OutputChannelTemp,"[");
    strcpy(OutputChannel,plett);
    plett = strtok(NULL,"]");
    strcat(OutputChannel,plett);
    plett = strchr(ResonanceTemp,']');plett++;
    if(plett!=NULL) strcat(OutputChannel,plett);
    plett = strtok(ResonanceTemp,"[");
    plett = strtok(NULL,"]");
    strcpy(ResonanceTemp,plett);
    plett = strtok(ResonanceTemp,"[");
    strcpy(ResonanceTemp,plett);
    plett = strchr(ResonanceTemp,')');
    if(plett!=NULL)
    {
      plett = strtok(ResonanceTemp,")");
      strcpy(Resonance,plett);
      plett = strtok(NULL,")");
      strcat(Resonance,plett);
    }
    else strcpy(Resonance,ResonanceTemp);
    NofPartRes = HowMuchParticles(Resonance);
    printf("Resonance is %s\n",Resonance);
  }
  else strcpy(OutputChannel,OutputChannelTemp);
  
  cout << "Separate Detected and Unobserved particles in the Output channel" << endl;
  char zero[]="";
  NofUnObsPart = 0;
  strcpy(DetectedPart,OutputChannel);
  plett = strtok(DetectedPart,"()");
  strcpy(DetectedPart,plett);
  plett = strchr(OutputChannel,')');
  plett++;
  if(strcmp(plett,zero)) 
  {
    plett++;strcpy(UnObservedPart,plett);
    NofUnObsPart = HowMuchParticles(UnObservedPart);
  }
  
  cout << "How much Input and Detected particles" << endl;
  NofInPart = HowMuchParticles(InputChannel);
  if(NofInPart<2||NofInPart>2) {printf("Wrong number of particles in the Input channel\n");}
  NofDetPart = HowMuchParticles(DetectedPart);
  if(NofDetPart==0) {printf("Wrong number of detected particles\n");}

  cout  << "Define particles in the input channel" << endl;

  plett = strtok(InputChannel,"+");
  strcpy(projname,plett);
  plett = strtok(NULL,"+");
  strcpy(tarname,plett);
}
//----------------------------------------------------------------------------
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

  //Ntelescopes NLayMax NofDetPart NDivXYMax
  al = new double***[Ntelescopes];
  for (int i =0; i <Ntelescopes; i++){
    al[i] = new double**[NLayMax];
    for (int j = 0; j < NLayMax; j++){
      al[i][j] = new double*[NofDetPart];
      for (int k = 0; k < NofDetPart; k++){
        al[i][j][k] = new double[NDivXYMax];
      }
    }
  }

  // Ntelescopes NofDetPart Ntelescopes NofDetPart
  spectator = new Particle***[Ntelescopes];
  participants = new Particle***[Ntelescopes];
  for (int i = 0; i<Ntelescopes; i++){
    spectator[i] = new Particle**[NofDetPart];
    participants[i] = new Particle**[NofDetPart];
    for (int j = 0; j < NofDetPart; j++){
      spectator[i][j] = new Particle*[Ntelescopes];
      participants[i][j] = new Particle*[Ntelescopes];
      for (int k = 0; k < Ntelescopes; k++){
        spectator[i][j][k] = new Particle[NofDetPart];
        participants[i][j][k] = new Particle[NofDetPart];
      }
    }
  }

  //Ntelescopes NLayMax NDetMax NofDetPart
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
        HitX[i][j][k] = new int[NofDetPart];
        HitY[i][j][k] = new int[NofDetPart];
        HitXT[i][j][k] = new int[NofDetPart];
        HitYT[i][j][k] = new int[NofDetPart];
      }
    }
  }
  //Ntelescopes NofDetPart+NofUnObsPart
  ejectile = new Particle**[Ntelescopes];
  for (int i=0; i<Ntelescopes; i++){
    ejectile[i] = new Particle*[NofDetPart+NofUnObsPart];
    for (int j=0; j<NofDetPart+NofUnObsPart; j++)
      ejectile[i][j] = new Particle[NDivXYMax];
  }
  //Ntelescopes NofDetPart+NofUnObsPart NDivXYMax
  mis = new Particle**[Ntelescopes];
  for (int i = 0; i<Ntelescopes; i++){
    mis[i] = new Particle*[NofDetPart+NofUnObsPart];
    for (int j = 0; j<NofDetPart+NofUnObsPart; j++){
      mis[i][j] = new Particle[NDivXYMax];
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
void ERHe8Analyzer::Tof(){
  TRandom Rnd;
  char ShowTrack[10];
  double tof_offset = 87.98;
  //      double tof_offset = 84.;
  double dt_F3,dt_F4,t_F3,t_F4;
  FairRun* run = FairRun::Instance();
  ERHe8EventHeader* header = (ERHe8EventHeader*)run->GetEventHeader();
  header->mtrack = i_flag_MW;
  //RawD.mtrack = 1;i_flag_MW = 1;

  if(ReIN->TOFis&&i_flag_MW)
  {
    header->mbeam = 0;
    // ****************** measurement of TOF spread around tof_0, calculated from the magnetic field in the 2nd dipole ************************
    //        fTofEvent->tF3l = fTofEvent->tF3l - fTofEvent->tF4r+ParD.CLB[3][0][0][1];
    //        fTofEvent->tF3r = fTofEvent->tF3r - fTofEvent->tF4r+ParD.CLB[3][1][0][1];
    //        fTofEvent->tF4l = fTofEvent->tF4l - fTofEvent->tF4r+ParD.CLB[4][1][0][1];
    //        fTofEvent->tF4r = fTofEvent->tF4r - fTofEvent->tF4r+ParD.CLB[4][0][0][1];
    // ****************************************************************************************************************************************
          
            dt_F3 = (fTofEvent->tF3l-fTofEvent->tF3r);
            dt_F4 = (fTofEvent->tF4l-fTofEvent->tF4r);

    //        if(fabs(dt_F4)<=UpMat->tF4_dlt&&fabs(dt_F3)<=UpMat->tF3_dlt&&
    //        fabs(fTofEvent->tF3l)<=UpMat->tF3l_rng&&fabs(fTofEvent->tF3r)<=UpMat->tF3r_rng&&
    //        fabs(fTofEvent->tF4l)<=UpMat->tF4l_rng&&fabs(fTofEvent->tF4r)<=UpMat->tF4r_rng)
    //        if(fabs(dt_F4)<=UpMat->tF4_dlt&&fabs(dt_F3)<=UpMat->tF3_dlt)
    //        {
    //printf("*********************************\n");
    //          t_F3 = (Rnd.Gaus(fTofEvent->tF3l,UpMat->TofRes/2.35)+Rnd.Gaus(fTofEvent->tF3r,UpMat->TofRes/2.35))/2;
    //          t_F4 = (Rnd.Gaus(fTofEvent->tF4l,UpMat->TofRes/2.35)+Rnd.Gaus(fTofEvent->tF4r,UpMat->TofRes/2.35))/2.;
          t_F4 = Rnd.Gaus(fTofEvent->tF4l,UpMat->TofRes/2.35);
          t_F3 = Rnd.Gaus(fTofEvent->tF3r,UpMat->TofRes/2.35);
    //          t_F4 = Rnd.Gaus((fTofEvent->tF4l+fTofEvent->tF4r)/2,UpMat->TofRes/2.35);
    // ****************** measurement of TOF spread around tof_0, calculated from the magnetic field in the 2nd dipole ************************
    //          fTofEvent->tofb = t_F4 - t_F3 + tof_0;
    // ********************************* measurement of absolute TOF value tof_offset = dT1-L0*(dT1-dT0)/(L1-L0)*******************************
          fTofEvent->tofb = t_F4 - t_F3 + tof_offset;
    //if(fTofEvent->tofb<130.) 
    //{printf("ntF3l=%i,ntF3r=%i,ntF4l=%i,ntF4r=%i\n",RawD.ntF3l,RawD.ntF3r,RawD.ntF4l,RawD.ntF4r);
    //printf("tF3l=%lf,tF3r=%lf,tF4l=%lf,tF4r=%lf,  TOF=%lf\n",fTofEvent->tF3l,fTofEvent->tF3r,fTofEvent->tF4l,fTofEvent->tF4r,fTofEvent->tofb);
    //printf("t_F3=%lf,t_F4=%lf,(fTofEvent->tF3l+fTofEvent->tF3r)/2=%lf,(fTofEvent->tF4l+fTofEvent->tF4r)/2=%lf,  TOF=%lf\n",t_F3,t_F4,(fTofEvent->tF3l+fTofEvent->tF3r)/2,
    //(fTofEvent->tF4l+fTofEvent->tF4r)/2,(fTofEvent->tF4l+fTofEvent->tF4r-fTofEvent->tF3l-fTofEvent->tF3r)/2+ tof_offset);}
    // ****************************************************************************************************************************************
          if(fTofEvent->aF4r+fTofEvent->aF4l>500.&&fTofEvent->tofb<150.&&fTofEvent->tofb>60.)
          {

            beta_b = UpMat->PlasticDist/fTofEvent->tofb/slight;

            if(beta_b>0.&&beta_b<=1.)
            {
              header->mbeam = 1;
              strcpy(ShowTrack,"invisible");
              gamma_b = 1./sqrt(1.-beta_b*beta_b);
              p_beam  = beta_b*projectile->Mass*gamma_b;
              projectile->Part.SetPxPyPzE(p_beam*sin(Vbeam.Theta())*cos(Vbeam.Phi()),
                p_beam*sin(Vbeam.Theta())*sin(Vbeam.Phi()),p_beam*cos(Vbeam.Theta()),
                p_beam/beta_b);

              Tb = UpstreamEnergyLoss(UpMat,projectile,ReIN->TOFis,ReIN->TRACKINGis,ShowTrack);
              if(Tb>0.1&&!strcmp(UpMat->HeatScreenAns,"yes")) 
                Tb = EiEo(UpMat->beam_TARwin,Tb,UpMat->HeatScreenThick/cos(Vbeam.Theta()));
              if(Tb>0.1) Tb = EiEo(UpMat->beam_TARwin,Tb,UpMat->FoilThick/cos(Vbeam.Theta()));
              else Tb = 0.;
              range = UpMat->TarThick*UpMat->TarPress*TempNorm/UpMat->TarTemp/cos(Vbeam.Theta())/2.;
              if(Tb>0.1) Tb = EiEo(UpMat->beam_target,Tb,range);
              else Tb = 0.;
              p_beam = sqrt(pow(Tb+projectile->Mass,2)-pow(projectile->Mass,2));
              projectile->Part.SetPxPyPzE(p_beam*sin(Vbeam.Theta())*
                cos(Vbeam.Phi()),p_beam*sin(Vbeam.Theta())*sin(Vbeam.Phi()),
                p_beam*cos(Vbeam.Theta()),Tb+projectile->Mass);
              CM0->Part = projectile->Part + target->Part;
              projectile->Part.Boost(-CM0->Part.BoostVector());
              target->Part.Boost(-CM0->Part.BoostVector());
              t_cm0 = projectile->Part.E()+target->Part.E()-projectile->Mass-target->Mass;
              t_cm = t_cm0 + Qreaction;
              projectile->Part.Boost(CM0->Part.BoostVector());
              target->Part.Boost(CM0->Part.BoostVector());
              if(t_cm>0.) good_mbeam++;
              else header->mbeam = 0;
              fTofEvent->tb = projectile->Part.E()-projectile->Mass;
              fTofEvent->tcm = t_cm;              

              trackD->xbt = MdistX+(-UpMat->MWgasThick/2-UpMat->MWclosDist+UpMat->TarZshift)*sin(Vbeam.Theta())*cos(Vbeam.Phi())/cos(Vbeam.Theta());
              trackD->ybt = MdistY+(-UpMat->MWgasThick/2-UpMat->MWclosDist+UpMat->TarZshift)*sin(Vbeam.Theta())*sin(Vbeam.Phi())/cos(Vbeam.Theta());
              trackD->zbt = UpMat->TarZshift;
            } /* if(beta_b>0.&&beta_b<=1.) */  
          } /* if(fTofEvent->aF4r+fTofEvent->aF4l>500.) */
          //        } /* if(fabs(dt_F4)<=UpMat->tF4_dlt&&fabs(dt_F3)<=UpMat->tF3_dlt&& */
      } /* if(ReIN.TOFis) */
      if(i_flag_MW==1&&sqrt(pow(trackD->xbt,2)+pow(trackD->ybt,2))<UpMat->TarEntrHoleRad) header->mtrack = 1;
}
//----------------------------------------------------------------------------
void ERHe8Analyzer::MWPC(){
  i_flag_MW = 0;
  double tarcoord[3];
  if(ReIN->TRACKINGis)
      {
        for(int iMW=0;iMW<UpMat->MWNwires;iMW++)
        {
          RawT->nMW11[iMW] = 0;RawT->nMW12[iMW] = 0;
          RawT->nMW21[iMW] = 0;RawT->nMW22[iMW] = 0;
        }
        RawT->mMW12 = fMwpcEvent->nx1;
        RawT->mMW11 = fMwpcEvent->ny1;
        RawT->mMW22 = fMwpcEvent->nx2;
        RawT->mMW21 = fMwpcEvent->ny2;
        
        if(RawT->mMW11>=1&&RawT->mMW12>=1) good_mw1++;
        if(RawT->mMW21>=1&&RawT->mMW22>=1) good_mw2++;
        if(RawT->mMW11>=1&&RawT->mMW12>=1&&RawT->mMW21>=1&&RawT->mMW22>=1) good_mw++;

        for(int iMW=1;iMW<=RawT->mMW11;iMW++) RawT->nMW11[iMW-1] = fMwpcEvent->y1[iMW-1];
        for(int iMW=1;iMW<=RawT->mMW12;iMW++) RawT->nMW12[iMW-1] = fMwpcEvent->x1[iMW-1];
        for(int iMW=1;iMW<=RawT->mMW21;iMW++) RawT->nMW21[iMW-1] = fMwpcEvent->y2[iMW-1];
        for(int iMW=1;iMW<=RawT->mMW22;iMW++) RawT->nMW22[iMW-1] = fMwpcEvent->x2[iMW-1];

        RawT->mcMW11 = mcluMW(RawT->mMW11,RawT->nMW11);
        RawT->mcMW12 = mcluMW(RawT->mMW12,RawT->nMW12);
        RawT->mcMW21 = mcluMW(RawT->mMW21,RawT->nMW21);
        RawT->mcMW22 = mcluMW(RawT->mMW22,RawT->nMW22);
        
        int i_flag_MW1 = 0;
        int i_flag_MW2 = 0;

        if(RawT->mcMW11==1&&RawT->mcMW12==1) {i_flag_MW1 = 1;goodclu_mw1++;}
        else if(RawT->mcMW11>1||RawT->mcMW12>1) badclu_mw1++;
        if(RawT->mcMW21==1&&RawT->mcMW22==1) {i_flag_MW2 = 1;goodclu_mw2++;}
        else if(RawT->mcMW21>1||RawT->mcMW22>1) badclu_mw2++;
        if(i_flag_MW1&&i_flag_MW2) {i_flag_MW = 1;goodclu_mw++;}
        else badclu_mw++;

        if(i_flag_MW)
        {
          char MWid[]="MWfar";
          char XY[]="X";
          trackD->xmw1=coordMW(UpMat,RawT,MWid,XY);
          strcpy(XY,"Y");
          trackD->ymw1=coordMW(UpMat,RawT,MWid,XY);
          strcpy(MWid,"MWclo");
          strcpy(XY,"X");
          trackD->xmw2=coordMW(UpMat,RawT,MWid,XY);
          strcpy(XY,"Y");
          trackD->ymw2=coordMW(UpMat,RawT,MWid,XY);
          
          trackD->ymw1 += (trackD->ymw2-trackD->ymw1)*(UpMat->MWXYdist)/(UpMat->MWclosDist-UpMat->MWfarDist);
          trackD->ymw2 = trackD->ymw1 + (trackD->ymw2-trackD->ymw1)*
            (UpMat->MWclosDist-UpMat->MWfarDist+UpMat->MWXYdist/2+UpMat->MWgasThick/2)/(UpMat->MWclosDist-UpMat->MWfarDist);
          trackD->xmw2 = trackD->xmw1 + (trackD->xmw2-trackD->xmw1)*
            (UpMat->MWclosDist-UpMat->MWfarDist-UpMat->MWXYdist/2+UpMat->MWgasThick/2)/(UpMat->MWclosDist-UpMat->MWfarDist);
          VmwCl.SetXYZ(trackD->xmw2,trackD->ymw2,UpMat->MWclosDist);
          VmwFa.SetXYZ(trackD->xmw1,trackD->ymw1,UpMat->MWfarDist);
          Vbeam = (VmwCl - VmwFa);
          trackD->thb = Vbeam.Theta()/rad;
          trackD->phib = Vbeam.Phi()/rad;
          Vbeam.GetXYZ(tarcoord);
          MdistX = trackD->xmw2;
          MdistY = trackD->ymw2;
          MdistZ = UpMat->MWclosDist+UpMat->MWgasThick/2;
    //**** xbdet and ybdet are useful in the case when beam particles can get in the detector ************************************/
    //          for(it=0;it<Ntelescopes;it++)
    //          {
    //            for(il=0;il<layer[it];il++)
    //            {
    //              for(id=0;id<NDet[it][il];id++)
    //              {
    //                Vbeam = (VmwCl - VmwFa)*((abs(UpMat->MWfarDist)-UpMat->MWXYdist/2+Det[it][il][id].Dist)/(UpMat->MWclosDist-UpMat->MWfarDist-
    //                  UpMat->MWXYdist/2+UpMat->MWgasThick/2));
    //                Vbeam.GetXYZ(tarcoord);
    //                xbdet0[it][il][id] = trackD->xmw1 + tarcoord[0];
    //                ybdet0[it][il][id] = trackD->ymw1 + tarcoord[1];
    //              }
    //            }
    //          }
    /**** when simulating XY are randomized twice: as a result we have coorinates played out and reconstructed *******************/
    /**** when simulating both thb and rthb are witten down. Difference thb-rthb gives the accuracy of measurement ***************/
              /*
              if(ReIN->Simulation)
              {
                strcpy(MWid,"MWfar");
                strcpy(XY,"X");
                PlayXmw1=coordMW(&UpMat,&RawT,MWid,XY);
                strcpy(XY,"Y");
                PlayYmw1=coordMW(&UpMat,&RawT,MWid,XY);
                strcpy(MWid,"MWclos");
                strcpy(XY,"X");
                PlayXmw2=coordMW(&UpMat,&RawT,MWid,XY);
                strcpy(XY,"Y");
                PlayYmw2=coordMW(&UpMat,&RawT,MWid,XY);

                PlayYmw1 += (PlayYmw2-PlayYmw1)*(UpMat->MWXYdist)/(UpMat->MWclosDist-UpMat->MWfarDist);
                PlayYmw2 = PlayYmw1 + (PlayYmw2-PlayYmw1)*(UpMat->MWclosDist-UpMat->MWfarDist+UpMat->MWXYdist/2+UpMat->MWgasThick/2)/
                  (UpMat->MWclosDist-UpMat->MWfarDist);
                PlayXmw2 = PlayXmw1 + (PlayXmw2-PlayXmw1)*(UpMat->MWclosDist-UpMat->MWfarDist-UpMat->MWXYdist/2+UpMat->MWgasThick/2)/
                  (UpMat->MWclosDist-UpMat->MWfarDist);
                VmwCl.SetXYZ(PlayXmw2,PlayYmw2,UpMat->MWclosDist);
                VmwFa.SetXYZ(PlayXmw1,PlayYmw1,UpMat->MWfarDist);
                VbeamPlay = (VmwCl - VmwFa);
                Play.rthb = VbeamPlay.Theta()/rad;
                Play.rphib = VbeamPlay.Phi()/rad;
                VbeamPlay.GetXYZ(tarcoord);
                PdistX = PlayXmw2;
                PdistY = PlayYmw2;
                PdistZ = UpMat->MWclosDist+UpMat->MWgasThick/2;
            }*/
        } /* i_flag_MW */
      } /* ReIN.TRACKINGis */
}
//----------------------------------------------------------------------------
double ERHe8Analyzer::coordMW(UpstreamMatter* pT,RawTrack* pR,char* MWid,char* XY){
  double co = -1000.;
  double offset;
  double Sn;
  int iMW;
  int mMW;
  int nMW[16];
  char Xchoice[]="X";
  char Ychoice[]="Y";
  TRandom Rnd;
  if(!strcmp(MWid,"MWfar"))
  {
    if(!strcmp(XY,Xchoice))
    {
      iMW = pT->MWfarXNum/abs(pT->MWfarXNum);
      offset = pT->MWfarXshift;
      mMW = pR->mMW12;
      for(int k=0;k<16;k++) {nMW[k] = pR->nMW12[k];}
    }
    else if(!strcmp(XY,Ychoice))
    {
      iMW = pT->MWfarYNum/abs(pT->MWfarYNum);
      offset = pT->MWfarYshift;
      mMW = pR->mMW11;
      for(int k=0;k<16;k++) {nMW[k] = pR->nMW11[k];}
    }
    else
    {printf("hMW: it's not neither X nor Y\n");return co;}    
  }
  else if(!strcmp(MWid,"MWclo"))
  {
    if(!strcmp(XY,Xchoice))
    {
      iMW = pT->MWclosXNum/abs(pT->MWclosXNum);
      offset = pT->MWclosXshift;
      mMW = pR->mMW22;
      for(int k=0;k<16;k++) {nMW[k] = pR->nMW22[k];}
    }
    else if(!strcmp(XY,Ychoice))
    {
      iMW = pT->MWclosYNum/abs(pT->MWclosYNum);
      offset = pT->MWclosYshift;
      mMW = pR->mMW21;
      for(int k=0;k<16;k++) {nMW[k] = pR->nMW21[k];}
    }
    else
    {printf("hMW: it's not neither X nor Y\n");return co;}    
  }
  else
  {printf("hMW: bad number of MWPC\n");return co;}
  
  if(mMW<1) {return co;}
  if(mMW==1) {Sn = (double)nMW[mMW-1];}
  if(mMW>1)
  {
    int n = 0;
    for(int k=1;k<=mMW;k++)
    {n += nMW[k-1];}
    Sn = (double)n/(double)mMW;
  }
  Sn += (rand() %10000)/10000.-0.5;
  co = pT->MWstep*iMW*(Sn-(double)(pT->MWNwires+1)/2.)+offset;
  return co;
}
//----------------------------------------------------------------------------
int ERHe8Analyzer::mcluMW(int mMW,int* nMW){
  int i;
  if(mMW<=1) {i = mMW;}
  else
  {
    i = 1;
    for(int j=2;j<=mMW;j++)
    {
      if(nMW[j]-nMW[j-1]>1) {i++;}
    }
  }
  return i;
}
//----------------------------------------------------------------------------
void ERHe8Analyzer::Exec(Option_t* opt){
  cout << "Event "  << fEvent++ << endl; 
  Reset();
  MWPC();
  Tof();
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

  MuY[0][0][0]=fDsrdEvent->mC11;MuX[0][1][0]=fDsrdEvent->mC12;//MuX[0][2][0]=fDsrdEvent->mC13;
  MuY[1][0][0]=fTelescopeEvent->mC21;MuX[1][1][0]=fTelescopeEvent->mC22;MuX[1][2][0]=fTelescopeEvent->mC23;
  //MuX[2][0][0]=fTelescopeEvent->mC31;MuY[2][1][0]=fTelescopeEvent->mC32;MuX[2][2][0]=fTelescopeEvent->mC33;

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
  /*
  for(int imu=0;imu<=MuX[0][2][0];imu++)
  {
    DepoX[0][2][0][imu] = fDsrdEvent->eC13[imu];
    NhitX[0][2][0][imu] = fDsrdEvent->nC13[imu];
  }*/
  //////////////////////////////////////////////////////////////////////////////////
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
  //////////////////////////////////////////////////////////////////////////////////
    /*
    for(int imu=0;imu<=MuX[2][0][0];imu++)
    {
      DepoX[2][0][0][imu] = fTelescopeEvent->eC31[imu];
      NhitX[2][0][0][imu] = fTelescopeEvent->nC31[imu];
    }
    for(int imu=0;imu<=MuY[2][1][0];imu++)
    {
      DepoX[2][1][0][imu] = fTelescopeEvent->eC32[imu];
      NhitY[2][1][0][imu] = fTelescopeEvent->nC32[imu];
    }
    for(int imu=0;imu<=MuX[2][2][0];imu++)
    {
      DepoX[2][2][0][imu] = fTelescopeEvent->eC33[imu];
      NhitX[2][2][0][imu] = fTelescopeEvent->nC33[imu];
  }*/
  //cout << "Condition" << endl;
  if(MuY[0][0][0]==0&&NhitY[0][0][0][0]>0&&NhitY[0][0][0][0]<=abs(Det[0][0][0].NstripY)&&DepoX[0][0][0][0]>0.) mpd[0][0][0]=MuY[0][0][0]+1;

  if(MuX[0][1][0]==0&&NhitX[0][1][0][0]>0&&NhitX[0][1][0][0]<=abs(Det[0][1][0].NstripX)&&DepoX[0][1][0][0]>0.) mpd[0][1][0]=MuX[0][1][0]+1;

  if(MuY[1][0][0]==0&&NhitY[1][0][0][0]>0&&NhitY[1][0][0][0]<=abs(Det[1][0][0].NstripY)&&DepoX[1][0][0][0]>0.) mpd[1][0][0]=MuY[1][0][0]+1;
  
  if(MuX[1][1][0]==0&&NhitX[1][1][0][0]>0&&NhitX[1][1][0][0]<=abs(Det[1][1][0].NstripX)&&DepoX[1][1][0][0]>0.) mpd[1][1][0]=MuX[1][1][0]+1;
  /*
  if(MuX[2][0][0]==0&&NhitX[2][0][0][0]>0&&NhitX[2][0][0][0]<=abs(Det[2][0][0].NstripX)&&DepoX[2][0][0][0]>0.&&

  MuY[2][1][0]==0&&NhitY[2][1][0][0]>0&&NhitY[2][1][0][0]<=abs(Det[2][0][0].NstripY)&&DepoX[2][2][0][0]>0.) mpd[2][0][0]=MuX[2][0][0]+1;
  */
  if(mpd[0][0][0]==1&&mpd[0][1][0]==1) mp[0] = mpd[0][0][0];
  if(mpd[1][0][0]==1&&mpd[1][1][0]==1) mp[1] = mpd[1][0][0];
  //if(mpd[2][0][0]==1) mp[2] = mpd[2][0][0];

  FairRun* run = FairRun::Instance();
  ERHe8EventHeader* header = (ERHe8EventHeader*)run->GetEventHeader();
  header->mp1 = mp[0]; header->mp2 = mp[1]; header->mp3 = mp[2];
  if(mp[0]&&header->mtrack&&header->mbeam) good_mp0++;
  if(mp[1]&&header->mtrack&&header->mbeam) good_mp1++;
  //if(mp[2]&&header->mtrack&&header->mbeam) good_mp2++;

  
  int NxX1,NyX1,NxY1,NyY1,NxX2,NyX2,NxY2,NyY2;
  double dst,minX1,minY1,minX2,minY2;
  double dstmin = 1000.;
  int Nstep = 20;
  double Delta = 0.05;
  if(ReIN->DetectorTune)
  {
    //cout << " Vertex Reconstruction " << endl;
    Telescope DtempX1,DtempY1,DtempX2,DtempY2;
    DtempX1 = Det[0][0][0];
    NxX1 = NhitX[0][0][0][0];
    NyX1 = NhitY[0][0][0][0];
    DtempY1 = Det[0][1][0];
    NxY1 = NhitX[0][1][0][0];
    NyY1 = NhitY[0][1][0][0];
    
    DtempX2 = Det[1][0][0];
    NxX2 = NhitX[1][0][0][0];
    NyX2 = NhitY[1][0][0][0];
    DtempY2 = Det[1][1][0];
    NxY2 = NhitX[1][1][0][0];
    NyY2 = NhitY[1][1][0][0];

    if(mpd[0][0][0]&&mpd[0][1][0]&&mpd[1][0][0]&&mpd[1][1][0]&&header->mtrack&&header->mbeam)
    {
      DtempX2.OffsetX = Det[1][0][0].OffsetX - Nstep*Delta;
      for(int it=0;it<=2*Nstep;it++)
      {
        DtempY2.OffsetX = Det[1][1][0].OffsetX - Nstep*Delta;
        for(int il=0;il<=2*Nstep;il++)
        {
          DtempY1.OffsetX = Det[0][1][0].OffsetX - Nstep*Delta;
          for(int id=0;id<=2*Nstep;id++)
          {
            DtempX1.OffsetX = Det[0][0][0].OffsetX - Nstep*Delta;
            for(int is=0;is<=2*Nstep;is++)
            {
              Vert1.SetXYZ(0.,0.,0.);
              AngleDet[0][0] = Traject(&DtempX1,&DtempX1,NxX1,NyX1,Vert1);
              Vert1.SetXYZ(AngleDet[0][0].X(),AngleDet[0][0].Y(),AngleDet[0][0].Z());
              AngleDet[0][0] = Traject(&DtempY1,&DtempY1,NxY1,NyY1,AngleDet[0][0]);

              Vert2.SetXYZ(0.,0.,0.);
              AngleDet[1][1] = Traject(&DtempX2,&DtempX2,NxX2,NyX2,Vert2);
              Vert2.SetXYZ(AngleDet[1][1].X(),AngleDet[1][1].Y(),AngleDet[1][1].Z());
              AngleDet[1][1] = Traject(&DtempY2,&DtempY2,NxY2,NyY2,AngleDet[1][1]);
        
              dst = (AngleDet[0][0].Cross(AngleDet[1][1])*(Vert1-Vert2))/
                (AngleDet[0][0].Cross(AngleDet[1][1])).Mag();
              if(dst<dstmin) 
              {
                dstmin = dst;
                minX1 = DtempX1.OffsetX;
                minY1 = DtempY1.OffsetX;
                minX2 = DtempX2.OffsetX;
                minY2 = DtempY2.OffsetX;
              }
              DtempX1.OffsetX += Delta;
            }
            DtempY1.OffsetX += Delta;
          }
          DtempX2.OffsetX += Delta;
        }
        DtempY2.OffsetX += Delta;
      }
      printf("dst=%lf at DtempX1.OffseX=%lf,DtempY1.OffseX=%lf,DtempX2.OffseX=%lf,DtempY2.OffseX=%lf\n",dstmin,minX1,minY1,minX2,minY2);
    }
  }
  
  double tarcoord[3];
  if(ReIN->Vertex&&!ReIN->DetectorTune)
      {
        //cerr << " Trajectory reconstruction" << endl;
        if(mp[0]==1&&mp[1]==1)
        {
          // Very specific case: 11Li(or9Li) QFS or QFR with 2 muDSSD telescopes:
          Vert1.SetXYZ(0.,0.,0.);
          AngleDet[0][0] = Traject(&Det[0][0][0],&Det[0][0][0],NhitX[0][0][0][0],NhitY[0][0][0][0],Vert1);
          AngleDet[0][0].GetXYZ(tarcoord);
          cx[0][0] = tarcoord[0];
          cy[0][0] = tarcoord[1];
          cz[0][0] = tarcoord[2];
          Vert1.SetXYZ(cx[0][0],cy[0][0],cz[0][0]);
          if(MuX[0][1][0]) AngleDet[0][0] = Traject(&Det[0][1][0],&Det[0][1][0],NhitX[0][1][0][0],NhitY[0][1][0][0],Vert1);
          else if(MuX[0][1][1]) AngleDet[0][0] = Traject(&Det[0][1][1],&Det[0][1][1],NhitX[0][1][1][0],NhitY[0][1][1][0],Vert1);

          Vert2.SetXYZ(0.,0.,0.);
          AngleDet[1][0] = Traject(&Det[1][0][0],&Det[1][0][0],NhitX[1][0][0][0],NhitY[1][0][0][0],Vert2);
          cx[1][0] = AngleDet[1][0].X();
          cy[1][0] = AngleDet[1][0].Y();
          cz[1][0] = AngleDet[1][0].Z();
          Vert2.SetXYZ(cx[1][0],cy[1][0],cz[1][0]);
          if(MuX[1][1][0]) AngleDet[1][0] = Traject(&Det[1][1][0],&Det[1][1][0],NhitX[1][1][0][0],NhitY[1][1][0][0],Vert2);
          else if(MuX[1][1][1]) AngleDet[1][0] = Traject(&Det[1][1][1],&Det[1][1][1],NhitX[1][1][1][0],NhitY[1][1][1][0],Vert2);

          Vert1 = VertexPosition(AngleDet[0][0],AngleDet[1][0],Vert1,Vert2);
          trackD->xbt = Vert1.X();
          trackD->ybt = Vert1.Y();
          trackD->zbt = Vert1.Z();
        }
          
        if(mp[2]==1)
        {
          Vert2.SetXYZ(trackD->xbt,trackD->ybt,trackD->zbt);
          AngleDet[2][0] = Traject(&Det[2][0][0],&Det[2][1][0],NhitX[2][0][0][0],NhitY[2][1][0][0],Vert2);
          cx[2][0] = AngleDet[2][0].X();
          cy[2][0] = AngleDet[2][0].Y();
          cz[2][0] = AngleDet[2][0].Z();
        }
      } /* if(ReIN.Vertex&&!ReIN.DetectorTune) */
      if(!ReIN->Vertex&&!ReIN->DetectorTune)
      {
      // Typical case: no vertices:
        for(int it=0;it<Ntelescopes;it++)
        {
          if(mp[it]==1)
          {
            Vert1.SetXYZ(trackD->xbt,trackD->ybt,trackD->zbt);
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
          trajectory = geom->InitTrack(trackD->xbt,trackD->ybt,trackD->zbt,
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
              if(!strcmp(OutputChannelTemp,"gas")) deposit[0][0][0][1] = range*UpMat->TarPress*TempNorm/UpMat->TarTemp;
              if(!strcmp(OutputChannelTemp,"foilex")) deposit[0][0][0][2] = range;
              if(!strcmp(OutputChannelTemp,"foilen")) deposit[0][0][0][3] = range;
              if(!strcmp(OutputChannelTemp,"TargetCell")) deposit[0][0][0][4] = range;
              if(!strcmp(OutputChannelTemp,"HscrFoil")) deposit[0][0][0][5] = range;
            }
            trajectory = geom->Step();
          } /* while(!geom->IsOutside) */
          for(int ip=0;ip<NofDetPart;ip++)
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
            trajectory = geom->InitTrack(trackD->xbt,trackD->ybt,trackD->zbt,
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
                if(!strcmp(OutputChannelTemp,"gas")&&Tb>0.1) Tb = EiEo(EjMat[ip]->ej_target,Tb,range*UpMat->TarPress*TempNorm/UpMat->TarTemp);
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
          for(int ip=0;ip<NofDetPart;ip++)
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
          } /* for(ip==0;ip<NofDetPart;ip++) */
        } /* if(mp[it]==1&&RawD.mbeam&&RawD.mtrack) */
      } /* for(it==0;it<Ntelescopes) */
  InLabFrame();
  InReactionCM();
  InProjectileFrame();
}
//----------------------------------------------------------------------------
void ERHe8Analyzer::InLabFrame(){
  //cout << "In Laboratory frame" << endl;
  FairRun* run = FairRun::Instance();
  ERHe8EventHeader* header = (ERHe8EventHeader*)run->GetEventHeader();

  tel->dep11 = DepoX[0][0][0][0];tel->dep12 = DepoX[0][1][0][0];     
  tel->dep21 = DepoX[1][0][0][0];tel->dep22 = DepoX[1][1][0][0];tel->dep23 = DepoX[1][2][0][0];
  
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
  FairRun* run = FairRun::Instance();
  ERHe8EventHeader* header = (ERHe8EventHeader*)run->GetEventHeader();
  for(int it=0;it<Ntelescopes;it++)
    {
      if(mp[it]==1&&header->mbeam&&header->mtrack)
      {
        for(int ip=0;ip<NofDetPart;ip++)
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
  FairRun* run = FairRun::Instance();
  ERHe8EventHeader* header = (ERHe8EventHeader*)run->GetEventHeader();
  for(int it=0;it<Ntelescopes;it++)
  {
    if(mp[it]==1&&header->mbeam&&header->mtrack)
    {
      for(int ip=0;ip<NofDetPart;ip++)
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
      for(int ip=0;ip<NofDetPart;ip++)
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
            for(int ip=0;ip<NofDetPart;ip++)
            {
              for(int ipx=0;ipx<NofDetPart;ipx++)
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
          for(int ip=0;ip<NofDetPart;ip++)
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
            for(int ip=0;ip<NofDetPart;ip++)
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
void ERHe8Analyzer::ReadInputData()
{
  char Zeros[32];
  ReIN->Simulation = false;
  ReIN->Vertex = false;
  ReIN->DetectorTune = false;
  ReIN->TOFis = false;
  ReIN->TRACKINGis = false;
  ReIN->TrackCheck = false;
  ReIN->WriteRawData = false;
  ReIN->WriteCalData = false;
  ReIN->WriteTofData = false;
  ReIN->WriteTrackData = false;
  ReIN->WriteTelData = false;
  ReIN->WritePhysData = false;
  ReIN->WritePlayData = false;
  ReIN->WriteRawTrack = false;
  /******************** Readout ReactionInput.dat ************************/
  printf("************************************************************\n");
  TString filePath = gSystem->Getenv("VMCWORKDIR") + TString("/input/ReactionInput.dat");
  FILE *F1 = fopen(filePath.Data(),"r");
  if(F1==NULL) {printf("Main: File ReactionInput.dat was not found\n");}
  else
  {
    fscanf(F1,"%s\n",ReIN->ReactionName);
    fscanf(F1,"%s %s\n",ReIN->AboutBeam,ReIN->EnergyUn);
    fscanf(F1,"%s %s\n",ReIN->AboutSlit,ReIN->SlitUn);
    fscanf(F1,"%s %s\n",Zeros,ReIN->Mechanism);
    fscanf(F1,"%s %s\n",Zeros,ReIN->Fname);
    fscanf(F1,"%s %s\n",Zeros,Zeros);
    if(!strcmp(Zeros,"yes")) {ReIN->Simulation = true;}
    fscanf(F1,"%s %s\n",Zeros,Zeros);
    if(!strcmp(Zeros,"yes")) {ReIN->Vertex = true;}
    fscanf(F1,"%s %s\n",Zeros,Zeros);
    if(!strcmp(Zeros,"yes")) {ReIN->DetectorTune = true;}
    fscanf(F1,"%s %s\n",Zeros,Zeros);
    if(!strcmp(Zeros,"yes")) {ReIN->TOFis = true;}
    fscanf(F1,"%s %s\n",Zeros,Zeros);
    if(!strcmp(Zeros,"yes")) {ReIN->TRACKINGis = true;}
    fscanf(F1,"%s %s\n",Zeros,Zeros);
    if(!strcmp(Zeros,"yes")) {ReIN->TrackCheck = true;}
    fscanf(F1,"%s %s\n",Zeros,Zeros);
    if(!strcmp(Zeros,"yes")) {ReIN->WriteRawData = true;}
    fscanf(F1,"%s %s\n",Zeros,Zeros);
    if(!strcmp(Zeros,"yes")) {ReIN->WriteCalData = true;}
    fscanf(F1,"%s %s\n",Zeros,Zeros);
    if(!strcmp(Zeros,"yes")) {ReIN->WriteTofData = true;}
    fscanf(F1,"%s %s\n",Zeros,Zeros);
    if(!strcmp(Zeros,"yes")) {ReIN->WriteTrackData = true;}
    fscanf(F1,"%s %s\n",Zeros,Zeros);
    if(!strcmp(Zeros,"yes")) {ReIN->WriteTelData = true;}
    fscanf(F1,"%s %s\n",Zeros,Zeros);
    if(!strcmp(Zeros,"yes")) {ReIN->WritePhysData = true;}
    fscanf(F1,"%s %s\n",Zeros,Zeros);
    if(!strcmp(Zeros,"yes")) {ReIN->WritePlayData = true;}
    fscanf(F1,"%s %s\n",Zeros,Zeros);
    if(!strcmp(Zeros,"yes")) {ReIN->WriteRawTrack = true;}

    for(int m=0;m<8;m++)
    {
      fscanf(F1,"%s %i\n",Zeros,&ReIN->ifill[m]);
    }
    printf("Main: File ReactionInput.dat has been read\n");
  }
  /******************** Readout Simulation.dat ************************/
  if(ReIN->Simulation)
  {
    printf("************************************************************\n");
    filePath = gSystem->Getenv("VMCWORKDIR") + TString("/input/Simulation.dat");
    F1 = fopen(filePath.Data(),"r");
    if(F1==NULL) {printf("Main: File Simulation.dat was not found\n");}
    else
    {
      fscanf(F1,"%s %i\n",Zeros,&SimDat->NEventsToBePlayed);
      fscanf(F1,"%s\n",Zeros);
      fscanf(F1,"%s %lf\n",Zeros,&SimDat->echeck);
      fscanf(F1,"%s %s\n",Zeros,SimDat->WaveFunction);
      fscanf(F1,"%s %s\n",Zeros,SimDat->FreeScatt);
      fscanf(F1,"%s %lf\n",Zeros,&SimDat->thtpmin);
      fscanf(F1,"%s %lf\n",Zeros,&SimDat->thtpmax);
      fscanf(F1,"%s %lf\n",Zeros,&SimDat->ResE0);
      fscanf(F1,"%s %lf\n",Zeros,&SimDat->ResS0);
      fscanf(F1,"%s %lf\n",Zeros,&SimDat->MomP0);
      fscanf(F1,"%s %lf\n",Zeros,&SimDat->MomS0);

      printf("Main: File Simulation.dat has been read\n");
      printf("%i events to be played\n",SimDat->NEventsToBePlayed);
      printf("Mechanism is %s, WaveFunction %s\n",ReIN->Mechanism,SimDat->WaveFunction);
    }
    if(cos(SimDat->thtpmin*rad)>cos(SimDat->thtpmax*rad)) {SimDat->AngleMax=SimDat->thtpmin;SimDat->AngleMin=SimDat->thtpmax;}
    else {SimDat->AngleMax=SimDat->thtpmax;SimDat->AngleMin=SimDat->thtpmin;}
  }   
  fclose(F1);
  /*********************** Readout MWPC parameters:***********************/
  if(ReIN->TRACKINGis)
  {
    filePath = gSystem->Getenv("VMCWORKDIR") + TString("/input/track.dat");
    F1 = fopen(filePath.Data(),"r");
    if(F1==NULL) {printf("Main: File track.dat was not found\n");}
    else  
    { 
      fscanf(F1,"%s %s %lf\n",Zeros,UpMat->MWwinMatter,&UpMat->MWwinThick);
      fscanf(F1,"%s %s %lf\n",Zeros,UpMat->MWgasMatter,&UpMat->MWgasThick);
      fscanf(F1,"%s %s %lf\n",Zeros,UpMat->MWcathMatter,&UpMat->MWcathThick);
      fscanf(F1,"%s %i\n",Zeros,&UpMat->MWNcathodes);
      fscanf(F1,"%s %lf\n",Zeros,&UpMat->MWXYdist);
      fscanf(F1,"%s %lf\n",Zeros,&UpMat->MWstep);
      fscanf(F1,"%s %i\n",Zeros,&UpMat->MWNwires);
      fscanf(F1,"%s\n",Zeros);
      fscanf(F1,"%s %lf\n",Zeros,&UpMat->MWfarDist);
      fscanf(F1,"%s %lf %lf\n",Zeros,&UpMat->MWfarXshift,&UpMat->MWfarYshift);
      fscanf(F1,"%s %i %i\n",Zeros,&UpMat->MWfarXNum,&UpMat->MWfarYNum);
      fscanf(F1,"%s\n",Zeros);
      fscanf(F1,"%s %lf\n",Zeros,&UpMat->MWclosDist);
      fscanf(F1,"%s %lf %lf\n",Zeros,&UpMat->MWclosXshift,&UpMat->MWclosYshift);
      fscanf(F1,"%s %i %i\n",Zeros,&UpMat->MWclosXNum,&UpMat->MWclosYNum);
      printf("Main: File track.dat has been read\n");
    }
  fclose(F1);
  }
  UpMat->MWcathThick *= UpMat->MWNcathodes;
  /*********************** Readout TOF parameters:************************/
  if(ReIN->TOFis)
  {
  filePath = gSystem->Getenv("VMCWORKDIR") + TString("/input/tof.dat");
  FILE *F1 = fopen(filePath.Data(),"r");
  if(F1==NULL) {printf("Main: File tof.dat was not found\n");}
  else
  { 
    fscanf(F1,"%s %lf %lf\n",UpMat->PlasticMatter1,&UpMat->PlasticAngle1,&UpMat->PlasticThick1);
    fscanf(F1,"%s %lf %lf\n",UpMat->PlasticMatter2,&UpMat->PlasticAngle2,&UpMat->PlasticThick2);
    fscanf(F1,"%s %lf\n",Zeros,&UpMat->PlasticDist);
    fscanf(F1,"%s %lf\n",Zeros,&UpMat->TofRes);
    fscanf(F1,"%s %lf %lf\n",Zeros,&UpMat->tF3l_rng,&UpMat->tF3r_rng);
    fscanf(F1,"%s %lf %lf\n",Zeros,&UpMat->tF4l_rng,&UpMat->tF4r_rng);
    fscanf(F1,"%s %lf %lf\n",Zeros,&UpMat->tF3_dlt,&UpMat->tF4_dlt);
    printf("Main: File tof.dat has been read\n");
  }
  fclose(F1);
  UpMat->PlasticThick1/=cos(UpMat->PlasticAngle1*rad);
  UpMat->PlasticThick2/=cos(UpMat->PlasticAngle2*rad);
  }
  /********************* Readout Target parameters:***********************/
  filePath = gSystem->Getenv("VMCWORKDIR") + TString("/input/target.dat");
  F1 = fopen(filePath.Data(),"r");
  if(F1==NULL) printf("Main: File target.dat was not found\n");
  else
  { 
    fscanf(F1,"%s %s\n",Zeros,UpMat->TarShape);
    fscanf(F1,"%s %lf %lf %lf\n",Zeros,&UpMat->TarXshift,&UpMat->TarYshift,&UpMat->TarZshift);
    fscanf(F1,"%s %s %lf\n",Zeros,UpMat->TarFoilMatter,&UpMat->FoilThick);
    fscanf(F1,"%s %lf\n",Zeros,&UpMat->TarRadius);
    fscanf(F1,"%s %lf\n",Zeros,&UpMat->TarHeight);
    fscanf(F1,"%s %lf\n",Zeros,&UpMat->TarWallThick);
    fscanf(F1,"%s %lf\n",Zeros,&UpMat->TarEntrHoleRad);
    fscanf(F1,"%s %lf\n",Zeros,&UpMat->MeniskSize);
    fscanf(F1,"%s %lf\n",Zeros,&UpMat->TarTemp);
    fscanf(F1,"%s %lf\n",Zeros,&UpMat->TarPress);
    fscanf(F1,"%s %lf\n",Zeros,&UpMat->TarAngle);
    fscanf(F1,"%s %s\n",Zeros,UpMat->HeatScreenAns);
    fscanf(F1,"%s %s %lf\n",Zeros,UpMat->HeatScreenMatter,&UpMat->HeatScreenThick);
    fscanf(F1,"%s %lf\n",Zeros,&UpMat->InHscrRad);
    fscanf(F1,"%s %lf\n",Zeros,&UpMat->HscrWallWidth);
    fscanf(F1,"%s %lf\n",Zeros,&UpMat->HscrHeight);
    fscanf(F1,"%s %lf\n",Zeros,&UpMat->EntrHRad);
    fscanf(F1,"%s %lf\n",Zeros,&UpMat->ExHX);
    fscanf(F1,"%s %lf\n",Zeros,&UpMat->ExHY);
    printf("Main: File target.dat has been read\n");
  }
  fclose(F1);
  if(UpMat->MeniskSize==0.) UpMat->MeniskSize=0.00001;
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
  if(!strcmp(UpMat->TarShape,"SemiCylindrical"))
  {
    UpMat->TarThick = UpMat->TarRadius;
    TGeoVolume *ttargetgas=geom->MakeTubs("ttargetgas",Si,0,UpMat->TarRadius,UpMat->TarHeight/2.,0.,180.);

    TGeoVolume *TargetUp=geom->MakeTubs("TargetUp",Si,0,UpMat->TarRadius,UpMat->TarWallThick/2.,0.,180.);
    TGeoTranslation *TargetUpShift=new TGeoTranslation("TargetUpShift",0.,-UpMat->TarRadius/2.,-(UpMat->TarHeight+UpMat->TarWallThick)/2.);
    TargetUpShift->RegisterYourself();

    TGeoVolume *TargetDown=geom->MakeTubs("TargetDown",Si,0,UpMat->TarRadius,UpMat->TarWallThick/2,0.,180.);
    TGeoTranslation *TargetDownShift=new TGeoTranslation("TargetDownShift",0.,-UpMat->TarRadius/2.,(UpMat->TarHeight+UpMat->TarWallThick)/2.);
    TargetDownShift->RegisterYourself();

    TGeoVolume *TargetBack=geom->MakeBox("TargetBack",Si,UpMat->TarRadius,UpMat->TarWallThick+(UpMat->TarHeight/2.),UpMat->TarWallThick/2.);
    TGeoTranslation *TargetBackShift=new TGeoTranslation("TargetBackShift",0.,0.,-UpMat->TarWallThick/2.-UpMat->TarRadius/2.);
    TargetBackShift->RegisterYourself();

    TGeoVolume *TargetEntrHole=geom->MakeTube("TargetEntrHole",Si,0.,UpMat->TarEntrHoleRad,(UpMat->TarWallThick+UpMat->FoilThick)/2.);
    TGeoCompositeShape *target=new TGeoCompositeShape("target","(TargetUp:TargetUpShift+TargetDown:TargetDownShift):rot0+(TargetBack:TargetBackShift-TargetEntrHole:TargetBackShift)");
    TGeoVolume *ttarcell=new TGeoVolume("ttarcell",target,Si);

    TGeoVolume *tExitFoil=geom->MakeTubs("tExitFoil",Si,UpMat->TarRadius,UpMat->TarRadius+UpMat->FoilThick,UpMat->TarHeight/2.,0.,180.);
    TGeoTranslation *ExitFoilShift=new TGeoTranslation("ExitFoilShift",0.,0.,-UpMat->TarRadius/2.);
    ExitFoilShift->RegisterYourself();

    TGeoVolume *tEntrFoil=geom->MakeTube("tEntrFoil",Si,0.,UpMat->TarEntrHoleRad,UpMat->FoilThick/2.);
    TGeoTranslation *EntrFoilShift=new TGeoTranslation(0,0.,-UpMat->FoilThick/2.-UpMat->TarRadius/2.);

    TGeoCombiTrans *posexf=new TGeoCombiTrans();
    posexf->RegisterYourself();
    *posexf = *ExitFoilShift*(*rot0);

    top->AddNode(tEntrFoil,1,EntrFoilShift);
    top->AddNode(tExitFoil,1,posexf);
    top->AddNode(ttarcell,1);
    top->AddNode(ttargetgas,1,posexf);
  }
  else if(!strcmp(UpMat->TarShape,"Cylindrical"))
  {
    TGeoRotation *rottar=new TGeoRotation();
    rottar->SetAngles(90.,UpMat->TarAngle,0.);

    UpMat->TarThick = UpMat->TarHeight;

  // making the cylindrical part of the gaseous body "ttargetgas", that afterward must be shifted backward by TargetShift

    TGeoVolume *ttargetgas=geom->MakeTube("ttargetgas",Si,0,UpMat->TarEntrHoleRad,UpMat->TarThick/2.);
    TGeoTranslation *TargetShift=new TGeoTranslation("TargetShift",0.,0.,-(UpMat->MeniskSize+UpMat->TarThick)/2.);
    TargetShift->RegisterYourself();

  // making the menisk entrance part of the gaseous body "tGasMenEntr", that afterward must be shifted backward by MeniskShift

    TGeoVolume *tGasMenEntr=geom->MakeParaboloid("tGasMenEntr",Si,0.,UpMat->TarEntrHoleRad,UpMat->MeniskSize/2.);
    TGeoTranslation *MeniskShift=new TGeoTranslation("MeniskShift",0.,0.,-(UpMat->MeniskSize+UpMat->TarThick));
    MeniskShift->RegisterYourself();

  // making the menisk exit part of the gaseous body "tGasMenExit", that afterward must be revolved by 180 deg by rotExitMen

    TGeoVolume *tGasMenExit=geom->MakeParaboloid("tGasMenExit",Si,0.,UpMat->TarEntrHoleRad,UpMat->MeniskSize/2.);
    TGeoRotation *rotExitMen=new TGeoRotation("rotExitMen",0.,180.,0.);
    rotExitMen->RegisterYourself();

  // Unifying three previous forms into one whole gaseous body

    TGeoCompositeShape *tgg=new TGeoCompositeShape("tgg","ttargetgas:TargetShift+tGasMenEntr:MeniskShift+tGasMenExit:rotExitMen");
    TGeoVolume *tgas=new TGeoVolume("tgas",tgg,Si);

  // The whole gaseous body must be shifted forward to the origin of the frame by GasShift and then rotated around the Y-axis if needed

    TGeoTranslation *GasShift=new TGeoTranslation("GasShift",UpMat->TarYshift,-UpMat->TarXshift,(UpMat->MeniskSize+UpMat->TarHeight)/2.+UpMat->TarZshift);
    GasShift->RegisterYourself();
    TGeoTranslation *Zshift=new TGeoTranslation("Zshift",UpMat->TarYshift,-UpMat->TarXshift,UpMat->TarZshift);
    Zshift->RegisterYourself();
    TGeoCombiTrans *postfex=new TGeoCombiTrans();
    postfex->RegisterYourself();
    *postfex = *rottar*(*GasShift);
    TGeoCombiTrans *posttar=new TGeoCombiTrans();
    posttar->RegisterYourself();
    *posttar = *rottar*(*Zshift);

  // To construct the foil shell surrounding the gaseous body, we repeat the previous procedure twice, for the inner and outer shapes of the foil, and subtruct one from another

    TGeoVolume *tTargetCell=geom->MakeTube("tTargetCell",Si,UpMat->TarEntrHoleRad,UpMat->TarRadius,UpMat->TarThick/2.);

    TGeoVolume *tEntrFoilInt=geom->MakeParaboloid("tEntrFoilInt",Si,0.,UpMat->TarEntrHoleRad,UpMat->MeniskSize/2.);
    TGeoVolume *tEntrFoilExt=geom->MakeParaboloid("tEntrFoilExt",Si,0.,UpMat->TarEntrHoleRad+3.*UpMat->FoilThick,UpMat->MeniskSize/2.+UpMat->FoilThick);
    TGeoVolume *tEntrFoilHole=geom->MakeTube("tEntrFoilHole",Si,0.,UpMat->TarEntrHoleRad+3.*UpMat->FoilThick,1.01*UpMat->FoilThick/2.);
    TGeoTranslation *HoleShift=new TGeoTranslation("HoleShift",0.,0.,UpMat->MeniskSize/2.+UpMat->FoilThick/2.);
    HoleShift->RegisterYourself();

    TGeoVolume *tExitFoilInt=geom->MakeParaboloid("tExitFoilInt",Si,0.,UpMat->TarEntrHoleRad,UpMat->MeniskSize/2.);
    TGeoVolume *tExitFoilExt=geom->MakeParaboloid("tExitFoilExt",Si,0.,UpMat->TarEntrHoleRad+3.*UpMat->FoilThick,UpMat->MeniskSize/2.+UpMat->FoilThick);
    TGeoVolume *tExitFoilHole=geom->MakeTube("tExitFoilHole",Si,0.,UpMat->TarEntrHoleRad+3.*UpMat->FoilThick,1.01*UpMat->FoilThick/2.);

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
  if(!strcmp(UpMat->HeatScreenAns,"yes"))
  {
    TGeoVolume *Hscr=geom->MakeTube("Hscr",Si,UpMat->InHscrRad,UpMat->InHscrRad+UpMat->HscrWallWidth,UpMat->HscrHeight/2.);

    TGeoVolume *HscrEntHole=geom->MakeTube("HscrEntHole",Si,0.,UpMat->EntrHRad,UpMat->InHscrRad);
    TGeoTranslation *HEntHolShift=new TGeoTranslation("HEntHolShift",0.,0.,-UpMat->InHscrRad);
    HEntHolShift->RegisterYourself();

    TGeoVolume *HscrExHole=geom->MakeBox("HscrExHole",Si,UpMat->ExHX/2.,UpMat->ExHY/2.,UpMat->InHscrRad);
    TGeoTranslation *HExHolShift=new TGeoTranslation("HExHolShift",0.,0.,UpMat->InHscrRad);
    HExHolShift->RegisterYourself();

    TGeoCompositeShape *HeatScr=new TGeoCompositeShape("HeatScr","Hscr:rot0-(HscrEntHole:HEntHolShift+HscrExHole:HExHolShift)");
    TGeoVolume *tHeatScreen=new TGeoVolume("tHeatScreen",HeatScr,Si);

    TGeoVolume *tHscrFoil=geom->MakeTube("tHscrFoil",Si,UpMat->InHscrRad-UpMat->HeatScreenThick,UpMat->InHscrRad,UpMat->HscrHeight/2.);

    tHeatScreen->SetLineColor(1);
    tHscrFoil->SetLineColor(2);
    top->AddNode(tHeatScreen,1);
    top->AddNode(tHscrFoil,1,rot0);
  }
  geom->CloseGeometry();
  cout << "Target geometry creating finished" << endl;
}
//-----------------------------------------------------------------------------
int ERHe8Analyzer::HowMuchParticles(char* str){
  char xname[32];
  char* ptr;
  strcpy(xname,str);
  int N = 0;
  do
  {
    ptr = strchr(xname,'+');
    if(ptr!=NULL)
    {N++;ptr++;strcpy(xname,ptr);}
  } while(ptr!=NULL);
  N++;
  return N;
}
//-----------------------------------------------------------------------------

void ERHe8Analyzer::WhatParticlesInOut(Particle* ptr,char* str,int N){
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
double Particle::ReturnMass(char* NON,char* WayMass)
{
  double MassExcess;
  double LifeTime;
  double massa;
  int z;
  int a;
  char Name[5];
  char TimeUnit [6];
  FILE *F1 = fopen(WayMass,"r");
  if(F1==NULL) {printf("ReturnMass: File %s is not found\n",WayMass);}
  else 
  {
  while(!feof(F1))
  {
    fscanf(F1,"%s %d %d %lf %s %lf\n",Name,&z,&a,&MassExcess,TimeUnit,&LifeTime);
    if (!strcmp(NON,Name))
    {massa=AMU*(double)a+MassExcess;break;}
  }
  fclose(F1);
  }
  return massa;
}
//-----------------------------------------------------------------------------
void Particle::ParticleID(char* name, char* path){
  char nucl[5];
  double MassExcess;
  int z;
  int a;
  char TimeUnit[6];
  double LifeTime;

  FILE *F1 = fopen(path,"r");
  if(F1==NULL) {printf("IdParticle: File %s is not found\n",path);}
  else 
  {
    while(!feof(F1))
    {
      fscanf(F1,"%s %d %d %lf %s %lf\n",nucl,&z,&a,&MassExcess,
        TimeUnit,&LifeTime);
      if (!strcmp(name,nucl))
      {
        Mass=AMU*(double)a+MassExcess;
        NameOfNucleus = new char [strlen(nucl)+1];
        strcpy(NameOfNucleus,nucl);
        AtNumber = z;
        AtMass = a;
        break;
      }
    }
  }
  fclose(F1);
  return;
}
//-----------------------------------------------------------------------------
void ERHe8Analyzer::ReadDeDx(){
  EjMat = new DownstreamMatter*[NofDetPart];
  for(int i =0; i<NofDetPart;i++)
    EjMat[i] = new DownstreamMatter();
  char Matter[128];
  TString filePath = gSystem->Getenv("VMCWORKDIR") + TString("/input/eloss/");
  for(int ip=0;ip<NofDetPart;ip++)
  {   
    strcpy(Matter,filePath.Data());
    strcat(Matter,ejectile[0][ip][0].NameOfNucleus);
    strcat(Matter,"_");
    strcat(Matter,"si");
    ReadRint(Matter,EjMat[ip]->ej_si);
  }
  for(int ip=0;ip<NofDetPart;ip++)
  {   
    strcpy(Matter,filePath.Data());
    strcat(Matter,ejectile[0][ip][0].NameOfNucleus);
    strcat(Matter,"_");
    strcat(Matter,"csi");
    ReadRint(Matter,EjMat[ip]->ej_csi);
  }
  for(int ip=0;ip<NofDetPart;ip++)
  {   
    strcpy(Matter,filePath.Data());
    strcat(Matter,ejectile[0][ip][0].NameOfNucleus);
    strcat(Matter,"_");
    strcat(Matter,UpMat->TarFoilMatter);
    ReadRint(Matter,EjMat[ip]->ej_TARwin);
  }
  for(int ip=0;ip<NofDetPart;ip++)
  {   
    strcpy(Matter,filePath.Data());
    strcat(Matter,ejectile[0][ip][0].NameOfNucleus);
    strcat(Matter,"_");
    strcat(Matter,target->NameOfNucleus);
    ReadRint(Matter,EjMat[ip]->ej_target);
  }

  /********************* For the TOF plastic *****************************/
  strcpy(Matter,filePath.Data());
  strcat(Matter,projectile->NameOfNucleus);
  strcat(Matter,"_");
  strcat(Matter,UpMat->PlasticMatter1);
  ReadRint(Matter,UpMat->beam_TOF);
  /********************* For the MWPC windows ****************************/
  strcpy(Matter,filePath.Data());
  strcat(Matter,projectile->NameOfNucleus);
  strcat(Matter,"_");
  strcat(Matter,UpMat->MWwinMatter);
  ReadRint(Matter,UpMat->beam_MWwin);
  /************************ For the MWPC gas *****************************/
  strcpy(Matter,filePath.Data());
  strcat(Matter,projectile->NameOfNucleus);
  strcat(Matter,"_");
  strcat(Matter,UpMat->MWgasMatter);
  ReadRint(Matter,UpMat->beam_MWgas);
  /********************* For the MWPC cathodes ***************************/
  strcpy(Matter,filePath.Data());
  strcat(Matter,projectile->NameOfNucleus);
  strcat(Matter,"_");
  strcat(Matter,UpMat->MWcathMatter);
  ReadRint(Matter,UpMat->beam_MWcathod);
  /********************* For the heat screen **************************/
  //  strcpy(Matter,"../../../include/eloss/");
  //  strcat(Matter,projectile->NameOfNucleus);
  //  strcat(Matter,"_");
  //  strcat(Matter,UpMat->TarFoilMatter);
  //  ReadRint(Matter,UpMat->beam_heatscreen);
  /********************* For the target windows **************************/
  strcpy(Matter,filePath.Data());
  strcat(Matter,projectile->NameOfNucleus);
  strcat(Matter,"_");
  strcat(Matter,UpMat->TarFoilMatter);
  ReadRint(Matter,UpMat->beam_TARwin);
  /********************* For the target material *************************/
  strcpy(Matter,filePath.Data());
  strcat(Matter,projectile->NameOfNucleus);
  strcat(Matter,"_");
  strcat(Matter,target->NameOfNucleus);
  ReadRint(Matter,UpMat->beam_target);
  /*************************************************************************/
}
//-----------------------------------------------------------------------------
int ERHe8Analyzer::ReadRint(char* Fname,double Ranges[][105]){
  char DummyA[256];
  char termA[]="---";
  char unR[10],un3[10],un4[10],dee[10],b[10],cc[10];
  char unE[10]="begin";
  double Energy,Rng,e,r,den;
  double x[4],y[4],c[4];
  int i;
  double a[4];
  
  FILE *F2 = fopen(Fname,"r");
  if(F2==NULL) {printf("ReadRint: File %s has not been read\n",Fname);
  return 1;}
  else
  {
  printf("ReadRint: File %s has been read\n",Fname);
  while (fgets(DummyA,256,F2)) {if(strstr(DummyA,termA)) break;}
  i=0;
  while (strcmp(unE,"GeV"))
  {
  //printf("%i\n",i);
  fscanf(F2,"%lf %s %s %lf %lf %s %s %s %s %s\n",&e,unE,dee,
  &den,&r,unR,b,un3,cc,un4);
  if(!strcmp(unE,"keV")) {Energy=e/1000.0;}
  else if(!strcmp(unE,"MeV")) {Energy=e;}
  else if(!strcmp(unE,"GeV")) {Energy=e*1000.0;}
  else {printf("Error of reading in ReadRint a: %s %s\n",unE,unR);}

  if(!strcmp(unR,"A")) {Rng=r*1.0E-8;}
  else if(!strcmp(unR,"um")) {Rng=r/10000.0;}
  else if(!strcmp(unR,"mm")) {Rng=r/10.0;}
  else if(!strcmp(unR,"cm")) {Rng=r;}
  else if(!strcmp(unR,"m")) {Rng=r*100.0;}
  else if(!strcmp(unR,"km")) {Rng=r*100000.0;}
  else {printf("Error of reading in ReadRint b: %s %s\n",unE,unR);}
  
  Ranges[0][i]=Energy;
  Ranges[1][i]=Rng;
  i++;
  }
  fclose(F2);
  }
  for (i=0;i<4;++i)
  {
    x[i]=Ranges[0][i];
    y[i]=Ranges[1][i];
  }
  if(intrp4(x,y,c))
  {printf("1 Interpolation error 1 intrp4=%i\n",intrp4(x,y,c));
  return 1;}

  Ranges[2][0]=c[0];
  Ranges[3][0]=c[1];
  Ranges[4][0]=c[2];
  Ranges[5][0]=c[3];
      
  for (i=0;i<4;++i)
  {
    x[i]=Ranges[1][i];
    y[i]=Ranges[0][i]; 
  }

  if(intrp4(x,y,c))
  {printf("2 Interpolation error 2 intrp4=%i\n",intrp4(x,y,c));return 1;}

  Ranges[6][0]= c[0];
  Ranges[7][0]= c[1];
  Ranges[8][0]= c[2];
  Ranges[9][0]= c[3];

  for(i=1;i<103;++i)
  {
    x[0] = Ranges[0][i-1];
    x[1] = Ranges[0][i];
    x[2] = Ranges[0][i+1];
    x[3] = Ranges[0][i+2];
    y[0] = Ranges[1][i-1];
    y[1] = Ranges[1][i];
    y[2] = Ranges[1][i+1];
    y[3] = Ranges[1][i+2];

    if(intrp4(x,y,c))
    {printf("3 Interpolation error 3 intrp4=%i\n",intrp4(x,y,c));return 1;}
    Ranges[2][i] = c[0];
    Ranges[3][i] = c[1];
    Ranges[4][i] = c[2];
    Ranges[5][i] = c[3];
  }
  Ranges[2][i] = c[0];
  Ranges[3][i] = c[1];
  Ranges[4][i] = c[2];
  Ranges[5][i] = c[3];

  Ranges[2][i+1] = c[0];
  Ranges[3][i+1] = c[1];
  Ranges[4][i+1] = c[2];
  Ranges[5][i+1] = c[3];
  
  for(i=1;i<103;++i)
  {
    y[0] = Ranges[0][i-1];
    y[1] = Ranges[0][i];
    y[2] = Ranges[0][i+1];
    y[3] = Ranges[0][i+2];

    x[0] = Ranges[1][i-1];
    x[1] = Ranges[1][i];
    x[2] = Ranges[1][i+1];
    x[3] = Ranges[1][i+2];

    if(intrp4(x,y,c))
    {printf("4 Interpolation error 4 intrp4=%i\n",intrp4(x,y,c));return 1;}
    Ranges[6][i] = c[0];
    Ranges[7][i] = c[1];
    Ranges[8][i] = c[2];
    Ranges[9][i] = c[3];
  }

  Ranges[6][i] = c[0];
  Ranges[7][i] = c[1];
  Ranges[8][i] = c[2];
  Ranges[9][i] = c[3];

  Ranges[6][i+1] = c[0];
  Ranges[7][i+1] = c[1];
  Ranges[8][i+1] = c[2];
  Ranges[9][i+1] = c[3];

  return 0;
}
//-----------------------------------------------------------------------------
int ERHe8Analyzer::intrp4(double* x,double* y, double* c){
  //______________________________________________________________________
  //  returns c0,c1,c2,c3 coeff. of y= c0 + c1*x + c2*x^2 + c3*x^3 function 
  //     passing through 4 points:
  //     x1,y1; x2,y2; x3,y3; x4,y4
  //______________________________________________________________________|
      double d0,d1,d2,d3;
      double x12,x13,x22,x23,x32,x33,x42,x43;
      int rp4;
  x12 = x[0]*x[0];
  x13 = x12*x[0];
  x22 = x[1]*x[1];
  x23 = x22*x[1];
  x32 = x[2]*x[2];
  x33 = x32*x[2];
  x42 = x[3]*x[3];
  x43 = x42*x[3];

  d3=(x13*x22*x[2]-x12*x23*x[2]-x13*x[1]*x32+x[0]*x23*x32+x12*x[1]*x33-
  x[0]*x22*x33-x13*x22*x[3]+x12*x23*x[3]+x13*x32*x[3]-x23*x32*x[3]-
  x12*x33*x[3]+x22*x33*x[3]+x13*x[1]*x42-x[0]*x23*x42-x13*x[2]*x42+
  x23*x[2]*x42+x[0]*x33*x42-x[1]*x33*x42-x12*x[1]*x43+x[0]*x22*x43+
  x12*x[2]*x43-x22*x[2]*x43-x[0]*x32*x43+x[1]*x32*x43);

  c[3]=(x22*x[2]*y[0]-x[1]*x32*y[0]-x22*x[3]*y[0]+
  x32*x[3]*y[0]+x[1]*x42*y[0]-x[2]*x42*y[0]-
  x12*x[2]*y[1]+x[0]*x32*y[1]+x12*x[3]*y[1]- 
  x32*x[3]*y[1]-x[0]*x42*y[1]+x[2]*x42*y[1]+
  x12*x[1]*y[2]-x[0]*x22*y[2]-x12*x[3]*y[2]+ 
  x22*x[3]*y[2]+x[0]*x42*y[2]-x[1]*x42*y[2]-
  x12*x[1]*y[3]+x[0]*x22*y[3]+x12*x[2]*y[3]- 
  x22*x[2]*y[3]-x[0]*x32*y[3]+x[1]*x32*y[3])/d3;

  d2=(x13*x22*x[2]-x12*x23*x[2]-x13*x[1]*x32+x[0]*x23*x32+x12*x[1]*x33-
  x[0]*x22*x33-x13*x22*x[3]+x12*x23*x[3]+x13*x32*x[3]-x23*x32*x[3]-
  x12*x33*x[3]+x22*x33*x[3]+x13*x[1]*x42-x[0]*x23*x42-x13*x[2]*x42+
  x23*x[2]*x42+x[0]*x33*x42-x[1]*x33*x42-x12*x[1]*x43+x[0]*x22*x43+
  x12*x[2]*x43-x22*x[2]*x43-x[0]*x32*x43+x[1]*x32*x43);

  c[2]=(-(x23*x[2]*y[0])+x[1]*x33*y[0]+x23*x[3]*y[0]-x33*x[3]*y[0]-x[1]*x43*y[0]+
  x[2]*x43*y[0]+x13*x[2]*y[1]-x[0]*x33*y[1]- 
  x13*x[3]*y[1]+x33*x[3]*y[1]+x[0]*x43*y[1]- 
  x[2]*x43*y[1]-x13*x[1]*y[2]+x[0]*x23*y[2]+ 
  x13*x[3]*y[2]-x23*x[3]*y[2]-x[0]*x43*y[2]+
  x[1]*x43*y[2]+x13*x[1]*y[3]-x[0]*x23*y[3]- 
  x13*x[2]*y[3]+x23*x[2]*y[3]+x[0]*x33*y[3]-
  x[1]*x33*y[3])/d2;

  d1=(x13*x22*x[2]-x12*x23*x[2]-x13*x[1]*x32+x[0]*x23*x32+x12*x[1]*x33-
  x[0]*x22*x33-x13*x22*x[3]+x12*x23*x[3]+x13*x32*x[3]-x23*x32*x[3]-
  x12*x33*x[3]+x22*x33*x[3]+x13*x[1]*x42-x[0]*x23*x42-x13*x[2]*x42+
  x23*x[2]*x42+x[0]*x33*x42-x[1]*x33*x42-x12*x[1]*x43+x[0]*x22*x43+
  x12*x[2]*x43-x22*x[2]*x43-x[0]*x32*x43+x[1]*x32*x43);

  c[1]=(x23*x32*y[0]-x22*x33*y[0]-x23*x42*y[0]+x33*x42*y[0]+x22*x43*y[0]-
  x32*x43*y[0]-x13*x32*y[1]+x12*x33*y[1]+x13*x42*y[1]-x33*x42*y[1]-
  x12*x43*y[1]+x32*x43*y[1]+x13*x22*y[2]-x12*x23*y[2]-x13*x42*y[2]+
  x23*x42*y[2]+x12*x43*y[2]-x22*x43*y[2]-x13*x22*y[3]+x12*x23*y[3]+
  x13*x32*y[3]-x23*x32*y[3]-x12*x33*y[3]+x22*x33*y[3])/d1;

  d0=(x13*x22*x[2]-x12*x23*x[2]-x13*x[1]*x32+x[0]*x23*x32+x12*x[1]*x33-
  x[0]*x22*x33-x13*x22*x[3]+x12*x23*x[3]+x13*x32*x[3]-x23*x32*x[3]-
  x12*x33*x[3]+x22*x33*x[3]+x13*x[1]*x42-x[0]*x23*x42-x13*x[2]*x42+
  x23*x[2]*x42+x[0]*x33*x42-x[1]*x33*x42-x12*x[1]*x43+x[0]*x22*x43+
  x12*x[2]*x43-x22*x[2]*x43-x[0]*x32*x43+x[1]*x32*x43);

  c[0]=(-(x23*x32*x[3]*y[0])+x22*x33*x[3]*y[0]+x23*x[2]*x42*y[0]-x[1]*x33*x42*y[0]-
  x22*x[2]*x43*y[0]+x[1]*x32*x43*y[0]+x13*x32*x[3]*y[1]-x12*x33*x[3]*y[1]-
  x13*x[2]*x42*y[1]+x[0]*x33*x42*y[1]+x12*x[2]*x43*y[1]-x[0]*x32*x43*y[1]-
  x13*x22*x[3]*y[2]+x12*x23*x[3]*y[2]+x13*x[1]*x42*y[2]-x[0]*x23*x42*y[2]-
  x12*x[1]*x43*y[2]+x[0]*x22*x43*y[2]+x13*x22*x[2]*y[3]-x12*x23*x[2]*y[3]-
  x13*x[1]*x32*y[3]+x[0]*x23*x32*y[3]+x12*x[1]*x33*y[3]-x[0]*x22*x33*y[3])/d0;

  if(d0*d1*d2*d3!=0.0) {rp4=0;}
  else {rp4=-1;}              
  
  return rp4;
}
//-----------------------------------------------------------------------------
void ERHe8Analyzer::DefineBeamEnergy(){
  cout << "Define beam energy" << endl;
  char RightEnUn1[]="MeV";
  char RightEnUn2[]="MeV/n";
  char RightEnUn3[]="A";
  char Dipole[] = "D2";
  char RightSpreadUn2[]="%";
  char* plett = strchr(ReIN->AboutBeam,'=');
  plett++;
  Tb = atof(plett);
  if(!strcmp(ReIN->EnergyUn,RightEnUn1)) Tb = Tb;
  else if(!strcmp(ReIN->EnergyUn,RightEnUn2)) Tb *= projectile->AtMass;
  else if(!strcmp(ReIN->EnergyUn,RightEnUn3)) Tb = Stepantsov(Dipole,projectile->AtNumber,projectile->Mass,Tb);
  else printf("Main: wrong energy unit\n");
  Tboutput = Tb;

  plett = strchr(ReIN->AboutSlit,'=');
  plett++;
  BeamSpread = atof(plett);
  if(!strcmp(ReIN->SlitUn,RightEnUn1)) BeamSpread = BeamSpread;
  else if(!strcmp(ReIN->SlitUn,RightSpreadUn2)) BeamSpread *= Tb/100.; 
  else printf("Main: wrong energy spread unit\n");

  projectile->Part.SetPxPyPzE(0.,0.,sqrt(pow(Tb+projectile->Mass,2)-pow(projectile->Mass,2)),Tb+projectile->Mass);
  
  Qreaction = 0.;
  for(int ip=0;ip<NofDetPart+NofUnObsPart;ip++) Qreaction -= ejectile[0][ip][0].Mass;
  Qreaction +=projectile->Mass + target->Mass;
}
//-----------------------------------------------------------------------------
double ERHe8Analyzer::Stepantsov(char* D,int Z,double A,double I){
  double a0_2 = 20.19773;
  double a1_2 = 265.01317;
  double a2_2 = 54.27665;
  double a0_3 = 17.46898;
  double a1_3 = 272.31081;
  double a2_3 = 50.04487;
  double P,B,Bro,Discr,T;
  double R = 3.;
  double a0,a1,a2;
  char Dipole1[]="D1";
  char Dipole2[]="D2";
  
  if(!strcmp(D,Dipole1))
  {a0 = a0_2;
  a1 = a1_2;
  a2 = a2_2;}
  else if(!strcmp(D,Dipole2))
  {a0 = a0_3;
  a1 = a1_3;
  a2 = a2_3;}
  else
  {printf("Stepantsov: one should write D1 or D2 only!!!\n"); T = 0.;}
  Discr = sqrt(a1*a1-4.*(a0-I)*a2);
  if(Discr>0.) {B = (-a1 + Discr)/2./a2;}
  else {B = 0.;}
  Bro = B*R;
  P = Bro*Z/3.3356*1000.;
  T = sqrt(P*P + A*A) - A;
  return T;
}
//-----------------------------------------------------------------------------
void ERHe8Analyzer::TelescopeThresholds(){
  double Threshold[Ntelescopes][NofDetPart];
  printf("Telescope thresholds:\n");
  for(int it=0;it<Ntelescopes;it++)
  {
    for(int ip=0;ip<NofDetPart;ip++)
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
double ERHe8Analyzer::EiEo(double tableER[][105],double Tp,double Rp){
  if(Tp<0.1||Tp>1000.)
  {printf("Energy is out of range\n"); return -1.;}
  
  int look=0;
  while (Tp>tableER[0][look]) {look++;}

  double R1=tableER[2][look-1]+
  +tableER[3][look-1]*Tp+
  +tableER[4][look-1]*Tp*Tp+
  +tableER[5][look-1]*Tp*Tp*Tp;
  
  double R2 = R1 - Rp;
  
  if(Rp>0.)
  {if(R2<tableER[1][0]) {return 0.;}}
  else
  {if(R2>=tableER[1][104]) {return -2.;
  printf("table is out of range\n");}}
  
  look=0;
  while (R2>tableER[1][look]) {look++;}
  
  double E1=tableER[6][look-1]+
  +tableER[7][look-1]*R2+
  +tableER[8][look-1]*R2*R2+
  +tableER[9][look-1]*R2*R2*R2;
  
  return E1;
};
//-----------------------------------------------------------------------------
void ERHe8Analyzer::PrintReaction(){
  printf("************ ReactionInput is decoded **********************\n");
  printf("So we're studying the following reaction:\n");
  printf("Projectile %s (M=%lf MeV)\n",projectile->NameOfNucleus,projectile->Mass);
  printf("interacts with the target nucleus %s (M=%lf MeV)\n",
  target->NameOfNucleus,target->Mass);
  printf(" \n");
  printf("As a result of the %s reaction we have:\n",ReIN->Mechanism);
  printf("%i detected particles:\n",NofDetPart);
  for(int ip=0;ip<NofDetPart;ip++) printf("%s (M=%lf MeV)\n",ejectile[0][ip][0].NameOfNucleus,ejectile[0][ip][0].Mass);
  printf("And %i unobserved particles:\n",NofUnObsPart);
  for(int ip=NofDetPart;ip<NofDetPart+NofUnObsPart;ip++) printf("%s (M=%lf MeV)\n",ejectile[0][ip][0].NameOfNucleus,ejectile[0][ip][0].Mass);
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
void ERHe8Analyzer::ElossTOFaMWPCaTarget(){
  /*
  Есть два способа определения энергии конкретного
  налетающего иона по времени пролета. Более прогрессивный, но пока не
  работавший - с абсолютной калибровкой времени и пересчетом энергии из
  скорости. Более древний и реально использующийся - В дипольном магните
  на входе и выходе стоят щели, которые пропускают частицы с определенным
  отношением импульса к заряду. Величина, характеризующая это отношение -
  магнитная жесткость B*rho - где B вертикальная проекция магнитного поля,
  а rho - радиус кривизны траектории в горизонтальной плоскости. Связь
  между B*rho и энергией дается "формулой Степанцова" (Stepantsov.cpp).
  Производятся имзерения времени прорлета ионов без калибровки разности
  времен. Получается пик с некоторым центром. Положение центра пика
  сопоставляется энергии, вычисленной через B*rho/ энергия каждого
  конкретного иона считается через его ТОF - в зависимости от положения
  измерннного TOF относительно центра пика. Так вот, положение центра
  пика измеренных некалиброванных TOF- называется tof_0.
  */
  char ShowTrack[10];
  range = UpMat->PlasticThick2;
  Tb = EiEo(UpMat->beam_TOF,projectile->Part.E()-projectile->Mass,UpMat->PlasticThick2);
  p_beam = sqrt(pow(Tb+projectile->Mass,2)-pow(projectile->Mass,2));
  projectile->Part.SetPxPyPzE(0.,0.,p_beam,Tb+projectile->Mass);
  if(ReIN->TOFis)
  {
    tof_0 = UpMat->PlasticDist/sqrt(1-pow(projectile->Mass/projectile->Part.E(),2))/slight;
    printf("\n");
    printf("TOF measured between two plastics on the base of %lf cm is %lf ns\n",UpMat->PlasticDist,tof_0);
  }
  strcpy(ShowTrack,"visible");
  Tb = UpstreamEnergyLoss(UpMat,projectile,ReIN->TOFis,ReIN->TRACKINGis,ShowTrack);
  p_beam = sqrt(pow(Tb+projectile->Mass,2)-pow(projectile->Mass,2));
  projectile->Part.SetPxPyPzE(0.,0.,p_beam,Tb+projectile->Mass);

  //target
  if(!strcmp(UpMat->HeatScreenAns,"yes")) Tb = EiEo(UpMat->beam_TARwin,Tb,UpMat->HeatScreenThick);
  if(Tb>0.1) Tb = EiEo(UpMat->beam_TARwin,Tb,UpMat->FoilThick);
  if(Tb>0.1) Tb = EiEo(UpMat->beam_target,Tb,UpMat->TarThick*UpMat->TarPress*TempNorm/UpMat->TarTemp/2.);
  p_beam = sqrt(pow(Tb+projectile->Mass,2)-pow(projectile->Mass,2));
  projectile->Part.SetPxPyPzE(0.,0.,p_beam,Tb+projectile->Mass);

  CM0->Part = projectile->Part + target->Part;
  projectile->Part.Boost(-CM0->Part.BoostVector());
  target->Part.Boost(-CM0->Part.BoostVector());

  t_cm0 = projectile->Part.E()+target->Part.E()-projectile->Mass-target->Mass;
  t_cm = t_cm0 + Qreaction;

  projectile->Part.Boost(CM0->Part.BoostVector());
  target->Part.Boost(CM0->Part.BoostVector());

  printf("\n");
  printf("TARGET: %s H=%lf cm (T=%lf K, P=%lf bar) with %s entance window H=%lf cm\n",
    target->NameOfNucleus,UpMat->TarThick,UpMat->TarTemp,UpMat->TarPress,UpMat->TarFoilMatter,UpMat->FoilThick);
  printf("In the center of the target: %lf MeV (%lfA MeV)\n",
    projectile->Part.E()-projectile->Mass,(projectile->Part.E()-projectile->Mass)/projectile->AtMass);
  printf("Ecm+Q = %lf, Ecm = %lf MeV\n",t_cm,t_cm0);
  printf("************************************************************\n");
  Tp1 = projectile->Part.E()-projectile->Mass;
  Tp2 = (projectile->Part.E()-projectile->Mass)/projectile->AtMass;
  Tp3 = t_cm;
}
//-----------------------------------------------------------------------------
double ERHe8Analyzer::UpstreamEnergyLoss(UpstreamMatter* pU,Particle* pP,bool Cond1, bool Cond2,char* Show){
  char Matter[32]="visible";
  double Tb,range;
  if(Cond1)
  {
    range = pU->PlasticThick2;
    Tb = EiEo(pU->beam_TOF,pP->Part.E()-pP->Mass,range);  /*Energy after 2nd plastic*/
    if(!strcmp(Show,Matter))
    {
      printf("TOF PLASTIC2 (%s), Thickness=%lf cm\n",pU->PlasticMatter2,pU->PlasticThick2);
      printf("%lf MeV (%lfA MeV)\n",Tb,Tb/pP->AtMass);
    }
  }
  if(Cond2)
  {
    for(int i=1;i<3;i++)
    {
      range = pU->MWwinThick;
      Tb = EiEo(pU->beam_MWwin,Tb,range); /*Energy after entrance MWPC window*/

      range = pU->MWgasThick;
      Tb = EiEo(pU->beam_MWgas,Tb,range); /*Energy after MWPC gas*/

      range = pU->MWcathThick;
      Tb = EiEo(pU->beam_MWcathod,Tb,range);  /*Energy after MWPC cathodes*/

      range = pU->MWwinThick;
      Tb = EiEo(pU->beam_MWwin,Tb,range); /*Energy after exit MWPC window*/

      if(!strcmp(Show,Matter))
      {
        printf("MWPC%i: 2 %s windows (%lf cm), %s cathodes (%lf cm), gas %s (%lf cm)\n",
          i,pU->MWwinMatter,pU->MWwinThick,pU->MWcathMatter,
          pU->MWcathThick,pU->MWgasMatter,pU->MWgasThick);
        printf("%lf MeV (%lfA MeV)\n",Tb,Tb/pP->AtMass);
      }
    }
  }
  return Tb;
};
//-----------------------------------------------------------------------------
ClassImp(ERHe8Analyzer)
ClassImp(RawTrack)
ClassImp(TrackData)
