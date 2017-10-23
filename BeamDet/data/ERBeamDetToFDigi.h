// -------------------------------------------------------------------------
// -----                      ERBeamDetToFDigi header file             -----
// -----                      Created   by               -----
// -------------------------------------------------------------------------

#ifndef ERBeamDetToFDigi_H
#define ERBeamDetToFDigi_H


#include "TObject.h"
#include "TVector3.h"

#include "FairMultiLinkedData.h"

class ERBeamDetToFDigi : public FairMultiLinkedData
{
 public:
  /** Default constructor **/
  ERBeamDetToFDigi();
  
  /** Constructor with arguments
   **/
  ERBeamDetToFDigi(Int_t id, Float_t Edep, Double_t time, Int_t tofNb);
                   
  /** Copy constructor **/
  ERBeamDetToFDigi(const ERBeamDetToFDigi&);

  /** Destructor **/
  virtual ~ERBeamDetToFDigi();

  ERBeamDetToFDigi& operator=(const ERBeamDetToFDigi&) { return *this; }

  /** Output to screen **/
  virtual void Print(const Option_t* opt = 0) const;

  /* Accessors */
  Int_t    ID () const {return fID;}
  Float_t  Edep() const {return fEdep;}
  Double_t GetTime() const {return fTime;}
  Int_t    GetToFNb() const {return fToFNb;}
  
 protected:
  Int_t     fID;

  Int_t     fToFNb;
  Double_t  fTime;
  Float_t   fEdep;
  
  ClassDef(ERBeamDetToFDigi,1)

};

#endif
