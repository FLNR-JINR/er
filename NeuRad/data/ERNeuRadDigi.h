// -------------------------------------------------------------------------
// -----                  ERNeuRadDigi header file                     -----
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
   *@param ModuleIndex bundle number
   **/
  ERNeuRadDigi(Int_t id, Double_t frontTDC, Double_t backTDC, Double_t tdc,
                  Double_t qdc, Int_t bundle, Int_t fiber, Int_t side);
                   
  /** Copy constructor **/
  ERNeuRadDigi(const ERNeuRadDigi&);

  /** Destructor **/
  virtual ~ERNeuRadDigi();

  ERNeuRadDigi& operator=(const ERNeuRadDigi&) { return *this; }

  /** Output to screen **/
  virtual void Print(const Option_t* opt = 0) const;

  void SetTDC(Double_t time) {fTDC = time;}
  void SetFrontTDC(Double_t time){fFrontTDC = time;}
  void SetBackTDC(Double_t time){fBackTDC = time;}
  void SetQDC(Double_t charge){fQDC = charge;}
  void SetID(Int_t id){fID = id;}
  void SetFiberIndex(Int_t fiber){fFiberIndex = fiber;}
  void SetModuleIndex(Int_t bundle){fModuleIndex = bundle;}

  Double_t TDC() const {return fTDC;}
  Double_t FrontTDC() const {return fFrontTDC;}
  Double_t BackTDC() const {return fBackTDC;}
  Double_t QDC() const {return fQDC;}
  Int_t ID() const {return fID;}
  Int_t FiberIndex() const {return fFiberIndex;}
  Int_t ModuleIndex() const {return fModuleIndex;}
  Int_t Side() const {return fSide;}
  
protected:

  Double32_t fTDC;
  Double32_t fFrontTDC;
  Double32_t fBackTDC;
  Double32_t fQDC;
  Int_t fID;
  Int_t fFiberIndex;
  Int_t fModuleIndex;
  Int_t fSide; //0 - front, 1 - back

  ClassDef(ERNeuRadDigi,1)
};

#endif // ERNeuRadDigi_H
