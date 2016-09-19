// -------------------------------------------------------------------------
// -----                      ERNeuRadPMTSignal header file            -----
// -----                  Created 02/03/15  by v. Schetinin            -----
// -------------------------------------------------------------------------


#ifndef ERNeuRadPMTSignal_H
#define ERNeuRadPMTSignal_H


#include "TObject.h"
#include "TVector3.h"
#include "TArrayF.h"

#include <map>

#include "FairMultiLinkedData.h"

#include "ERNeuRadFiberPoint.h"

class ERNeuRadPMTSignal : public FairMultiLinkedData {

protected:
  Int_t fFiberIndex;
  Int_t fModuleIndex;
  Double_t *fAmplitudes;
  Double_t fAmplitudesSum;
  Double_t *fAnodeTimes;
  Int_t fCurFPoint;
  Double_t fStartTime;
  Double_t fFinishTime;
  Int_t *fTimeShifts;
  Int_t fFPointsCount;
  Float_t* fResFunction;
  TArrayF fResFunctionRoot;
  Int_t fPECount;
  Int_t fSide;
  //constants
  static const Double_t csdT; //ns
  static const Int_t csdTCount; //count

  Double_t Function(Double_t time, Double_t amplitude);
public:

  /** Default constructor **/
  ERNeuRadPMTSignal();

  /** Constructor with arguments **/
  ERNeuRadPMTSignal(Int_t iBundle, Int_t iFiber, Int_t fpoints_count, Int_t side);

  virtual ~ERNeuRadPMTSignal();
  
  virtual void AddFiberPoint(ERNeuRadFiberPoint* fpoint);

  virtual void Generate();

  virtual bool Exist(){return fCurFPoint > 0;}

  TArrayF* ResultSignal() {return &fResFunctionRoot;}

  //пока заглушки
  virtual std::vector<Double_t> GetIntersections(Double_t discriminatorThreshold);

  virtual Double_t GetMaxInteg(const Double_t window, const Double_t dt) {return -1.;}
  virtual Double_t GetInteg(const Double_t start,const Double_t finish);
  virtual Double_t GetFirstInteg(const Double_t window);
  virtual Double_t GetMean(const Double_t time) {return -1.;}

  virtual Double_t GetStartTime() {return fStartTime;} 
  virtual Double_t GetFinishTime() {return fFinishTime;}

  virtual Float_t GetThresholdTime(Float_t peThreshold);

  virtual Double_t OnePEIntegral() {return 4.8;}

  virtual Int_t PECount() {return fPECount;}

  Double_t  dT() {return csdT;}

  Int_t ModuleIndex() const {return fModuleIndex;}
  Int_t FiberIndex() const {return fFiberIndex;}
  Int_t Side() const {return fSide;}
  Double_t AmplitudesSum() const {return fAmplitudesSum;}

  ClassDef(ERNeuRadPMTSignal,1);
};

#endif
