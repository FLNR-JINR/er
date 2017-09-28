// -------------------------------------------------------------------------
// -----                  ERBeamDetReconstructor header file          -----
// -----                  Created 04/17  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERBeamDetReconstructor_H
#define ERBeamDetReconstructor_H

#include "TRandom.h"

#include "FairTask.h"


#include "ERBeamDetEvent.h"
#include "ERBeamDetCalEvent.h"
#include "ERBeamDetRecoEvent.h"
#include "ERHe8EventHeader.h"

#define slight 29.9792458 /* light speed (cm/nsec) */

class ERBeamDetReconstructor : public FairTask {

public:
  /** Default constructor **/
  ERBeamDetReconstructor();

  /** Constructor 
  ** verbose: 1 - only standard log print
  **/
  ERBeamDetReconstructor(Int_t verbose);

  /** Destructor **/
  ~ERBeamDetReconstructor();

  /** Virtual method Init **/
  virtual InitStatus Init();

  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

  /** Virtual method Finish **/
  virtual void Finish();

  /** Virtual method Reset **/
  virtual void Reset();

  void SetTofOffset(Float_t tofOffset){fTofOffset = tofOffset;}

protected:
  //Input objects
  ERBeamDetEvent* fInEvent;
  ERBeamDetCalEvent* fInCalEvent;

  //Output object
  ERBeamDetRecoEvent* fOutEvent;

  void Tof();
  void MWPC();
  double coordMW(UpstreamMatter* pT,ERBeamDetRecoEvent* pR,char* MWid,char* XY);
  int mcluMW(int mMW,int* nMW);
  void DefineBeamEnergy();
  void ElossTOFaMWPCaTarget();
  double Stepantsov(char* D,int Z,double A,double I);
  void ReadDeDx();
  
  double UpstreamEnergyLoss(UpstreamMatter* pU,ERParticle* pP,bool Cond1, bool Cond2,char* Show);
  Int_t fEvent;

  float Tb; // temp
  // релятивистский гамма-фактор.
  double beta_b,gamma_b,p_beam;

  int i_flag_MW;

  /*
  Это единичные (? так ли это?) радиус-вектора (обычные, идущие из начала
  отсчета не лоренц) которые показывают направления полета иона из пучка
  (Vbeam); направление на точку пересечения ионом дальней от мишени
  станции MWPC (far -VmwFa ) и ближней станции MWPc(close-VmwCl):
  */
  TVector3 Vbeam;
  TVector3 VmwFa,VmwCl;

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
  int good_mtrack = 0;

  int good_mbeam = 0;
  // вспомогательные переменные для кин энергии
  double Tp1,Tp2,Tp3;

  double tof_0;
  //кинетическая энергия target + projectile в центре масс.
  double t_cm;
  // энергетический эффект реакции - сколько энергии высвобождается, если "+", сколько поглощается, если "-"
  double Qreaction,Tboutput,BeamSpread;
  //t_cm0 = t_cm+Qreaction это кинетическая энергия, доступная продуктам реакции в ЦМ.
  double t_cm0;
  //кинетическая энергия налетающей частицы (beam)

  ERHe8EventHeader* header;

  /*
  Это некие расстояния, нужные при определении точки входа налетающего иона в мишень. M - measured P-played
  */
  double MdistX,MdistY,MdistZ;

  Float_t fTofOffset;
private:

  virtual void SetParContainers();
  
  ClassDef(ERBeamDetReconstructor,1)
};

#endif
