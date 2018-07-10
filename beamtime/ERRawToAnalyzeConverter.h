/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERRawToAnalyzeConverter_H
#define ERRawToAnalyzeConverter_H

#include "FairTask.h"

#include "ERNeuRadRawEvent.h"
#include "ERNeuRadAEvent.h"

class FairTask;

class ERNeuRadRawEvent;
class ERNeuRadAEvent;

/** @class ERRawToAnalyzeConverter
 ** @brief task for analyzing raw data
 ** @author V.Chudoba <chudoba@jinr.ru>
 ** @version 1.0
**/

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
  ERNeuRadRawEvent** fRawEvents;

  //Output arrays
  ERNeuRadAEvent** fAEvents;

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
