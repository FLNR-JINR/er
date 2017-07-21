// -------------------------------------------------------------------------
// -----                        ERBeamDetTrackFinder header file          -----
// -----                        Created   by                 -----
// -------------------------------------------------------------------------

#ifndef ERBeamDetTrackFinder_H
#define ERBeamDetTrackFinder_H

#include "TClonesArray.h"

#include "FairTask.h"

#include "ERBeamDetMWPCDigi.h"

class ERBeamDetTrackFinder : public FairTask {

public:
  /** Default constructor **/
  ERBeamDetTrackFinder();

  /** Constructor 
  ** verbose: 1 - only standard log print
  **/
  ERBeamDetTrackFinder(Int_t verbose);

  /** Destructor **/
  ~ERBeamDetTrackFinder();

  /** Virtual method Init **/
  virtual InitStatus Init();

  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

  /** Virtual method Finish **/
  virtual void Finish();

  /** Virtual method Reset **/
  virtual void Reset();
  
  /** Modifiers **/
  void SetAngleCut(Double_t angle){fAngleCut = angle;}
  /** Accessors **/ 
protected:
  //Input arrays
  TClonesArray *fBeamDetMWPCDigi;

  //Output arrays
  TClonesArray *fBeamDetTracks;


private:
  Double_t fAngleCut;

  virtual void SetParContainers();
  
  ClassDef(ERBeamDetTrackFinder,1)
};

struct ERmuSiSinglet {
  ERmuSiHit* first;
  ERmuSiHit* second;
};

#endif
