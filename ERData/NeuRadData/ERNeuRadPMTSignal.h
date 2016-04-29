// -------------------------------------------------------------------------
// -----                      ERNeuRadPMTSignal header file            -----
// -----                  Created 02/03/15  by v. Schetinin            -----
// -------------------------------------------------------------------------


#ifndef ERNeuRadPMTSignal_H
#define ERNeuRadPMTSignal_H


#include "TObject.h"
#include "TVector3.h"

#include <map>

#include "FairMultiLinkedData.h"

#include "ERNeuRadFiberPoint.h"

class ERNeuRadPMTSignal : public FairMultiLinkedData
{
 private:
  Int_t fFiberIndex;
  Int_t fBundleIndex;
 public:

  /** Default constructor **/
  ERNeuRadPMTSignal();

  /** Constructor with arguments **/
  ERNeuRadPMTSignal(Int_t iBundle, Int_t iFiber);

  virtual ~ERNeuRadPMTSignal();
  
  virtual void AddFiberPoint(ERNeuRadFiberPoint* fpoint) = 0;
  virtual void Generate() = 0;
  
  virtual bool Exist() = 0;

  Int_t FiberIndex() const {return fFiberIndex;}
  Int_t BundleIndex() const {return fBundleIndex;}
  
  virtual std::vector<Double_t> GetIntersections(Double_t discriminatorThreshold) = 0;

  virtual Double_t GetMaxInteg(const Double_t window, const Double_t dt) = 0;
  virtual Double_t GetInteg(const Double_t start,const Double_t finish) = 0;
  virtual Double_t GetFirstInteg(const Double_t window) = 0;
  virtual Double_t GetMean(const Double_t time) = 0;
  virtual Double_t GetStartTime() = 0;
  virtual Double_t GetFinishTime() = 0;

  virtual Double_t OnePEIntegral() = 0;
  virtual Int_t PECount() = 0;
  
  ClassDef(ERNeuRadPMTSignal,1);
};

#endif
