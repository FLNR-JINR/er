#include "ERTracker.h"

#include<iostream>
using namespace std;

#include "TGeoManager.h"
#include "TGeoMaterial.h"
#include "TGeoMedium.h"
#include "TGeoCompositeShape.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

// ----------------------------------------------------------------------------
ERTracker::ERTracker()
  : FairTask("ERTracker"),
  fEvent(0),
  ReIN(new ReactionDataInput()),
  SimDat(new SimulationData()),
  UpMat (new UpstreamMatter()),
  Ntelescopes(0),
  NTelMax(5),
  NLayMax(10),
  NDetMax(20),
  NDivMax(0),
  NDivXYMax(0),
  Telescope(NULL),
  layer(NULL),
  NDet(NULL)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERTracker::ERTracker(Int_t verbose)
  : FairTask("ERTracker", verbose),
  fEvent(0),
  ReIN(new ReactionDataInput()),
  SimDat(new SimulationData()),
  UpMat (new UpstreamMatter()),
  Ntelescopes(0),
  NTelMax(5),
  NLayMax(10),
  NDetMax(20),
  NDivMax(0),
  NDivXYMax(0),
  Telescope(NULL),
  layer(NULL),
  NDet(NULL
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERTracker::~ERTracker()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERTracker::SetParContainers()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus ERTracker::Init()
{
  ReadInputData();
  
  ReadTelescopeParameters();
  
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
  char DetectedPart[32];
  char UnObservedPart[32];
  char zero[]="";
  int NofUnObsPart = 0;
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
  int NofInPart = HowMuchParticles(InputChannel);
  if(NofInPart<2||NofInPart>2) {printf("Wrong number of particles in the Input channel\n");}
  int NofDetPart = HowMuchParticles(DetectedPart);
  if(NofDetPart==0) {printf("Wrong number of detected particles\n");}
  
  cout  << "Define particles in the input channel" << endl;
  char projname[5];
  char tarname[5];
  plett = strtok(InputChannel,"+");
  strcpy(projname,plett);
  plett = strtok(NULL,"+");
  strcpy(tarname,plett);

  TVector3 VmwFa,VmwCl,Vbeam,VbeamPlay,Vert1,Vert2;
  TVector3 AngleDet[Ntelescopes][NLayMax];

  int mp[Ntelescopes];
  int mpd[Ntelescopes][NLayMax][NDetMax];
  int MuX[Ntelescopes][NLayMax][NDetMax];
  int MuY[Ntelescopes][NLayMax][NDetMax];
  int MuXT[Ntelescopes][NLayMax][NDetMax];
  int MuYT[Ntelescopes][NLayMax][NDetMax];
  int HitX[Ntelescopes][NLayMax][NDetMax][NofDetPart];
  int HitY[Ntelescopes][NLayMax][NDetMax][NofDetPart];
  int HitXT[Ntelescopes][NLayMax][NDetMax][NofDetPart];
  int HitYT[Ntelescopes][NLayMax][NDetMax][NofDetPart];
  int NhitX[Ntelescopes][NLayMax][NDetMax][NDivXYMax];
  int NhitY[Ntelescopes][NLayMax][NDetMax][NDivXYMax];
  int NhitXT[Ntelescopes][NLayMax][NDetMax][NDivXYMax];
  int NhitYT[Ntelescopes][NLayMax][NDetMax][NDivXYMax];

  int TCheck[Ntelescopes][NLayMax];

  double xbdet0[Ntelescopes][NLayMax][NDetMax];
  double ybdet0[Ntelescopes][NLayMax][NDetMax];
  double deposit[Ntelescopes][NLayMax][NDetMax][NDivXYMax];
  double DepoX[Ntelescopes][NLayMax][NDetMax][NDivXYMax];
  double DepoY[Ntelescopes][NLayMax][NDetMax][NDivXYMax];
  double al[Ntelescopes][NLayMax][NofDetPart][NDivXYMax];
  double thty[Ntelescopes][NofDetPart][Ntelescopes][NofDetPart];

  double cx[Ntelescopes][NLayMax];
  double cy[Ntelescopes][NLayMax];
  double cz[Ntelescopes][NLayMax];

  Particle projectile,target,CM0,PlayParticle;
  Particle intermed[NofDetPart+NofUnObsPart];
  Particle PlayPairs[NofDetPart+NofUnObsPart+1][NofDetPart+NofUnObsPart+1];
  Particle PlayEjectile[NofDetPart+NofUnObsPart];
  Particle PlaySpectator[NofDetPart+NofUnObsPart+1][NofDetPart+NofUnObsPart+1];
  Particle ejectile[Ntelescopes][NofDetPart+NofUnObsPart][NDivXYMax];
  Particle mis[Ntelescopes][NofDetPart][NDivXYMax];
  Particle spectator[Ntelescopes][NofDetPart][Ntelescopes][NofDetPart];
  Particle participants[Ntelescopes][NofDetPart][Ntelescopes][NofDetPart];
  Particle aux[Ntelescopes][NofDetPart][Ntelescopes][NofDetPart];
  
  char WayMass[]="/home/vitaliy.schetinin/er/input/StableNuclei.dat";
  projectile.ParticleID(projname,WayMass);
  target.ParticleID(tarname,WayMass);
  target.Part.SetPxPyPzE(0.,0.,0.,target.Mass);

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
      PlayEjectile[ip].NameOfNucleus = new char [5];
      strcpy(PlayEjectile[ip].NameOfNucleus,ejectile[0][ip][0].NameOfNucleus);
      PlayEjectile[ip].Mass = ejectile[0][ip][0].Mass;
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
  
  int NPS=NofDetPart+NofUnObsPart-2;
  for(int ip=0;ip<NofDetPart+NofUnObsPart+1;ip++)
  {
    for(int ipx=0;ipx<NofDetPart+NofUnObsPart+1;ipx++)
    {
      if(ip!=ipx)
      {
        PlayPairs[ip][ipx].NameOfNucleus = new char [2];
        PlaySpectator[ip][ipx].NameOfNucleus = new char [2];
        strcpy(PlayPairs[ip][ipx].NameOfNucleus,"ps");
        strcpy(PlaySpectator[ip][ipx].NameOfNucleus,"ps");
        PlayPairs[ip][ipx].Excitation = 0.;
        PlaySpectator[ip][ipx].Excitation = 0.;
      }
    }
  }
  cout << "Check for Zin=Zout and Ain=Aout" << endl;
  int InAtNumber = projectile.AtNumber + target.AtNumber;
  int OutAtNumber = 0;
  for(int ip=0;ip<NofDetPart+NofUnObsPart;ip++) OutAtNumber += ejectile[0][ip][0].AtNumber;

  if(InAtNumber!=OutAtNumber) {printf("In ReactionInput.dat Zin is not equal to Zout!!!\n");}

  InAtNumber = projectile.AtMass + target.AtMass;
  OutAtNumber = 0;
  for(int ip=0;ip<NofDetPart+NofUnObsPart;ip++) OutAtNumber += ejectile[0][ip][0].AtMass;
  if(InAtNumber!=OutAtNumber) {printf("In ReactionInput.dat Ain is not equal to Aout!!!\n");}
  
  CreateTelescopeGeometry();
  
  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERTracker::Exec(Option_t* opt)
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERTracker::Reset()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERTracker::Finish()
{   
}
// ----------------------------------------------------------------------------
void ERTracker::ReadTelescopeParameters(){

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

  FILE *F1 = fopen("/home/vitaliy.schetinin/er/input/detsys.prm","r");
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
  F1 = fopen("/home/vitaliy.schetinin/er/input/detsys.prm","r");
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
void ERTracker::ReadInputData()
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
  FILE *F1 = fopen("/home/vitaliy.schetinin/er/input/ReactionInput.dat","r");
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
    F1 = fopen("/home/vitaliy.schetinin/er/input/Simulation.dat","r");
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
    F1 = fopen("/home/vitaliy.schetinin/er/input/track.dat","r");
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
  FILE *F1 = fopen("/home/vitaliy.schetinin/er/input/tof.dat","r");
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
  F1 = fopen("/home/vitaliy.schetinin/er/input/target.dat","r");
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
void ERTracker::CreateTelescopeGeometry(){

  cout << "CreateTelescopeGeometry started" << endl;
  double ThickStrip=0.0001;

  TGeoManager *geom=new TGeoManager("Reaction Chamber","geom"); 
  TGeoMaterial *matVacuum=new TGeoMaterial("Vacuum",0,0,0);
  TGeoMaterial *matSi=new TGeoMaterial("Si",24.,12,2.4);
  TGeoMedium *Vacuum=new TGeoMedium("Vacuum",1,matVacuum);
  TGeoMedium *Si=new TGeoMedium("Si",2,matSi);
  TGeoVolume *top=geom->MakeSphere("Top",Vacuum,0.,100.);
  geom->SetTopVolume(top);
  TGeoNode *trajectory;

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
int ERTracker::HowMuchParticles(char* str)
{
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

void ERTracker::WhatParticlesInOut(Particle* ptr,char* str,int N)
{
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
    {massa=amu*(double)a+MassExcess;break;}
  }
  fclose(F1);
  }
return massa;
}

void Particle::ParticleID(char* name, char* path)
{
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
        Mass=amu*(double)a+MassExcess;
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


ClassImp(ERTracker)
