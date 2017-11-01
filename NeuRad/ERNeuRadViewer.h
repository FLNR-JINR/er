// -------------------------------------------------------------------------
// -----                  ERNeuRadViewer header file                   -----
// -----                  Created 03/16  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERNeuRadViewer_H
#define ERNeuRadViewer_H

#include "TClonesArray.h"
#include "TFile.h"

#include "FairTask.h"

class ERNeuRadViewer : public FairTask {

public:

  /** Default constructor **/
  ERNeuRadViewer();

  ERNeuRadViewer(Int_t verbose);

  /** Destructor **/
  ~ERNeuRadViewer();

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
  TClonesArray *fNeuRadPixelSignals;
  
  //Outputs
  TFile* fFile;

private:

  virtual void SetParContainers();
  
  ClassDef(ERNeuRadViewer,1)
};

#endif // ERNeuRadViewer_H
