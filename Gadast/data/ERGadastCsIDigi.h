// -------------------------------------------------------------------------
// -----                      ERGadastCsIDigi header file             -----
// -----                  Created 03/15  by V. Schetinin               -----
// -------------------------------------------------------------------------

#ifndef ERGadastCsIDigi_H
#define ERGadastCsIDigi_H


#include "TObject.h"
#include "TVector3.h"

#include "FairMultiLinkedData.h"

class ERGadastCsIDigi : public FairMultiLinkedData
{
 public:
  /** Default constructor **/
  ERGadastCsIDigi();
  
  /** Constructor with arguments
   **/
  ERGadastCsIDigi(Int_t id, Float_t Edep, Int_t wall, Int_t block, Int_t cell);
                   
  /** Copy constructor **/
  ERGadastCsIDigi(const ERGadastCsIDigi&);

  /** Destructor **/
  virtual ~ERGadastCsIDigi();

  ERGadastCsIDigi& operator=(const ERGadastCsIDigi&) { return *this; }

  /** Output to screen **/
  virtual void Print(const Option_t* opt = 0) const;

  /* Accessors */
  Int_t ID () const {return fID;}
  Float_t Edep() const {return fEdep;}
  
 protected:
  Int_t fID;
  Float_t fEdep;
  Int_t fWall, fBlock, fCell;
  
  ClassDef(ERGadastCsIDigi,1)
};

#endif
