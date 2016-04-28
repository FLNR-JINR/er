// -------------------------------------------------------------------------
// -----                      ERNeuRadDigi header file             -----
// -----                  Created 03/15  by V. Schetinin               -----
// -------------------------------------------------------------------------

#ifndef ERNeuRadDigi_H
#define ERNeuRadDigi_H


#include "TObject.h"
#include "TVector3.h"

#include "FairMultiLinkedData.h"

class ERNeuRadDigi : public FairMultiLinkedData
{
 public:
  /** Default constructor **/
  ERNeuRadDigi();
  
  /** Constructor with arguments
   *@param id  Index of digi in collection
   *@param frontTDC Front Time to Digi Converter value [ns]
   *@param backTDC  Back Time to Digi Converter value [ns]
   *@param TDC      Time to Digi Converter value [ns]
   *@param QDC      Charge to Digi Converter value [MeV]
   *@param FinberIndex fiber in bundle number
   *@param BundleIndex bundle number
   **/
  ERNeuRadDigi(Int_t id, Double_t frontTDC, Double_t backTDC, Double_t TDC,
                  Double_t QDC, Int_t bundle, Int_t fiber, Int_t side);
                   
  /** Copy constructor **/
  ERNeuRadDigi(const ERNeuRadDigi&);

  /** Destructor **/
  virtual ~ERNeuRadDigi();

  ERNeuRadDigi& operator=(const ERNeuRadDigi&) { return *this; }

  /** Output to screen **/
  virtual void Print(const Option_t* opt = 0) const;

  void SetTDC(Double_t time){fTDC = time;}
  Double_t TDC(){return fTDC;}

  void SetFrontTDC(Double_t time){fFrontTDC = time;}
  Double_t FrontTDC(){return fFrontTDC;}
  
  void SetBackTDC(Double_t time){fBackTDC = time;}
  Double_t BackTDC() const {return fBackTDC;}

  void SetQDC(Double_t charge){fQDC = charge;}
  Double_t QDC(){return fQDC;}

  void SetID(Int_t id){fID = id;}
  Int_t ID(){return fID;}
  
  void SetFiberIndex(Int_t fiber){fFiberIndex = fiber;}
  Int_t FiberIndex(){return fFiberIndex;}

  void SetBundleIndex(Int_t bundle){fBundleIndex = bundle;}
  Int_t BundleIndex(){return fBundleIndex;}

  Int_t Side() {return fSide;}
  
 protected:
  Int_t fID;
  Double32_t fFrontTDC;
  Double32_t fBackTDC;
  Double32_t fTDC;
  Double32_t fQDC;
  Int_t fFiberIndex;
  Int_t fBundleIndex;
  Int_t fSide; //0 - front, 1 - back

  ClassDef(ERNeuRadDigi,1)

};

#endif
