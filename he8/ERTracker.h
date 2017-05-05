// -------------------------------------------------------------------------
// -----                  ERTracker header file          -----
// -----                  Created 04/17  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERTracker_H
#define ERTracker_H

#include "TRandom.h"
#include "TLorentzVector.h"
#include "TVector3.h"
#include "TGeoManager.h"

#include "FairTask.h"

#include "ERMwpcEvent.h"
#include "ERTofCalEvent.h"
#include "ERDsrdCalEvent.h"
#include "ERTelescopeCalEvent.h"

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

class RawTrack
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
};

class TrackData
{
public:
  TrackData(){}
  ~TrackData(){}
  double xmw1,ymw1,xmw2,ymw2,xbt,ybt,zbt,thb,phib;
  double xbd1,ybd1,xbd2,ybd2,xbd3,ybd3,xbd4,ybd4,xbd5,ybd5,xbd6,ybd6;
};

class TelData
{
public:
  TelData(){}
  ~TelData(){}
  double dep11,dep12,dep13,dep21,dep22,dep23,dep31,dep32,dep33;
  double x11,y11,z11,x21,y21,z21,x31,y31,z31;
  double al111,al112,al113,al121,al122,al123,al131,al132,al133,al211,al212,al213,al221,al222;
  double al223,al231,al232,al233,al311,al312,al313,al321,al322,al323,al331,al332,al333;
  double tcheck11,tcheck12,tcheck13,tcheck21,tcheck22,tcheck23,tcheck31,tcheck32,tcheck33;
  double t11,t12,t13,t21,t22,t23,t31,t32,t33,th11,phi11,th12,phi12,th13,phi13;
  double th21,phi21,th22,phi22,th23,phi23,th31,phi31,th32,phi32,th33,phi33;
  double t11cm0,t12cm0,t13cm0,t21cm0,t22cm0,t23cm0,t31cm0,t32cm0,t33cm0;
  double th11cm0,th12cm0,th13cm0,th21cm0,th22cm0,th23cm0,th31cm0,th32cm0,th33cm0;
  double t11cmp,t12cmp,t13cmp,t21cmp,t22cmp,t23cmp,t31cmp,t32cmp,t33cmp;
  double th11cmp,th12cmp,th13cmp,th21cmp,th22cmp,th23cmp,th31cmp,th32cmp,th33cmp;
  double pz11cmp,pz12cmp,pz13cmp,pz21cmp,pz22cmp,pz23cmp,pz31cmp,pz32cmp,pz33cmp;
};


class InclusiveData
{
public:
  InclusiveData(){}
  ~InclusiveData(){}
  double tmis11,tmis12,tmis13,tmis21,tmis22,tmis23,tmis31,tmis32,tmis33;
  double thmis11,thmis12,thmis13,thmis21,thmis22,thmis23,thmis31,thmis32,thmis33;
  double phimis11,phimis12,phimis13,phimis21,phimis22,phimis23,phimis31,phimis32,phimis33;
  double exmis11,exmis12,exmis13,exmis21,exmis22,exmis23,exmis31,exmis32,exmis33;
  double tmis11cm0,tmis12cm0,tmis13cm0,tmis21cm0,tmis22cm0,tmis23cm0,tmis31cm0,tmis32cm0,tmis33cm0;
  double thmis11cm0,thmis12cm0,thmis13cm0,thmis21cm0,thmis22cm0,thmis23cm0,thmis31cm0,thmis32cm0,thmis33cm0;
  double tmis11cmp,tmis12cmp,tmis13cmp,tmis21cmp,tmis22cmp,tmis23cmp,tmis31cmp,tmis32cmp,tmis33cmp;
  double thmis11cmp,thmis12cmp,thmis13cmp,thmis21cmp,thmis22cmp,thmis23cmp,thmis31cmp,thmis32cmp,thmis33cmp;
};

