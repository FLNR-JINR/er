// -------------------------------------------------------------------------
// -----                        ERmuSiVertexFinder header file          -----
// -----                  Created 03/16  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERMUSIVERTEXFINDER_H
#define ERMUSIVERTEXFINDER_H

#include "TClonesArray.h"

#include "FairTask.h"

#include "ERmuSiHit.h"

class ERmuSiVertexFinder : public FairTask {

public:
  /** Default constructor **/
  ERmuSiVertexFinder();

  /** Constructor 
  ** verbose: 1 - only standard log print
  **/
  ERmuSiVertexFinder(Int_t verbose);

  /** Destructor **/
  ~ERmuSiVertexFinder();

  /** Virtual method Init **/
  virtual InitStatus Init();

  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

  /** Virtual method Finish **/
  virtual void Finish();

  /** Virtual method Reset **/
  virtual void Reset();
  
  /** Modifiers **/
  //void SetAngleCut(Double_t angle){fAngleCut = angle;}
  /** Accessors **/ 
protected:
  //Input arrays
  TClonesArray *fmuSiTracks;

  //Output arrays
  
protected:

private:
  //Double_t fAngleCut;

  virtual void SetParContainers();
  
  ClassDef(ERmuSiVertexFinder,1)
};

#endif
