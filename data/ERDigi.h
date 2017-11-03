// -------------------------------------------------------------------------
// -----                      ERDigi header file             -----
// -----                      Created   by               -----
// -------------------------------------------------------------------------

#ifndef ERDigi_H
#define ERDigi_H


#include "TObject.h"
#include "TVector3.h"

#include "FairMultiLinkedData.h"

class ERDigi : public FairMultiLinkedData
{
 public:
  /** Default constructor **/
  ERDigi();
  
  /** Constructor with arguments
   **/
  ERDigi(Int_t id, Float_t Edep, Double_t time, Int_t volNb);
                   
  /** Copy constructor **/
  ERDigi(const ERDigi&);

  /** Destructor **/
  virtual ~ERDigi();

  ERDigi& operator=(const ERDigi&) { return *this; }

  /** Output to screen **/
  virtual void Print(const Option_t* opt = 0) const;

  /* Accessors */
  Int_t ID () const {return fID;}
  Float_t Edep() const {return fEdep;}
  
 protected:
  Int_t     fID;

  Int_t     fVolNb;
  Double_t  fTime;
  Float_t   fEdep;
  
  ClassDef(ERDigi,1)

};

#endif