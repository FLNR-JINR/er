// -------------------------------------------------------------------------
// -----                        ERBeamDetTrackFinder header file          -----
// -----                        Created   by                 -----
// -------------------------------------------------------------------------

#ifndef ERBeamDetTrackFinder_H
#define ERBeamDetTrackFinder_H

#include "TClonesArray.h"

#include "FairTask.h"

#include "ERBeamDetMWPCDigi.h"
#include "ERBeamDetTrack.h"
#include "ERBeamDetSetup.h"

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
  /** Accessors **/ 
protected:
  //Paramaters
  ERBeamDetSetup *fBeamDetSetup;
  
  //Input arrays
  TClonesArray   *fBeamDetMWPCDigiX1;
  TClonesArray   *fBeamDetMWPCDigiX2;
  TClonesArray   *fBeamDetMWPCDigiY1;
  TClonesArray   *fBeamDetMWPCDigiY2;

  //Output arrays
  ERBeamDetTrack *fBeamDetTrack ;

private:
  virtual void SetParContainers();
  ERBeamDetTrack* AddTrack(Double_t xt, Double_t yt, Double_t zt, TVector3 v);

  ClassDef(ERBeamDetTrackFinder,1)
};
#endif
