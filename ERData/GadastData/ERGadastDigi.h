// -------------------------------------------------------------------------
// -----                      ERGadastDigi header file             -----
// -----                  Created 03/15  by V. Schetinin               -----
// -------------------------------------------------------------------------

#ifndef ERGadastDigi_H
#define ERGadastDigi_H


#include "TObject.h"
#include "TVector3.h"

#include "FairMultiLinkedData.h"

class ERGadastDigi : public FairMultiLinkedData
{
 public:
  /** Default constructor **/
  ERGadastDigi();
  
  /** Constructor with arguments
   **/
  ERGadastDigi(Int_t id, Float_t Edep);
                   
  /** Copy constructor **/
  ERGadastDigi(const ERGadastDigi&);

  /** Destructor **/
  virtual ~ERGadastDigi();

  ERGadastDigi& operator=(const ERGadastDigi&) { return *this; }

  /** Output to screen **/
  virtual void Print(const Option_t* opt = 0) const;

  /* Accessors */
  Int_t ID () const {return fID;}
  Float_t Edep() const {return fEdep;}
  
 protected:
  Int_t fID;
  Float_t fEdep;
  
  ClassDef(ERGadastDigi,1)

};

#endif
