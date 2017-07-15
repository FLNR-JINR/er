// -------------------------------------------------------------------------
// -----                      ERRTelescopeDigi header file             -----
// -----                      Created   by               -----
// -------------------------------------------------------------------------

#ifndef ERRTelescopeDigi_H
#define ERRTelescopeDigi_H


#include "TObject.h"
#include "TVector3.h"

#include "FairMultiLinkedData.h"

class ERRTelescopeDigi : public FairMultiLinkedData
{
 public:
  /** Default constructor **/
  ERRTelescopeDigi();
  
  /** Constructor with arguments
   **/
  ERRTelescopeDigi(Int_t id, Float_t Edep, Double_t time, Int_t sensorNb, Int_t sectorNb);
                   
  /** Copy constructor **/
  ERRTelescopeDigi(const ERRTelescopeDigi&);

  /** Destructor **/
  virtual ~ERRTelescopeDigi();

  ERRTelescopeDigi& operator=(const ERRTelescopeDigi&) { return *this; }

  /** Output to screen **/
  virtual void Print(const Option_t* opt = 0) const;

  /* Accessors */
  Int_t ID () const {return fID;}
  Float_t Edep() const {return fEdep;}
  
 protected:
  Int_t     fID;

  Int_t     fSensorNb;
  Int_t     fSectorNb;
  Double_t  fTime;
  Float_t   fEdep;
  
  ClassDef(ERRTelescopeDigi,1)

};

#endif