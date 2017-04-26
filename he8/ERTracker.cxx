#include "ERTracker.h"

#include<iostream>
using namespace std;

#include "TGeoManager.h"
#include "TGeoMaterial.h"
#include "TGeoMedium.h"


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
  NDivXYMax(0)
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
  NDivXYMax(0)
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
  
  char WayMass[]="/home/vitaliy/er/input/StableNuclei.dat";
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
  //CreateTelescopeGeometry();
  
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

  int layer[NTelMax];
  int NDet[NTelMax][NLayMax];

  int it,il,id,is,imu,ip,count;
  int itx,ilx,idx,ipx;

  char DUpMatmyC[256];
  char Zeros[32];

  double read_angle1,read_angle2;
  double dist,dzf,dzb,thi,szx,szy,ofx,ofy,a1,a2,a3;
  double StripSizeX,StripSizeY,move,RingInit,RingFin,PhiInit,PhiFin;
  double ResolutionSi;
  double ResolutionCsI;
  int nx,ny;
  char st[12];
  char mat[12];
  char sha[12];

  FILE *F1 = fopen("/home/vitaliy/er/input/detsys.prm","r");
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

  Telescope Det[Ntelescopes][NLayMax][NDetMax];

  F1 = fopen("/home/vitaliy/er/input/detsys.prm","r");
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
  FILE *F1 = fopen("/home/vitaliy/er/input/ReactionInput.dat","r");
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
    F1 = fopen("/home/vitaliy/er/input/Simulation.dat","r");
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
    F1 = fopen("/home/vitaliy/er/input/track.dat","r");
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
  FILE *F1 = fopen("/home/vitaliy/er/input/tof.dat","r");
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
  F1 = fopen("/home/vitaliy/er/input/target.dat","r");
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
