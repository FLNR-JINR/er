// -------------------------------------------------------------------------
// -----                        ERmuSiTrackFinder header file          -----
// -----                  Created 03/16  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERMUSITRACKFINDER_H
#define ERMUSITRACKFINDER_H

#include "TClonesArray.h"

#include "FairTask.h"

#include "ERmuSiHit.h"

class ERmuSiTrackFinder : public FairTask {

public:
  /** Default constructor **/
  ERmuSiTrackFinder();

  /** Constructor 
  ** verbose: 1 - only standard log print
  **/
  ERmuSiTrackFinder(Int_t verbose);

  /** Destructor **/
  ~ERmuSiTrackFinder();

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
  TClonesArray *fmuSiHits;

  //Output arrays
  TClonesArray *fmuSiTracks;
protected:

private:
  virtual void SetParContainers();
  
  ClassDef(ERmuSiTrackFinder,1)
};

struct ERmuSiSinglet {
  ERmuSiHit* first;
  ERmuSiHit* second;
};

#endif
