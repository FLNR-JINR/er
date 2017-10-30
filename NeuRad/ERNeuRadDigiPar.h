// -------------------------------------------------------------------------
// -----                  ERNeuRadDigiPar header file                  -----
// -----                  Created 03/15  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERNEURADDIGIPAR_H
#define ERNEURADDIGIPAR_H

#include "FairParGenericSet.h"

#include "TObject.h"
#include "TObjArray.h"
#include "TArrayF.h"
#include "TArrayI.h"

#include "ERNeuRadSetup.h"

class FairParIo;
class FairParamList;
class ERNeuRadSetup;

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

    /** Modifiers**/
    
    /** Accessories  **/
    Int_t   NofFibers()     const {return fNofFibers;}
    Int_t   NofModules()    const {return fNofModules;}
    //TODO добавить обработку выхода зза границу
    Double_t PixelQuantumEfficiency (Int_t iFiber) const {return (*fPixelQuantumEfficiency)[iFiber];}
    Double_t PixelGain (Int_t iFiber) const {return (*fPixelGain)[iFiber];}
    Double_t PixelSigma (Int_t iFiber) const {return (*fPixelSigma)[iFiber];}
    Bool_t UseCrosstalks() const {return fUseCrosstalks;}
    void Crosstalks(Int_t iFiber, TArrayF& crosstalks) const;
    Int_t RowNofFibers() const {return fRowNofFibers;}

private:
    
    TArrayF* fPixelQuantumEfficiency;
    TArrayF* fPixelGain;
    TArrayF* fPixelSigma;
    TArrayF* fPixelCrosstalks;
    TArrayF* fFiberCrosstalks;
    Int_t fNofFibers;
    Int_t fNofModules;
    Bool_t fUseCrosstalks;
    Int_t fRowNofFibers;

    ERNeuRadDigiPar(const ERNeuRadDigiPar&);
    ERNeuRadDigiPar& operator=(const ERNeuRadDigiPar&);

    ClassDef(ERNeuRadDigiPar,1)
};

#endif // ERNEURADDIGIPAR_H
