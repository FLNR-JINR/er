// -------------------------------------------------------------------------
// -----                      ERBeamDetMWPCDigi header file             -----
// -----                  	  Created  by                -----
// -------------------------------------------------------------------------

#ifndef ERBeamDetMWPCDigi_H
#define ERBeamDetMWPCDigi_H


#include "TObject.h"
#include "TVector3.h"

#include "FairMultiLinkedData.h"

class ERBeamDetMWPCDigi : public FairMultiLinkedData
{
 public:
  /** Default constructor **/
  ERBeamDetMWPCDigi();
  
  /** Constructor with arguments
   **/
  ERBeamDetMWPCDigi(Int_t id, Float_t Edep, Double_t time, Int_t mwpcNb, Int_t planeNb, Int_t wireNb);
                   
  /** Copy constructor **/
  ERBeamDetMWPCDigi(const ERBeamDetMWPCDigi&);

  /** Destructor **/
  virtual ~ERBeamDetMWPCDigi();

  ERBeamDetMWPCDigi& operator=(const ERBeamDetMWPCDigi&) { return *this; }

  /** Output to screen **/
  virtual void Print(const Option_t* opt = 0) const;

  /* Accessors */
  Int_t   ID ()        const {return fID;}
  Float_t GetEdep()       const {return fEdep;}
  Double_t GetTime()       const {return fTime;}
  Int_t   GetMWPCNb()  const { return fMWPCNb;}
  Int_t   GetPlaneNb() const { return fPlaneNb;}
  Int_t   GetWireNb()  const { return fWireNb;}

 protected:
  Int_t 	  fID;

  Int_t		  fMWPCNb;
  Int_t     fPlaneNb;
  Int_t     fWireNb;
  Double_t 	fTime;
  Float_t 	fEdep;
  
  ClassDef(ERBeamDetMWPCDigi,1)

};

#endif
