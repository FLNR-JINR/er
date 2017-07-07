// -------------------------------------------------------------------------
// -----                        ERLi10Reconstructor header file        -----
// -----                  Created 03/16  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERLi10Reconstructor_H
#define ERLi10Reconstructor_H

#include "TClonesArray.h"

#include "FairTask.h"
#include "ERLi10MCEventHeader.h"

class ERLi10Reconstructor : public FairTask {

public:
  /** Default constructor **/
  ERLi10Reconstructor();

  /** Constructor 
  ** verbose: 1 - only standard log print, 2 - print digi information 
  **/
  ERLi10Reconstructor(Int_t verbose);

  /** Destructor **/
  ~ERLi10Reconstructor();

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
  //Input arrays
  TClonesArray *fNDHits;
  TClonesArray *fRTelescopeHits;
  //Output arrays
  // /TClonesArray *fNDHits;

  ERLi10MCEventHeader* fMCHeader;
protected:
  /*ERNDHit* AddHit(Int_t detID, TVector3& pos, TVector3& dpos, 
          Int_t point_index, Float_t lightYield, Float_t time, Float_t neutronProb);*/
private:
  virtual void SetParContainers();
  
  ClassDef(ERLi10Reconstructor,1)
};

#endif
