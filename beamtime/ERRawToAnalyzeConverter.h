// -------------------------------------------------------------------------
// -----                  ERRawToAnalyzeConverter header file          -----
// -----                  Created 03/16  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERRawToAnalyzeConverter_H
#define ERRawToAnalyzeConverter_H

#include "FairTask.h"

#include "RawEvent.h"
#include "AEvent.h"

class ERRawToAnalyzeConverter : public FairTask {

public:
  /** Default constructor **/
  ERRawToAnalyzeConverter();

  /** Constructor 
  ** verbose: 1 - only standard log print, 2 - print digi information 
  **/
  ERRawToAnalyzeConverter(Int_t verbose);

  /** Destructor **/
  ~ERRawToAnalyzeConverter();

  /** Virtual method Init **/
  virtual InitStatus Init();

  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

  /** Virtual method Finish **/
  virtual void Finish();

  /** Virtual method Reset **/
  virtual void Reset();

  void SetNChanels(Int_t chanels){fNChanels = chanels;}
  void SetNPoints(Int_t points){fNPoints = points;}
  void SetSmooth(Int_t smpar){ if (smpar == 1) fSmoothPar = kTRUE;}

protected:
  //Input objects
  RawEvent** fRawEvents;

  //Output arrays
  AEvent** fAEvents;

  Int_t fNChanels;
  Int_t fNPoints;
  Bool_t fSmoothPar;

  const Double_t fRatio;
  const Double_t fTD; //in ns
  Double_t fNoiseMin;
  Double_t fNoiseMax;
  Int_t fNumSmoothP;	//number of points to smooth

  Int_t fEvent;

private:
  virtual void SetParContainers();
  
  ClassDef(ERRawToAnalyzeConverter,1)
};

#endif
