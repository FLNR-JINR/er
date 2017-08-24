// -------------------------------------------------------------------------
// -----                      ERRTelescopeCsIDigi header file             -----
// -----                      Created   by               -----
// -------------------------------------------------------------------------

#ifndef ERRTelescopeCsIDigi_H
#define ERRTelescopeCsIDigi_H


#include "TObject.h"
#include "TVector3.h"

#include "FairMultiLinkedData.h"

class ERRTelescopeCsIDigi : public FairMultiLinkedData
{
 public:
  /** Default constructor **/
  ERRTelescopeCsIDigi();

  /** Constructor with arguments
   **/
  ERRTelescopeCsIDigi(Int_t id, Int_t telescopeNb, Float_t Edep, Double_t time, Int_t crystall);

  /** Copy constructor **/
  ERRTelescopeCsIDigi(const ERRTelescopeCsIDigi&);

  /** Destructor **/
  virtual ~ERRTelescopeCsIDigi();

  ERRTelescopeCsIDigi& operator=(const ERRTelescopeCsIDigi&) { return *this; }

  /** Output to screen **/
  virtual void Print(const Option_t* opt = 0) const;

  /* Accessors */
  Int_t ID () const {return fID;}
  Float_t Edep() const {return fEdep;}
  Int_t TelescopeNb () const {return fTelescopeNb;}
  Int_t CrystallNB () const {return fCrystallNb;}

 protected:
  Int_t     fID;
  Int_t     fTelescopeNb;
  Int_t     fCrystallNb;
  Double_t  fTime;
  Float_t   fEdep;

  ClassDef(ERRTelescopeCsIDigi,1)

};

#endif
