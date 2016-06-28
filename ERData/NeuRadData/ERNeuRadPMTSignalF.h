// -------------------------------------------------------------------------
// -----                      ERNeuRadPMTSignalF header file            -----
// -----                  Created 02/03/15  by v. Schetinin            -----
// -------------------------------------------------------------------------


#ifndef ERNeuRadPMTSignalF_H
#define ERNeuRadPMTSignalF_H


#include "TObject.h"
#include "TVector3.h"
#include "TArrayF.h"

#include <list>
#include <vector>

#include "FairMultiLinkedData.h"

#include "ERNeuRadFiberPoint.h"
#include "ERNeuRadPMTSignal.h"

class ERNeuRadPMTSignalF : public ERNeuRadPMTSignal
{
 protected:
    Double_t *fAmplitudes;
    Double_t *fAnodeTimes;
    Int_t fCurFPoint;
    Double_t fStartTime;
    Double_t fFinishTime;
    Int_t *fTimeShifts;
    Int_t fFPointsCount;
    Float_t* fResFunction;
    TArrayF fResFunctionRoot;
    Int_t fPECount;
    //constants
    static const Double_t csdT; //ns
    static const Int_t csdTCount; //count

    Double_t Function(Double_t time, Double_t amplitude);
 public:

  /** Default constructor **/
  ERNeuRadPMTSignalF();

  /** Constructor with arguments **/
  ERNeuRadPMTSignalF(Int_t iBundle, Int_t iFiber, Int_t fpoints_count);

  virtual ~ERNeuRadPMTSignalF();
  
  virtual void AddFiberPoint(ERNeuRadFiberPoint* fpoint);

  virtual void Generate();
  
  //virtual bool Exist(){return fFiberPoints.size() > 0;}

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
  ClassDef(ERNeuRadPMTSignalF,1);
};

#endif
