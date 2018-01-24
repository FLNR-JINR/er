// -------------------------------------------------------------------------
// -----                        ERGadastDigitizer header file          -----
// -----                  Created 03/15  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERGadastDigitizer_H
#define ERGadastDigitizer_H

#include "TRandom3.h"
#include "TStopwatch.h"
#include "TH1F.h"
#include "TClonesArray.h"

#include "FairTask.h"

#include "ERGadastCsIDigi.h"
#include "ERGadastLaBrDigi.h"
#include "ERGadastDigiPar.h"
#include "ERGadastSetup.h"

class TObjectArray;
class TH1F;
class TH2F;
class TF1;
class TRandom;

class ERGadastDigitizer : public FairTask {

public:
  /** Default constructor **/
  ERGadastDigitizer();

  /** Constructor 
  ** verbose: 1 - only standard log print, 2 - print digi information 
  **/
  ERGadastDigitizer(Int_t verbose);

  /** Destructor **/
  ~ERGadastDigitizer();

  /** Virtual method Init **/
  virtual InitStatus Init();

  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

  /** Virtual method Finish **/
  virtual void Finish();

  /** Virtual method Reset **/
  virtual void Reset();
  
  /** Modifiers **/
 
  /** Accessors **/ 

protected:
  //Digitization parameters
  ERGadastDigiPar* fDigiPar;
  //Input arrays
  TClonesArray *fGadastCsIPoints;
  TClonesArray *fGadastLaBrPoints;

  ERGadastSetup* fSetup;

  //Output arrays
  TClonesArray *fGadastCsIDigi;
  TClonesArray *fGadastLaBrDigi;
protected:
  
  ERGadastCsIDigi* AddCsIDigi(Float_t Edep,Int_t wall,Int_t block, Int_t cell);
  ERGadastLaBrDigi* AddLaBrDigi(Float_t Edep, Int_t cell);
  
private:
  virtual void SetParContainers();
  
  ClassDef(ERGadastDigitizer,1)
};

#endif
