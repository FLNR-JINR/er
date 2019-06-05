// -------------------------------------------------------------------------
// -----                  ERTelescopeReconstructor header file          -----
// -----                  Created 04/17  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERTelescopeReconstructor_H
#define ERTelescopeReconstructor_H

#include "TRandom.h"
#include "TVector3.h"
#include "TGeoManager.h"
#include "TGeoNode.h"
#include "TGeoCompositeShape.h"

#include "FairTask.h"

#include "ERQTelescopeCalEvent.h"
#include "ERRTelescopeCalEvent.h"
#include "ERHe8EventHeader.h"
#include "ERParticle.h"
#include "ERBeamDetRecoEvent.h"
#include "ERTelescopeRecoEvent.h"

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

class DownstreamMatter
{
public:
  double ej_si[10][105];
  double ej_csi[10][105];
  double ej_TARwin[10][105];
  double ej_target[10][105];
  double ej_heatscreen[10][105];
};

class ERTelescopeReconstructor : public FairTask {

public:
  /** Default constructor **/
  ERTelescopeReconstructor();

  /** Constructor 
  ** verbose: 1 - only standard log print
  **/
  ERTelescopeReconstructor(Int_t verbose);

  /** Destructor **/
  ~ERTelescopeReconstructor();

  /** Virtual method Init **/
  virtual InitStatus Init();

  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

  /** Virtual method Finish **/
  virtual void Finish();

  /** Virtual method Reset **/
  virtual void Reset();

protected:
  //Input objects
  ERQTelescopeCalEvent* fQTelescopeEvent;
  ERRTelescopeCalEvent* fRTelescopeEvent;
  ERBeamDetRecoEvent* fBeamDetEvent;

  //Output objects
  ERTelescopeRecoEvent* fOutEvent;

  Int_t fEvent;

  ERHe8EventHeader* header;

  Telescope*** Det;
  int* layer;
  int** NDet;

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


  TGeoManager *geom;
  TGeoNode *trajectory;

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
  int* mp;
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

  ERParticle*** ejectile;
  ERParticle* projectile;
  ERParticle* target;
  ERParticle* CM0;

  DownstreamMatter** EjMat;
  TVector3** AngleDet;

  TVector3 Vert1,Vert2;

  void ReadTelescopeParameters();
  void CreateTelescopeGeometry();
  void InitMemory();
  void InitParticlesInOutputs();
  void WhatParticlesInOut(ERParticle* ptr,char* str,int N);
  void ReadDeDx();
  void ReadDeposites();
  TVector3 Traject(Telescope* Dx,Telescope* Dy,int Nx,int Ny,TVector3 Vint);
  void misCalculations();
private:

  virtual void SetParContainers();
  
  ClassDef(ERTelescopeReconstructor,1)
};

#endif
