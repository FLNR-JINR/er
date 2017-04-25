// -------------------------------------------------------------------------
// -----                  ERTracker header file          -----
// -----                  Created 04/17  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERTracker_H
#define ERTracker_H

#include "TRandom.h"
#include "TVector3.h"

#include "FairTask.h"

#define pi 3.14159265358979323846
#define rad 0.01745329252 /* pi/180 */
#define slight 29.9792458 /* light speed (cm/nsec) */
#define hc 197.3425 /* Plank*c (MeV*fm) */

class Telescope
{
public:
  Telescope(){};
  ~Telescope(){}
  char Shape[12];
  char Matt[12];
  char StripFB[12];
  double Angle;
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
  int isi;
  int jsi;
  int icsi;
  int jcsi;
  bool si;
  bool csi;
  double pdx1,pdx2,pd1,pd3,pd31,pd4,pd5,pd6,pd7,pd8,pd9;
  int pd10,pd11;
  int NtelMax;
  char pd2[6],pd12[6],pd13[12];
  int* layer;
  int NlayMax;
  int NdivMax;
  int Ntelescopes;
  Telescope** Det;
  char DummyC[256];
  char Zeros[32];
  double ResolutionSi,ResolutionCsI;
  int Ndiv;

  void ReadTelescopeParameters();
  void CreateTelescopeGeometry();
private:

  virtual void SetParContainers();
  
  ClassDef(ERTracker,1)
};

#endif
