// -------------------------------------------------------------------------
// -----                        ERmuSiMatcher header file              -----
// -----                  Created 03/16  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERMUSIMATCHER_H
#define ERMUSIMATCHER_H

#include "TClonesArray.h"

#include "FairTask.h"

class ERmuSiMatcher : public FairTask {

public:
  /** Default constructor **/
  ERmuSiMatcher();

  /** Constructor 
  ** verbose: 1 - only standard log print, 2 - print digi information 
  **/
  ERmuSiMatcher(Int_t verbose);

  /** Destructor **/
  ~ERmuSiMatcher();

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
  TClonesArray *fmuSiPoints;
  TClonesArray *fmuSiHits;
  TClonesArray *fmuSiTracks;

protected:
  
private:
  virtual void SetParContainers();
  
  ClassDef(ERmuSiMatcher,1)
};

#endif
