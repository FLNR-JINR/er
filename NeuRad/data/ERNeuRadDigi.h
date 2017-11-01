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
   *@param id           Index of digi in collection
   *@param frontTDC     Front Time-to-Digital Converter value [ns]
   *@param backTDC      Back Time-to-Digital Converter value [ns]
   *@param tdc          Time-to-Digital Converter value [ns]
   *@param qdc          Charge to Digi Converter value [MeV]
   *@param bundle       Bundle number
   *@param fiber        Fiber in bundle number
   *@param side         ?????
   **/
  ERNeuRadDigi(Int_t id,
               Double_t frontTDC,
               Double_t backTDC,
               Double_t tdc,
               Double_t qdc,
               Int_t bundle,
               Int_t fiber,
               Int_t side);

  /** Copy constructor **/
  ERNeuRadDigi(const ERNeuRadDigi&);

  /** Destructor **/
  virtual ~ERNeuRadDigi();

  ERNeuRadDigi& operator=(const ERNeuRadDigi&) { return *this; }

  /** Output to screen **/
  virtual void Print(const Option_t* opt = 0) const;

  void SetID(Int_t id) {fID = id;}
  void SetFrontTDC(Double_t time) {fFrontTDC = time;}
  void SetBackTDC(Double_t time) {fBackTDC = time;}
  void SetTDC(Double_t time) {fTDC = time;}
  void SetQDC(Double_t charge) {fQDC = charge;}
  void SetModuleIndex(Int_t bundle) {fModuleIndex = bundle;}
  void SetFiberIndex(Int_t fiber) {fFiberIndex = fiber;}
  //TODO void SetSide()????

  Int_t ID() const {return fID;}
  Double_t FrontTDC() const {return fFrontTDC;}
  Double_t BackTDC() const {return fBackTDC;}
  Double_t TDC() const {return fTDC;}
  Double_t QDC() const {return fQDC;}
  Int_t ModuleIndex() const {return fModuleIndex;}
  Int_t FiberIndex() const {return fFiberIndex;}
  Int_t Side() const {return fSide;}

protected:

  Int_t fID;
  Double32_t fFrontTDC;
  Double32_t fBackTDC;
  Double32_t fTDC;
  Double32_t fQDC;
  Int_t fModuleIndex;
  Int_t fFiberIndex;
  Int_t fSide; //0 - front, 1 - back

  ClassDef(ERNeuRadDigi,1)
};

#endif // ERNeuRadDigi_H
