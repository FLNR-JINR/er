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
    Int_t   NofModules()    const {return fNofModules;}
    //@todo добавить обработку выхода зза границу
    Double_t PMTQuantumEfficiency (Int_t iFiber) const {return (*fPMTQuantumEfficiency)[iFiber];}
    Double_t PMTGain (Int_t iFiber) const {return (*fPMTGain)[iFiber];}
    Double_t PMTSigma (Int_t iFiber) const {return (*fPMTSigma)[iFiber];}
    Bool_t UseCrosstalks() const {return fUseCrosstalks;}
    void PMTCrosstalks(Int_t iFiber, TArrayF& crosstalks) const;
    Int_t RowNofFibers() const {return fRowNofFibers;}
  private:
    TArrayF* fPMTQuantumEfficiency;
    TArrayF* fPMTGain;
    TArrayF* fPMTSigma;
    TArrayF* fPMTCrosstalks;
    Int_t fNofFibers;
    Int_t fNofModules;
    Bool_t fUseCrosstalks;
    Int_t fRowNofFibers;

    ERNeuRadDigiPar(const ERNeuRadDigiPar&);
    ERNeuRadDigiPar& operator=(const ERNeuRadDigiPar&);

    ClassDef(ERNeuRadDigiPar,1);
};


#endif /**  ERNEURADDIGIPAR_H   **/
