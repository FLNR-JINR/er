// -------------------------------------------------------------------------

// -----                        ERNeuRadSetup header file              -----

// -----                        V.Schetinin (sch_vitaliy@mail.ru)      -----

// -------------------------------------------------------------------------

#ifndef ERNEURADSETUP_H
#define ERNEURADSETUP_H

#include <vector>
#include "Rtypes.h"
#include "ERNeuRadDigiPar.h"

class ERNeuRadDigiPar;

struct ERNeuRadModule{
        Float_t fX;
        Float_t fY;
        ERNeuRadModule(Float_t x, Float_t y){fX = x; fY = y;}
};

struct ERNeuRadFiber{
        Float_t fX;
        Float_t fY;
        ERNeuRadFiber(Float_t x, Float_t y){fX = x; fY = y;}
};

class ERNeuRadSetup {
    static ERNeuRadSetup* fInstance;
    static ERNeuRadDigiPar* fDigiPar;
    static std::vector<ERNeuRadModule*> fModules;
    static std::vector<std::vector<ERNeuRadFiber*> > fFibers;
    static Float_t fZ;
    static Float_t fLength;
    static Float_t fFiberWidth;
    static Int_t fRowNofFibers;
    static Int_t fRowNofModules;
    static Int_t fRowNofPixels;
    ERNeuRadSetup();
public:
    static ERNeuRadSetup* Instance();
    static Int_t   NofFibers();
    static Int_t   NofPixels();
    static Int_t   NofModules();
    static Float_t FiberLength();
    static Float_t FiberWidth();
    static Float_t ModuleX(Int_t iModule);
    static Float_t ModuleY(Int_t iModule);
    static Float_t FiberX(Int_t iModule, Int_t iFiber);
    static Float_t FiberY(Int_t iModule, Int_t iFiber);
    static Float_t PMTQuantumEfficiency(Int_t iModule, Int_t iFiber);
    static Float_t PMTGain(Int_t iModule, Int_t iFiber);
    static Float_t PMTSigma(Int_t iModule, Int_t iFiber);
    static Float_t Z(){return fZ;}
    static void    Print();
    static void Crosstalks(Int_t iFiber, TArrayF& crosstalks);
    static Int_t RowNofFibers();
    static Int_t RowNofPixels();
    static Int_t RowNofModules();
    static Bool_t UseCrosstalks();
    static Int_t SetParContainers();
    ClassDef(ERNeuRadSetup,1)

};

#endif

