// -------------------------------------------------------------------------
// -----                  ERHe8Analyzer header file          -----
// -----                  Created 04/17  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERHe8Analyzer_H
#define ERHe8Analyzer_H

#include "TRandom.h"
#include "TLorentzVector.h"
#include "TVector3.h"
#include "TGeoManager.h"

#include "FairTask.h"

#include "ERMwpcEvent.h"
#include "ERTofCalEvent.h"
#include "ERDsrdCalEvent.h"
#include "ERTelescopeCalEvent.h"
#include "ERTelData.h"
#include "ERInclusiveData.h"

#define pi 3.14159265358979323846
#define rad 0.01745329252 /* pi/180 */
#define slight 29.9792458 /* light speed (cm/nsec) */
#define hc 197.3425 /* Plank*c (MeV*fm) */
#define AMU 931.494028 /* atomic mass unit (MeV) */
#define TempNorm 293.16 /* Normal temperature (K) */

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

class RawTrack : public TNamed
{
public:
  RawTrack(){}
  ~RawTrack(){}
  int mMW11;
  int mMW12;
  int mMW21;
  int mMW22;
  int mcMW11;
  int mcMW12;
  int mcMW21;
  int mcMW22;
  int nMW11[32];
  int nMW12[32];
  int nMW21[32];
  int nMW22[32];
  short mwpc[8];
  ClassDef(RawTrack,1)
};

class TrackData : public TNamed
{
public:
  TrackData(){}
  ~TrackData(){}
  double xmw1,ymw1,xmw2,ymw2,xbt,ybt,zbt,thb,phib;
  double xbd1,ybd1,xbd2,ybd2,xbd3,ybd3,xbd4,ybd4,xbd5,ybd5,xbd6,ybd6;
  ClassDef(TrackData,1)
};

class ERHe8Analyzer : public FairTask {

public:
  /** Default constructor **/
  ERHe8Analyzer();

  /** Constructor 
  ** verbose: 1 - only standard log print
  **/
  ERHe8Analyzer(Int_t verbose);

  /** Destructor **/
  ~ERHe8Analyzer();

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

  char projname[5];
  char tarname[5];  
  char DetectedPart[32];
  char UnObservedPart[32];

  int Ntelescopes = 0;
  int NTelMax=5;        /* Max nUpMatber of telescopes */
  int NLayMax=10;       /* Max nUpMatber of layers in telescopes */
  /*индекс NDetMax на случай если посде телескопа площадь разбита на
  несколько детекторов, например,
  за кольцевым кремнием, стоят CsI, разбитые на кусочки - каждвй кусочек -
  это детектор. Получается, гранулярность,
  независящая от стрипов.
  */
  int NDetMax=20;       /* Max nUpMatber of detectors in layer */
  int NDivMax = 0;      /* Max sUpMatm of X- and Y-strips on both sides of a detector */
  int NDivXYMax = 0;      /* Max nUpMatber of X- or Y-strips a a single side of a detector */

  Telescope*** Det;
  int* layer;
  int** NDet;
  // ПРИЗНАК КАЧЕСТВА ДАННЫХ
  int* mp;

  int NofDetPart;
  int NofInPart;
  Particle*** ejectile;
  Particle* target;
  Particle* projectile;
  Particle* CM0;
  /*
  некая совокупность нуклонов в налетающем ядре, которая,
  согласно рассматриваемому механизму, не участвует в реакции, а просто
  просто продолжает движение.
  */
  Particle**** spectator;
  /*
  некая совокупность нуклонов в налетающем ядре, которая,
  согласно рассматриваемому механизму, участвует в реакции.
  */
  Particle**** participants;
  int** TCheck; //рудимент
  /*
  Разница между измеренным (сумма детекторов) и вычисленным выделением
  энергии. вычисления делаются с учетом средних потерь в мертвых слоях.
  Если отклонение не велико - событие принимается, если велико -
  отбрасывается:
  */
  double**** al;
  /*
  Эти переменные заполняются в симуляции. В обработке данных они тоже
  используются для унификации вида данных из симуляций и и з измерений:
  */
  double**** deposit;
  double**** DepoX;
  double**** DepoY;
  int*** mpd;
  /*
  Mu - всегда множественность (количество частиц, пойманных в детекторе).
  MuYT - временная множественность - timestamps в событии могут
  группироваться в несколько кластеров.
  */
  int*** MuX;
  int*** MuY;
  int*** MuXT;
  int*** MuYT;
  /*
  проекция на плоскость детекторов траектории налетающего
  иона, определенной с помощью mwpc:*/
  double*** xbdet0;
  double*** ybdet0;
  /*Координаты - любые промежуточные:*/
  double** cx;
  double** cy;
  double** cz;
  //все переменные с hit ниже - для симуляций.
  int**** HitX;
  int**** HitY;
  int**** HitXT;
  int**** HitYT;
  int**** NhitX;
  int**** NhitY;
  int**** NhitXT;
  int**** NhitYT;
  //объект, куда пишутся разности лоренц-векторов между тем, что было в начальной системе (target+projectile) и тем, что зарегистрировано.
  Particle*** mis;


