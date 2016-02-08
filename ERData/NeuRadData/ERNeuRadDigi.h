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
   *@param digi_nr  Index of digi in collection
   *@param frontTDC Front Time to Digi Converter value [ns]
   *@param backTDC  Back Time to Digi Converter value [ns]
   *@param TDC      Time to Digi Converter value [ns]
   *@param frontQDC Front Charge to Digi Converter value [MeV]
   *@param backQDC  Back Charge to Digi Converter value [MeV]
   *@param QDC      Charge to Digi Converter value [MeV]
   *@param fiber_nr fiber in bundle number
   **/
  ERNeuRadDigi(Int_t digi_nr, Double_t frontTDC, Double_t backTDC, Double_t TDC,
                   Double_t frontQDC, Double_t backQDC, Double_t QDC, Int_t fiber_nr);
                   
  /** Copy constructor **/
  ERNeuRadDigi(const ERNeuRadDigi&);

  /** Destructor **/
  virtual ~ERNeuRadDigi();

  ERNeuRadDigi& operator=(const ERNeuRadDigi&) { return *this; }

  /** Output to screen **/
  virtual void Print(const Option_t* opt = 0) const;



  void SetTDC(Double_t time){fTDC = time;}
  Double_t GetTDC(){return fTDC;}

  void SetFrontTDC(Double_t time){fFrontTDC = time;}
  Double_t GetFrontTDC(){return fFrontTDC;}
  
  void SetBackTDC(Double_t time){fBackTDC = time;}
  Double_t GetBackTDC() const {return fBackTDC;}

  void SetQDC(Double_t charge){fQDC = charge;}
  Double_t GetQDC(){return fQDC;}

  void SetFrontQDC(Double_t charge){fFrontQDC = charge;}
  Double_t GetFrontQDC(){return fFrontQDC;}

  void SetBackQDC(Double_t charge){fBackQDC = charge;}
  Double_t GetBackQDC(){return fBackQDC;}

  void SetDigiNr(Int_t digiNr){fDigi_nr = digiNr;}
  Int_t GetDigiNr(){return fDigi_nr;}
  
  void SetFiberNr(Int_t fiberNr){fFiber_nr = fiberNr;}
  Int_t GetFiberNr(){return fFiber_nr;}
  
 protected:

  Int_t fDigi_nr;
  Double32_t fFrontTDC;
  Double32_t fBackTDC;
  Double32_t fTDC;
  Double32_t fFrontQDC;
  Double32_t fBackQDC;
  Double32_t fQDC;
  Int_t fFiber_nr;

  ClassDef(ERNeuRadDigi,1)

};

#endif
