// -------------------------------------------------------------------------

// -----                        ERBeamDetSetup header file              -----

// -----                              -----

// -------------------------------------------------------------------------
/*
#ifndef ERBeamDetSETUP_H
#define ERBeamDetSETUP_H

#include <vector>
#include "Rtypes.h"

struct ERBeamDetMWPC{
        Float_t fX;
        Float_t fY;
        Float_t fZ;
        ERBeamDetMWPC(Float_t x, Float_t y, Float_t z){fX = x; fY = y; fZ = z;}
};

struct ERBeamDetPlate{
        Float_t fX;
        Float_t fY;
        Float_t fZ;
        ERBeamDetPlate(Float_t x, Float_t y, Float_t z){fX = x; fY = y; fZ = z;}
};

struct ERBeamDetWire{
        Float_t fX;
        Float_t fY;
        Float_t fZ;
        ERBeamDetWire(Float_t x, Float_t y, Float_t z){fX = x; fY = y; fZ = z;}
};

class ERBeamDetSetup {
    static ERBeamDetSetup* fInstance;
    static std::vector<ERBeamDetModule*> fModules;
    static std::vector<std::vector<ERBeamDetFiber*> > fFibers;
    static Float_t fZ;
    static Float_t fLength;
    static Float_t fFiberWidth;
    static Int_t fRowNofFibers;
    static Int_t fRowNofModules;
    static Int_t fRowNofPixels;
    ERBeamDetSetup();
public:
    static ERBeamDetSetup* Instance();
    static Int_t   NofFibers();
    static Int_t   NofPixels();
    static Int_t   NofModules();
    static Float_t FiberLength();
    static Float_t FiberWidth();
    static Float_t ModuleX(Int_t iModule);
    static Float_t ModuleY(Int_t iModule);
    static Float_t FiberX(Int_t iModule, Int_t iFiber);
    static Float_t FiberY(Int_t iModule, Int_t iFiber);
    static Float_t PixelQuantumEfficiency(Int_t iModule, Int_t iFiber);
    static Float_t PixelGain(Int_t iModule, Int_t iFiber);
    static Float_t PixelSigma(Int_t iModule, Int_t iFiber);
    static Float_t Z(){return fZ;}
    static void    Print();
    static void Crosstalks(Int_t iFiber, TArrayF& crosstalks);
    static Int_t RowNofFibers();
    static Int_t RowNofPixels();
    static Int_t RowNofModules();
    static Bool_t UseCrosstalks();
    static Int_t SetParContainers();
    ClassDef(ERBeamDetSetup,1)

};
*/
#endif

