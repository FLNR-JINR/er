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
#include "ERBeamDetRecoEvent.h"
#include "ERDsrdCalEvent.h"
#include "ERTelescopeCalEvent.h"
#include "ERTelData.h"
#include "ERInclusiveData.h"
#include "ERHe8EventHeader.h"
#include "ERParticle.h"

#define pi 3.14159265358979323846
#define hc 197.3425 /* Plank*c (MeV*fm) */

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

class DownstreamMatter
{
public:
  double ej_si[10][105];
  double ej_csi[10][105];
  double ej_TARwin[10][105];
  double ej_target[10][105];
  double ej_heatscreen[10][105];
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
/*
class TrackData : public TNamed
{
public:
  TrackData(){}
  ~TrackData(){}
  double xmw1,ymw1,xmw2,ymw2,xbt,ybt,zbt,thb,phib;
  double xbd1,ybd1,xbd2,ybd2,xbd3,ybd3,xbd4,ybd4,xbd5,ybd5,xbd6,ybd6;
  ClassDef(TrackData,1)
};
*/
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
  ERHe8EventHeader* header;

  Int_t fEvent;
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

  ERParticle*** ejectile;
  ERParticle* target;
  ERParticle* projectile;
  ERParticle* CM0;
  /*
  некая совокупность нуклонов в налетающем ядре, которая,
  согласно рассматриваемому механизму, не участвует в реакции, а просто
  просто продолжает движение.
  */
  ERParticle**** spectator;
  /*
  некая совокупность нуклонов в налетающем ядре, которая,
  согласно рассматриваемому механизму, участвует в реакции.
  */
  ERParticle**** participants;
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
  ERParticle*** mis;

  double Tb;
  // различные промежуточные кинетические энергии
  double Ta,dT,Tout,Tboutput;
  double BeamSpread;
  // пробег в толще вещества
  double range;
  // это для симуляций
  double procur,promax;
  // тэта и фи в ЦМ для спектатора
  double thscm,phiscm;
  // угол тэта в ЦМ между векторами скорости target и participant
  double thtp;

  TVector3** AngleDet;
  /*
  Это единичные (? так ли это?) радиус-вектора (обычные, идущие из начала
  отсчета не лоренц) которые показывают направления полета иона из пучка
  (Vbeam); направление на точку пересечения ионом дальней от мишени
  станции MWPC (far -VmwFa ) и ближней станции MWPc(close-VmwCl):
  */
  TVector3 Vert1,Vert2;

  /*
  Это некие расстояния, нужные при определении точки входа налетающего иона в мишень. M - measured P-played
  */
  double Rdist,PdistX,PdistY,PdistZ;

  int good_he6 = 0;
  int good_mp0 = 0;
  int good_mp1 = 0;
  int good_mp2 = 0;
  int nread_tot = 0;
  int FlagCounter1 = 0;
  int FlagCounter2 = 0;
  //in
  ERBeamDetRecoEvent* fBeamDetRecoEvent;
  ERDsrdCalEvent* fDsrdEvent;
  ERTelescopeCalEvent* fTelescopeEvent;
  //out
  ERTelData* tel;
  ERInclusiveData* inclu;

  TGeoManager *geom;
  TGeoNode *trajectory;

  float Qreaction;

  void ReadTelescopeParameters();
  void CreateTelescopeGeometry();
  void ReadDeDx();
  void WhatParticlesInOut(ERParticle* ptr,char* str,int N);
  void TelescopeThresholds();
  void PrintReaction();
  TVector3 Traject(Telescope* Dx,Telescope* Dy,int Nx,int Ny,TVector3 Vint);
  TVector3 VertexPosition(TVector3 V1,TVector3 V2,TVector3 V3,TVector3 V4);
  void InitMemory();
  void InLabFrame();
  void InReactionCM();
  void InProjectileFrame();
  void InitParticlesInOutputs();
  void CheckInOutAZ();
private:

  virtual void SetParContainers();
  
  ClassDef(ERHe8Analyzer,1)
};

#endif
