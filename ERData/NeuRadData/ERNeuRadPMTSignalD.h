// -------------------------------------------------------------------------
// -----                      ERNeuRadPMTSignalD header file            -----
// -----                  Created 02/03/15  by v. Schetinin            -----
// -------------------------------------------------------------------------


#ifndef ERNeuRadPMTSignalD_H
#define ERNeuRadPMTSignalD_H


#include "TObject.h"
#include "TVector3.h"
#include "TArrayF.h"

#include <list>

#include "FairMultiLinkedData.h"

#include "ERNeuRadFiberPoint.h"
#include "ERNeuRadPMTSignal.h"

class ERNeuRadPMTSignalD : public ERNeuRadPMTSignal
{
 protected:
    //std::list<ERNeuRadFiberPoint*> fFiberPoints;
    Double_t *fAmplitudes;
    Double_t *fAnodeTimes;
    Int_t fCurFPoint;
    Double_t fStartTime;
    Double_t fFinishTime;
    Int_t *fTimeShifts;
    Int_t fFPointsCount;
    Float_t* fResFunction;
    TArrayF fResFunctionRoot;
    //constants
    static const Double_t csdT; //ns
    static const Int_t csdTCount; //count
 public:

  /** Default constructor **/
  ERNeuRadPMTSignalD();

  /** Constructor with arguments **/
  ERNeuRadPMTSignalD(Int_t iBundle, Int_t iFiber, Int_t fpoints_count);

  virtual ~ERNeuRadPMTSignalD();
  
  virtual void AddFiberPoint(ERNeuRadFiberPoint* fpoint);
  
  //virtual bool Exist(){return fFiberPoints.size() > 0;}

  virtual bool Exist(){return fCurFPoint > 0;}
  void Generate();
  Double_t Function(Double_t time);
  virtual Double_t GetStartTime() {return fStartTime;} 
  virtual Double_t GetFinishTime() {return fFinishTime;} 
  TArrayF* ResultSignal() {return &fResFunctionRoot;}
  Double_t  dT() {return csdT;}
  ClassDef(ERNeuRadPMTSignalD,1);
};

#endif
