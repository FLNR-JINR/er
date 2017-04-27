// -------------------------------------------------------------------------
// -----                  ERTracker header file          -----
// -----                  Created 04/17  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERTracker_H
#define ERTracker_H

#include "TRandom.h"
#include "TLorentzVector.h"
#include "TVector3.h"

#include "FairTask.h"

#define pi 3.14159265358979323846
#define rad 0.01745329252 /* pi/180 */
#define slight 29.9792458 /* light speed (cm/nsec) */
#define hc 197.3425 /* Plank*c (MeV*fm) */
#define amu 931.494028 /* atomic mass unit (MeV) */

class Telescope
{
public:
  Telescope(){};
  ~Telescope(){}
  char Shape[12];
  char Matt[12];
  char StripFB[12];
  double Theta;
  double Psi;
  double DeltaAlpha;
  double DeltaBeta;
  double DeltaGamma;
  double Dist;
  double DeadZoneF;
  double DeadZoneB;
  double Thick;
  double Rin;
  double Rout;
  double DeltaPhi;
  double OffsetX;
  double OffsetY;
  double SizeX;
  double SizeY;
  int NstripX;
  int NstripY;
  int Nring;
  int Nsector;
  TVector3 DetLabVect;
  TVector3 DetOwnAxisX; 
  TVector3 DetOwnAxisY; 
  TVector3 DetOwnAxisZ; 
};

class SimulationData 
{
public:
  SimulationData(){};
  ~SimulationData(){}
  char WaveFunction[10];
  char FreeScatt[2];
  double echeck;
  double thtpmin;
  double thtpmax;
  double AngleMin;
  double AngleMax;
  int NEventsToBePlayed;
  double ResE0;
  double ResS0;
  double MomP0;
  double MomS0;
};

class ReactionDataInput 
{
public:
  ReactionDataInput(){};
  ~ReactionDataInput(){}
  char ReactionName[32];
  char Mechanism[32];
  char AboutBeam[32];
  char AboutSlit[32];
  char EnergyUn[32];
  char SlitUn[32];
  char Fname[32];
  bool Simulation;
  bool Vertex;
  bool DetectorTune;
  bool TOFis;
  bool TRACKINGis;
  bool TrackCheck;
  bool WriteRawData;
  bool WriteCalData;
  bool WriteTofData;
  bool WriteTrackData;
  bool WriteTelData;
  bool WritePhysData;
  bool WritePlayData;
  bool WriteRawTrack;
  int ifill[8];
};

class UpstreamMatter 
{
public:
  UpstreamMatter(){};
  ~UpstreamMatter(){}
  char PlasticMatter1[16];
  char PlasticMatter2[16];
  char MWwinMatter[16];
  char MWcathMatter[16];
  char MWgasMatter[16];
  char HeatScreenAns[16];
  char HeatScreenMatter[16];
  char TarShape[16];
  char TarFoilMatter[16];
  int MWclosXNum;
  int MWclosYNum;
  double MWXYdist;
  double MWstep;
  int MWNcathodes;
  int MWNwires;
  double MWfarDist;
  double MWfarXshift;
  double MWfarYshift;
  int MWfarXNum;
  int MWfarYNum;
  double MWclosDist;
  double MWclosXshift;
  double MWclosYshift;
  double PlasticThick1;
  double PlasticThick2;
  double MWwinThick;
  double MWcathThick;
  double MWgasThick;
  double PlasticAngle1;
  double PlasticAngle2;
  double PlasticDist;
  double TofRes;
  double tF3l_rng;
  double tF3r_rng;
  double tF4l_rng;
  double tF4r_rng;
  double tF3_dlt;
  double tF4_dlt;
  double beam_TOF[10][105];
  double beam_MWwin[10][105];
  double beam_MWgas[10][105];
  double beam_MWcathod[10][105];
  double beam_TARwin[10][105];
  double beam_target[10][105];
  double beam_heatscreen[10][105];
  double TarRadius;
  double TarXshift;
  double TarYshift;
  double TarZshift;
  double TarFoilThick;
  double TarThick;
  double TarHeight;
  double TarWallThick;
  double TarEntrHoleThick;
  double TarTemp;
  double TarPress;
  double TarAngle;
  double FoilThick;
  double TarEntrHoleRad;
  double MeniskSize;
  double HeatScreenThick;
  double InHscrRad;
  double HscrWallWidth;
  double HscrHeight;
  double EntrHRad;
  double ExHX;
  double ExHY;
};

class DownstreamMatter
{
public:
  double ej_si[10][105];
  double ej_csi[10][105];
  double ej_TARwin[10][105];
  double ej_target[10][105];
  double ej_heatscreen[10][105];
};

class Particle
{
public:
  Particle(){};
  ~Particle(){};
  char* NameOfNucleus;
  double Mass;
  double Excitation;
  int AtNumber;
  int AtMass;
  void ParticleID(char* name, char* path);
  double ReturnMass(char* NON,char* WayMass);
  char* ReactionInput(char* WayReaction);
  TLorentzVector Part;
};

class ERTracker : public FairTask {

public:
  /** Default constructor **/
  ERTracker();

  /** Constructor 
  ** verbose: 1 - only standard log print
  **/
  ERTracker(Int_t verbose);

  /** Destructor **/
  ~ERTracker();

  /** Virtual method Init **/
  virtual InitStatus Init();

  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

  /** Virtual method Finish **/
  virtual void Finish();

  /** Virtual method Reset **/
  virtual void Reset();
protected:
  Int_t fEvent;
  ReactionDataInput* ReIN;
  SimulationData* SimDat;
  UpstreamMatter* UpMat;
  DownstreamMatter** EjMat;

  int Ntelescopes = 0;
  int NTelMax=5;        /* Max nUpMatber of telescopes */
  int NLayMax=10;       /* Max nUpMatber of layers in telescopes */
  int NDetMax=20;       /* Max nUpMatber of detectors in layer */
  int NDivMax = 0;      /* Max sUpMatm of X- and Y-strips on both sides of a detector */
  int NDivXYMax = 0;      /* Max nUpMatber of X- or Y-strips a a single side of a detector */

  Telescope*** Det;
  int* layer;
  int** NDet;

  int NofDetPart;
  int NofInPart;
  Particle*** ejectile;
  Particle* target;
  Particle* projectile;
  double t_cm,t_cm0,tof_0,Tb,Ta,dT,Tout,Tboutput,Qreaction;
  double BeamSpread;
  int NofUnObsPart;

  void ReadTelescopeParameters();
  void CreateTelescopeGeometry();
  void ReadInputData();
  void ReadDeDx();
  int HowMuchParticles(char* str);
  void WhatParticlesInOut(Particle* ptr,char* str,int N);
  int ReadRint(char* Fname,double Ranges[][105]);
  int intrp4(double* x,double* y, double* c);
  void DefineBeamEnergy();
  double Stepantsov(char* D,int Z,double A,double I);

private:

  virtual void SetParContainers();
  
  ClassDef(ERTracker,1)
};

#endif
