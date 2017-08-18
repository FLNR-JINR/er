// -------------------------------------------------------------------------
// -----                      ERRTelescopeSiDigi header file             -----
// -----                      Created   by               -----
// -------------------------------------------------------------------------

#ifndef ERRTelescopeSiDigi_H
#define ERRTelescopeSiDigi_H


#include "TObject.h"
#include "TVector3.h"

#include "FairMultiLinkedData.h"

class ERRTelescopeSiDigi : public FairMultiLinkedData
{
 public:
  /** Default constructor **/
  ERRTelescopeSiDigi();

  /** Constructor with arguments
   **/
  ERRTelescopeSiDigi(Int_t id, Float_t Edep, Double_t time, Int_t sectorNb, Int_t sensorNb, Int_t side);

  /** Copy constructor **/
  ERRTelescopeSiDigi(const ERRTelescopeSiDigi&);

  /** Destructor **/
  virtual ~ERRTelescopeSiDigi();

  ERRTelescopeSiDigi& operator=(const ERRTelescopeSiDigi&) { return *this; }

  /** Output to screen **/
  virtual void Print(const Option_t* opt = 0) const;

  /* Accessors */
  Int_t ID () const {return fID;}
  Float_t Edep() const {return fEdep;}

 protected:
  Int_t     fID;

  Int_t     fSide;      //  Side = 0 -- sensor ; Side = 1 -- sector ;
  Int_t     fSensorNb;
  Int_t     fSectorNb;
  Double_t  fTime;
  Float_t   fEdep;

  ClassDef(ERRTelescopeSiDigi,1)

};

#endif
