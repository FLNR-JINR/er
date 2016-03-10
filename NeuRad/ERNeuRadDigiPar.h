// -------------------------------------------------------------------------
// -----                  ERNeuRadDigiPar header file                 -----
// -----                  Created 03/15  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERNEURADDIGIPAR_H
#define ERNEURADDIGIPAR_H


#include "FairParGenericSet.h"

#include "TObject.h"
#include "TObjArray.h"
#include "TArrayF.h"
#include "TArrayI.h"

class FairParIo;
class FairParamList;


class ERNeuRadDigiPar : public FairParGenericSet
{

  public:

    /** Standard constructor **/
    ERNeuRadDigiPar(const char* name    = "ERNeuRadDigiPar",
                            const char* title   = "ERNeuRad Digitization Parameters",
                            const char* context = "Default");


    /** Destructor **/
    virtual ~ERNeuRadDigiPar();


    /** Initialisation from input device**/
    virtual Bool_t init(FairParIo* input);


    /** Output to file **/
    //virtual Int_t write(FairParIo* output);

    virtual void print();
    
    /** Reset all parameters **/
    virtual void clear();

    void putParams(FairParamList*);
    Bool_t getParams(FairParamList*);
    
    /** Accessories  **/
    Int_t   NofFibers()     const {return fNofFibers;}
    Int_t   NofBundles()    const {return fNofBundles;}
	Float_t FiberWidth()	const {return fFiberWidth;}
    Float_t FiberLength() 	const {return fFiberLength;}
    //@todo добавить обработку выхода зза границу
    Double_t PMTQuantumEfficiency (Int_t iFiber) const {return (*fPMTQuantumEfficiency)[iFiber];}
    Double_t PMTGain (Int_t iFiber) const {return (*fPMTGain)[iFiber];}
  private:
    Float_t fFiberLength;
	Float_t fFiberWidth;
    TArrayF* fPMTQuantumEfficiency;
    TArrayF* fPMTGain;
    Int_t fNofFibers;
    Int_t fNofBundles;

    ERNeuRadDigiPar(const ERNeuRadDigiPar&);
    ERNeuRadDigiPar& operator=(const ERNeuRadDigiPar&);

    ClassDef(ERNeuRadDigiPar,1);
};


#endif /**  ERNEURADDIGIPAR_H   **/