  double tof_0;
  //кинетическая энергия target + projectile в центре масс.
  double t_cm;
  // энергетический эффект реакции - сколько энергии высвобождается, если "+", сколько поглощается, если "-"
  double Qreaction;
  //t_cm0 = t_cm+Qreaction это кинетическая энергия, доступная продуктам реакции в ЦМ.
  double t_cm0;
  //кинетическая энергия налетающей частицы (beam)
  double Tb;
  // различные промежуточные кинетические энергии
  double Ta,dT,Tout,Tboutput;
  double BeamSpread;
  int NofUnObsPart;
  // релятивистский гамма-фактор.
  double beta_b,gamma_b,p_beam;
  // пробег в толще вещества
  double range;
  // это для симуляций
  double procur,promax;
  // тэта и фи в ЦМ для спектатора
  double thscm,phiscm;
  // угол тэта в ЦМ между векторами скорости target и participant
  double thtp;
  // вспомогательные переменные для кин энергии
  double Tp1,Tp2,Tp3;

  TVector3** AngleDet;
  /*
  Это единичные (? так ли это?) радиус-вектора (обычные, идущие из начала
  отсчета не лоренц) которые показывают направления полета иона из пучка
  (Vbeam); направление на точку пересечения ионом дальней от мишени
  станции MWPC (far -VmwFa ) и ближней станции MWPc(close-VmwCl):
  */
  TVector3 Vbeam;
  TVector3 VmwFa,VmwCl,VbeamPlay,Vert1,Vert2;

  /*
  Это некие расстояния, нужные при определении точки входа налетающего иона в мишень. M - measured P-played
  */
  double MdistX,MdistY,MdistZ,Rdist,PdistX,PdistY,PdistZ;

  /*
  Ниже кусок неактуальный - посвящен анализу числа кластеров в детекторе.
  кластер - это непрерывная последовательность зажЖенных стрипов. Нам пока
  что достаточно рассматривать события, где сработал ровно один стрип в
  каждой плоскости детектора.
  */
  int good_mw1 = 0;
  int badclu_mw1 = 0;
  int goodclu_mw1 = 0;
  int good_mw2 = 0;
  int badclu_mw2 = 0;
  int goodclu_mw2 = 0;
  int good_mw = 0;
  int badclu_mw = 0;
  int goodclu_mw = 0;

  int good_mbeam = 0;
  int good_he6 = 0;
  int good_mp0 = 0;
  int good_mp1 = 0;
  int good_mp2 = 0;
  int nread_tot = 0;
  int FlagCounter1 = 0;
  int FlagCounter2 = 0;

  int i_flag_MW;
  //in
  ERMwpcEvent* fMwpcEvent;
  ERTofCalEvent* fTofEvent;
  ERDsrdCalEvent* fDsrdEvent;
  ERTelescopeCalEvent* fTelescopeEvent;
  //out
  RawTrack* RawT;
  TrackData* trackD;
  ERTelData* tel;
  ERInclusiveData* inclu;

  TGeoManager *geom;
  TGeoNode *trajectory;

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
  void TelescopeThresholds();
  double EiEo(double tableER[][105],double Tp,double Rp);
  void PrintReaction();
  void ElossTOFaMWPCaTarget();
  double UpstreamEnergyLoss(UpstreamMatter* pU,Particle* pP,bool Cond1,
    bool Cond2,char* Show);
  void MWPC();
  int mcluMW(int mMW,int* nMW);
  double coordMW(UpstreamMatter* pT,RawTrack* pR,char* MWid,char* XY);
  void Tof();
  TVector3 Traject(Telescope* Dx,Telescope* Dy,int Nx,int Ny,TVector3 Vint);
  TVector3 VertexPosition(TVector3 V1,TVector3 V2,TVector3 V3,TVector3 V4);
  void InitMemory();
  void InLabFrame();
  void InReactionCM();
  void InProjectileFrame();
  void ReactionPreparation();
  void InitParticlesInOutputs();
  void CheckInOutAZ();
private:

  virtual void SetParContainers();
  
  ClassDef(ERHe8Analyzer,1)
};

#endif
