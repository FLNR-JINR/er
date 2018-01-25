// -------------------------------------------------------------------------
// -----                      ERGadastLaBrDigi header file             -----
// -----                  Created 03/15  by V. Schetinin               -----
// -------------------------------------------------------------------------

#ifndef ERGadastLaBrDigi_H
#define ERGadastLaBrDigi_H


#include "TObject.h"
#include "TVector3.h"

#include "FairMultiLinkedData.h"

class ERGadastLaBrDigi : public FairMultiLinkedData
{
 public:
  /** Default constructor **/
  ERGadastLaBrDigi();
  
  /** Constructor with arguments
   **/
  ERGadastLaBrDigi(Int_t id, Float_t Edep, Int_t cell);
                   
  /** Copy constructor **/
  ERGadastLaBrDigi(const ERGadastLaBrDigi&);

  /** Destructor **/
  virtual ~ERGadastLaBrDigi();

  ERGadastLaBrDigi& operator=(const ERGadastLaBrDigi&) { return *this; }

  /** Output to screen **/
  virtual void Print(const Option_t* opt = 0) const;

  /* Accessors */
  Int_t ID () const {return fID;}
  Float_t Edep() const {return fEdep;}
  
 protected:
  Int_t fID;
  Float_t fEdep;
  Int_t fCell;
  
  ClassDef(ERGadastLaBrDigi,1)
};

#endif
