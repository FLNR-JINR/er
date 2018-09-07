#ifndef EREASFUNCOFANGLESGENERATOR_H
#define EREASFUNCOFANGLESGENERATOR_H

#include <FairBoxGenerator.h> // mother class

class FairPrimaryGenerator;
class TH1D;

class EREasFuncOfAnglesGenerator : public FairBoxGenerator
{
public:
    EREasFuncOfAnglesGenerator();
    EREasFuncOfAnglesGenerator(Int_t pdgid, Int_t mult=1);
    virtual ~EREasFuncOfAnglesGenerator();

    Bool_t Init();

    virtual Bool_t ReadEvent(FairPrimaryGenerator* primGen);

public:
    void SetPhiRange     (Double32_t phimin=0, Double32_t phimax=360.)
    {fPhiMin=phimin; fPhiMax=phimax;};

    void SetThetaRange   (Double32_t thetamin=0, Double32_t thetamax=90.)
    {fThetaMin=thetamin; fThetaMax=thetamax; fThetaRangeIsSet=kTRUE;};

    void SetCosTheta   ()
    {fCosThetaIsSet=kTRUE;};

    void SetXYZ   (Double32_t x=0, Double32_t y=0, Double32_t z=0) {
      fX=x;
      fY=y;
      fZ=z;
      fPointVtxIsSet=kTRUE;
    }

    void SetBoxXYZ (Double32_t x1=0, Double32_t y1=0, Double32_t z1=0, Double32_t x2=0, Double32_t y2=0, Double32_t z2=0) {
      fX1=x1;
      fY1=y1;
      fX2=x2;
      fY2=y2;
      fZ1=z1;
      fZ2=z2;
      fBoxVtxIsSet=kTRUE;
    }

    void SetUserParameters(Double_t mA, Double_t ma, Double_t mb, Double_t mB, Double_t Ta, Double_t Q) {
      fma = ma;
      fmA = mA;
      fmb = mb;
      fmB = mB;
      fTa = Ta;
      fQ = Q;
    }

private:
    Double32_t fPDGMass;             // Particle mass [GeV]
    Int_t      fPDGType;             // Particle type (PDG encoding)
    Int_t      fMult;                // Multiplicity

    Double32_t fPhiMin, fPhiMax;     // Azimuth angle range [degree]
    Double32_t fThetaMin, fThetaMax; // Polar angle range in lab system [degree]

    Bool_t     fThetaRangeIsSet;     // True if theta range is set
    Bool_t     fCosThetaIsSet;       // True if uniform distribution in

    Double32_t fX, fY, fZ;           // Point vertex coordinates [cm]
    Double32_t fX1, fY1, fX2, fY2, fZ1, fZ2;   // Box vertex coords (x1,y1)->(x2,y2)

    Bool_t     fPointVtxIsSet;       // True if point vertex is set
    Bool_t     fBoxVtxIsSet;         // True if box vertex is set

    // User parameters for the function Tb = f(theta)
    Double_t fma;
    Double_t fmA;
    Double_t fmb;
    Double_t fmB;
    Double_t fTa;
    Double_t fQ;

    ClassDef(EREasFuncOfAnglesGenerator, 1);
};

#endif // EREASFUNCOFANGLESGENERATOR_H
