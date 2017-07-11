// -------------------------------------------------------------------------
// -----                      ERBeamDetTOFDigi header file             -----
// -----                      Created   by               -----
// -------------------------------------------------------------------------

#ifndef ERBeamDetTOFDigi_H
#define ERBeamDetTOFDigi_H


#include "TObject.h"
#include "TVector3.h"

#include "FairMultiLinkedData.h"

class ERBeamDetTOFDigi : public FairMultiLinkedData
{
 public:
  /** Default constructor **/
  ERBeamDetTOFDigi();
  
  /** Constructor with arguments
   **/
  ERBeamDetTOFDigi(Int_t id, Float_t Edep, Double_t time, Int_t tofNb);
                   
  /** Copy constructor **/
  ERBeamDetTOFDigi(const ERBeamDetTOFDigi&);

  /** Destructor **/
  virtual ~ERBeamDetTOFDigi();

  ERBeamDetTOFDigi& operator=(const ERBeamDetTOFDigi&) { return *this; }

  /** Output to screen **/
  virtual void Print(const Option_t* opt = 0) const;

  /* Accessors */
  Int_t ID () const {return fID;}
  Float_t Edep() const {return fEdep;}
  
 protected:
  Int_t     fID;

  Int_t     fTOFNb;
  Double_t  fTime;
  Float_t   fEdep;
  
  ClassDef(ERBeamDetTOFDigi,1)

};

#endif