class DoubleCoincidence
{
public:
  DoubleCoincidence(){}
  ~DoubleCoincidence(){}
  double tpar1122,tpar1123,tpar1132,tpar1133,tpar1221,tpar1223,tpar1231,tpar1233,tpar1321,tpar1322,tpar1331,tpar1332;
  double thpar1122,thpar1123,thpar1132,thpar1133,thpar1221,thpar1223,thpar1231,thpar1233,thpar1321,thpar1322,thpar1331,thpar1332;
  double phipar1122,phipar1123,phipar1132,phipar1133,phipar1221,phipar1223,phipar1231,phipar1233,phipar1321,phipar1322,phipar1331,phipar1332;
  double expar1122,expar1123,expar1132,expar1133,expar1221,expar1223,expar1231,expar1233,expar1321,expar1322,expar1331,expar1332;
  double tspe1122,tspe1123,tspe1132,tspe1133,tspe1221,tspe1223,tspe1231,tspe1233,tspe1321,tspe1322,tspe1331,tspe1332;
  double thspe1122,thspe1123,thspe1132,thspe1133,thspe1221,thspe1223,thspe1231,thspe1233,thspe1321,thspe1322,thspe1331,thspe1332;
  double phispe1122,phispe1123,phispe1132,phispe1133,phispe1221,phispe1223,phispe1231,phispe1233,phispe1321,phispe1322,phispe1331,phispe1332;
  double exspe1122,exspe1123,exspe1132,exspe1133,exspe1221,exspe1223,exspe1231,exspe1233,exspe1321,exspe1322,exspe1331,exspe1332;
  double th1122cmpp,th1123cmpp,th1132cmpp,th1133cmpp,th1221cmpp,th1223cmpp,th1231cmpp,th1233cmpp,th1321cmpp,th1322cmpp,th1331cmpp,th1332cmpp;
  double pspe1122cmp,pspe1123cmp,pspe1132cmp,pspe1133cmp,pspe1221cmp,pspe1223cmp,pspe1231cmp,pspe1233cmp,pspe1321cmp,pspe1322cmp,pspe1331cmp,pspe1332cmp;
  double pxspe1122cmp,pxspe1123cmp,pxspe1132cmp,pxspe1133cmp,pxspe1221cmp,pxspe1223cmp,pxspe1231cmp,pxspe1233cmp,pxspe1321cmp,pxspe1322cmp,pxspe1331cmp,pxspe1332cmp;
  double pyspe1122cmp,pyspe1123cmp,pyspe1132cmp,pyspe1133cmp,pyspe1221cmp,pyspe1223cmp,pyspe1231cmp,pyspe1233cmp,pyspe1321cmp,pyspe1322cmp,pyspe1331cmp,pyspe1332cmp;
  double pzspe1122cmp,pzspe1123cmp,pzspe1132cmp,pzspe1133cmp,pzspe1221cmp,pzspe1223cmp,pzspe1231cmp,pzspe1233cmp,pzspe1321cmp,pzspe1322cmp,pzspe1331cmp,pzspe1332cmp;
  double thspe1122cmp,thspe1123cmp,thspe1132cmp,thspe1133cmp,thspe1221cmp,thspe1223cmp,thspe1231cmp,thspe1233cmp,thspe1321cmp,thspe1322cmp,thspe1331cmp,thspe1332cmp;
  double thty1122,thty1123,expar1122pri,expar1123pri,expar1132pri,expar1133pri;
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
  TelData* tel;
  InclusiveData* inclu;
  DoubleCoincidence* dcoin;

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
  void InitOutputs();
  TVector3 Traject(Telescope* Dx,Telescope* Dy,int Nx,int Ny,TVector3 Vint);
  TVector3 VertexPosition(TVector3 V1,TVector3 V2,TVector3 V3,TVector3 V4);
  void InitMemory();
  void InLabFrame();
private:

  virtual void SetParContainers();
  
  ClassDef(ERTracker,1)
};

#endif
